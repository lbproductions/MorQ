#include "preferenceswindow.h"
#include "ui_preferenceswindow.h"

#include "preferences.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QItemSelection>

PreferencesWindow::PreferencesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreferencesWindow)
{
    ui->setupUi(this);

    ui->lineEditUserName->setText(Preferences::premiumizeMeUserName());
    ui->lineEditPassword->setText(Preferences::premiumizeMeUserPassword());

    ui->checkBoxEnableExtract->setChecked(Preferences::extractDownloads());
    ui->lineEditExtractFolder->setEnabled(Preferences::extractDownloads());
    ui->pushButtonChooseExtractFolder->setEnabled(Preferences::extractDownloads());

    ui->lineEditDownloadFolder->setText(Preferences::downloadFolder());
    ui->lineEditExtractFolder->setText(Preferences::extractFolder());

    ui->spinBoxMaxDownloads->setValue(Preferences::maxDownloads());

    ui->listWidgetLocations->addItems(Preferences::seriesLocations());
    connect(ui->listWidgetLocations->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PreferencesWindow::enableRemoveLocationButtonUponSelection);

    QActionGroup *viewActionGroup = new QActionGroup(this);
    viewActionGroup->addAction(ui->actionDownloads);
    viewActionGroup->addAction(ui->actionPremuimizeMe);
    viewActionGroup->addAction(ui->actionSeries);

    setAttribute(Qt::WA_DeleteOnClose, true);
}

PreferencesWindow::~PreferencesWindow()
{
    delete ui;
}

void PreferencesWindow::setCurrentPage(PreferencesWindow::Page page)
{
    switch(page) {
    default:
    case DownloadsPage:
        on_actionDownloads_triggered();
        break;
    case PremiumizeMePage:
        on_actionPremuimizeMe_triggered();
        break;
    }
}

void PreferencesWindow::on_lineEditUserName_editingFinished()
{
    Preferences::setPremiumizeMeUserName(ui->lineEditUserName->text());
}

void PreferencesWindow::on_lineEditPassword_editingFinished()
{
    Preferences::setPremiumizeMeUserPassword(ui->lineEditPassword->text());
}

void PreferencesWindow::on_checkBoxEnableExtract_clicked()
{
    ui->lineEditExtractFolder->setEnabled(ui->checkBoxEnableExtract->isChecked());
    ui->pushButtonChooseExtractFolder->setEnabled(ui->checkBoxEnableExtract->isChecked());
    Preferences::setExtractDownloads(ui->checkBoxEnableExtract->isChecked());
}

void PreferencesWindow::on_actionPremuimizeMe_triggered()
{
    ui->actionPremuimizeMe->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pagePremiumize);
}

void PreferencesWindow::on_actionDownloads_triggered()
{
    ui->actionDownloads->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pageDownloads);
}

void PreferencesWindow::on_actionSeries_triggered()
{
    ui->actionSeries->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->pageSeries);
}

void PreferencesWindow::on_lineEditDownloadFolder_editingFinished()
{
    Preferences::setDownloadFolder(ui->lineEditDownloadFolder->text());
}

void PreferencesWindow::on_lineEditExtractFolder_editingFinished()
{
    Preferences::setExtractFolder(ui->lineEditExtractFolder->text());
}

void PreferencesWindow::on_pushButtonChooseDownloadFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this);
    if(folder.isEmpty())
        return;

    ui->lineEditDownloadFolder->setText(folder);
    Preferences::setDownloadFolder(folder);
}

void PreferencesWindow::on_pushButtonChooseExtractFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this);
    if(folder.isEmpty())
        return;

    ui->lineEditExtractFolder->setText(folder);
    Preferences::setExtractFolder(folder);
}

void PreferencesWindow::on_spinBoxMaxDownloads_editingFinished()
{
    Preferences::setMaxDownloads(ui->spinBoxMaxDownloads->value());
}


void PreferencesWindow::on_pushButtonAddLocation_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this);
    if(folder.isEmpty())
        return;

    if(Preferences::seriesLocations().contains(folder)) {
        QMessageBox msg(this);
        msg.setWindowTitle(tr(""));
        msg.setText(tr("Location exists"));
        msg.setInformativeText(tr("You already added this location to MorQ."));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setModal(true);
        msg.setWindowModality(Qt::WindowModal);
        msg.exec();
        return;
    }

    ui->listWidgetLocations->addItem(folder);
    saveSeriesLocations();
}

void PreferencesWindow::on_pushButtonRemoveLocation_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle(tr(""));
    msg.setText(tr("Do you really want to remove the selected location?"));
    msg.setInformativeText(tr("Do you want to remove all shows in that location from MorQ?"));
    msg.setModal(true);
    msg.setWindowModality(Qt::WindowModal);

    msg.setIcon(QMessageBox::Warning);
    msg.addButton(QMessageBox::Cancel);
    QPushButton *buttonRemove = msg.addButton(tr("Remove shows"), QMessageBox::DestructiveRole);
    buttonRemove->setEnabled(false); // TODO: support removing all series from a remove location
    QPushButton *buttonKeep = msg.addButton(tr("Keep shows"), QMessageBox::YesRole);
    msg.setDefaultButton(buttonKeep);

    msg.exec();
    QAbstractButton *clickedButton = msg.clickedButton();
    if(clickedButton == buttonKeep) {
        delete ui->listWidgetLocations->selectedItems().first();
        saveSeriesLocations();
    }
}

void PreferencesWindow::enableRemoveLocationButtonUponSelection(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    ui->pushButtonRemoveLocation->setEnabled(!selected.isEmpty());
}

void PreferencesWindow::saveSeriesLocations()
{
    QStringList locations;
    int count = ui->listWidgetLocations->count();
    for(int i = 0; i < count; ++i) {
        locations << ui->listWidgetLocations->item(i)->text();
    }

    Preferences::setSeriesLocations(locations);
}
