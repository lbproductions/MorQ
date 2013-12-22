#include "choosedownloadlinksdialog.h"
#include "ui_choosedownloadlinksdialog.h"

#include "model/episode.h"
#include "model/series.h"
#include "model/season.h"
#include "model/videodownloadlink.h"

#include "controller/controller.h"
#include "controller/linkscontroller.h"

#include "preferences.h"

#include <QDir>

ChooseDownloadLinksDialog::ChooseDownloadLinksDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDownloadLinksDialog)
{
    ui->setupUi(this);

    if(Preferences::extractMode() != "SERIES") {
        ui->comboBoxPath->setEnabled(false);
    }

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
            //if(formats.contains(link->formatDescription()))
            if(formats.contains(link->name()))
                continue;

            formats.append(link->name());
            //formats.append(link->formatDescription());
            //ui->comboBoxFormat->addItem(link->formatDescription());
            ui->comboBoxFormat->addItem(link->name());
        }
        if(episode->season()->folders().size() == 0){
            foreach(QString folder, episode->season()->series()->folders()) {
                if(ui->comboBoxPath->findText(folder) == -1) {
                    ui->comboBoxPath->addItem(folder);
                }
            }       
            ui->checkBoxCreateSeason->setEnabled(true);
        }
        else{
            foreach(QString folder, episode->season()->folders()) {
                if(ui->comboBoxPath->findText(folder) == -1) {
                    ui->comboBoxPath->addItem(folder);
                }
            }
            ui->checkBoxCreateSeason->setChecked(false);
            ui->checkBoxCreateSeason->setEnabled(false);
        }

    }


    addMirrorsBasedOnSelectedFormat();
}

void ChooseDownloadLinksDialog::addMirrorsBasedOnSelectedFormat()
{
    ui->comboBoxMirror->clear();

    foreach(Episode *episode, m_episodes) {
        foreach(VideoDownloadLink *link, episode->downloadLinks()) {
            //if(link->formatDescription() != ui->comboBoxFormat->currentText() || ui->comboBoxMirror->findText(link->mirror()) != -1)
            if(link->name() != ui->comboBoxFormat->currentText() || ui->comboBoxMirror->findText(link->mirror()) != -1)
                continue;

            ui->comboBoxMirror->addItem(link->mirror());
        }
    }
}

void ChooseDownloadLinksDialog::finish()
{
    QString targetFolder;
    if(ui->checkBoxCreateSeason->isEnabled()) {
        QDir dir(ui->comboBoxPath->currentText());
        //TODO: Think about downloading episodes from different seasons
        //TODO: Different names for different languages
        QString newFolder = "Staffel " + QString::number(m_episodes.first()->seasonNumber());
        dir.mkdir(newFolder);
        dir.cd(newFolder);

        targetFolder = dir.absolutePath();

        m_episodes.first()->season()->addFolder(targetFolder);

        QPersistence::update(m_episodes.first()->season());
    }
    else{
        targetFolder = ui->comboBoxPath->currentText();
    }

    QList<VideoDownloadLink*> links;
    foreach(Episode *episode, m_episodes) {
        foreach(VideoDownloadLink *link, episode->downloadLinks()) {
            //if(link->formatDescription() == ui->comboBoxFormat->currentText() && link->mirror() == ui->comboBoxMirror->currentText()) {
            if(link->name() == ui->comboBoxFormat->currentText() && link->mirror() == ui->comboBoxMirror->currentText()) {
                links.append(link);
                if(Preferences::extractMode() == "SERIES") {
                    link->setExtractFolder(targetFolder);
                }
                else{
                    link->setExtractFolder(Preferences::extractFolder());
                }
            }
        }
    }

    Controller::links()->downloadVideos(links);
}
