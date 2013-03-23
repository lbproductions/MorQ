#include "choosedownloadlinksdialog.h"
#include "ui_choosedownloadlinksdialog.h"

#include "model/episode.h"
#include "model/videodownloadlink.h"

#include "controller/controller.h"
#include "controller/linkscontroller.h"

ChooseDownloadLinksDialog::ChooseDownloadLinksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDownloadLinksDialog)
{
    ui->setupUi(this);

    connect(ui->comboBoxFormat, &QComboBox::currentTextChanged,
            this, &ChooseDownloadLinksDialog::addMirrorsBasedOnSelectedFormat);

    connect(this, &QDialog::accepted,
            this, &ChooseDownloadLinksDialog::finish);
}

ChooseDownloadLinksDialog::~ChooseDownloadLinksDialog()
{
    delete ui;
}

void ChooseDownloadLinksDialog::setEpisodes(QList<Episode *> episodes)
{
    ui->comboBoxFormat->clear();

    QStringList formats;
    m_episodes = episodes;

    foreach(Episode *episode, episodes) {
        foreach(VideoDownloadLink *link, episode->downloadLinks()) {
            if(formats.contains(link->formatDescription()))
                continue;

            formats.append(link->formatDescription());
            ui->comboBoxFormat->addItem(link->formatDescription());
        }
    }

    addMirrorsBasedOnSelectedFormat();
}

void ChooseDownloadLinksDialog::addMirrorsBasedOnSelectedFormat()
{
    ui->comboBoxMirror->clear();

    foreach(Episode *episode, m_episodes) {
        foreach(VideoDownloadLink *link, episode->downloadLinks()) {
            if(link->formatDescription() != ui->comboBoxFormat->currentText() || ui->comboBoxMirror->findText(link->mirror()) != -1)
                continue;

            ui->comboBoxMirror->addItem(link->mirror());
        }
    }
}

void ChooseDownloadLinksDialog::finish()
{
    QList<VideoDownloadLink*> links;
    foreach(Episode *episode, m_episodes) {
        foreach(VideoDownloadLink *link, episode->downloadLinks()) {
            if(link->formatDescription() == ui->comboBoxFormat->currentText() && link->mirror() == ui->comboBoxMirror->currentText()) {
                links.append(link);
            }
        }
    }

    Controller::links()->downloadVideos(links);
}
