#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "downloadsitemdelegate.h"
#include "serieslistitemdelegate.h"
#include "seasonslistitemdelegate.h"
#include "episodeslistitemdelegate.h"
#include "model/downloadsitemmodel.h"
#include "model/serieslistmodel.h"
#include "model/seasonslistmodel.h"
#include "model/episodeslistmodel.h"
#include "ui/dialogs/newserieswizard.h"
#include "ui/dialogs/choosedownloadlinksdialog.h"
#include "ui/dialogs/rescancollectiondialog.h"
#include "ui/preferences/preferenceswindow.h"
#include "controller/controller.h"
#include "controller/downloadcontroller.h"
#include "controller/extractioncontroller.h"
#include "controller/plugincontroller.h"
#include "controller/scrapercontroller.h"
#include "plugins/downloadProviders/downloadprovider.h"
#include "plugins/downloadProviders/serienjunkiesprovider.h"

#include "plugins/scraper/filescraper.h"
#include "plugins/scraper/newseriesscraper.h"

#include "model/download.h"
#include "model/downloadpackage.h"
#include "model/episode.h"
#include "model/season.h"
#include "model/series.h"

#include <QLabel>
#include <QSettings>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QScrollBar>

static const QString WINDOWGEOMETRY("ui/mainwindow/geometry");
static const QString WINDOWSTATE("ui/mainwindow/state");
static const QString DOWNLOADSHEADERSTATE("ui/mainwindow/downloads/headerstate");
static const QString SERIESPAGESPLITTERSTATE("ui/mainwindow/series/splitterstate");

MainWindow *MainWindow::s_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_preferencesWindow(nullptr)
{
    ui->setupUi(this);

    // Init actions for sidebar and menu
    ui->buttonDownloads->setDefaultAction(ui->actionDownloads);
    ui->buttonSeries->setDefaultAction(ui->actionTV_Shows);

    QActionGroup *viewActionGroup = new QActionGroup(this);
    viewActionGroup->addAction(ui->actionDownloads);
    viewActionGroup->addAction(ui->actionTV_Shows);

    // Init downloads page
    m_downloadsModel = new DownloadsItemModel(this);

    ui->treeViewDownloads->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewDownloads->setModel(m_downloadsModel);
    ui->treeViewDownloads->setItemDelegate(new DownloadsItemDelegate(this));
    ui->treeViewDownloads->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeViewDownloads->addAction(ui->actionDeleteDownload);
    ui->treeViewDownloads->addAction(ui->actionResetDownload);
    ui->treeViewDownloads->addAction(ui->actionExtract);
    ui->treeViewDownloads->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui->treeViewDownloads->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActionsAccordingToDownloadSelection);

    // Init series page
    m_seriesModel = new SeriesListModel(this);
    ui->treeViewSeries->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewSeries->setModel(m_seriesModel);
    ui->treeViewSeries->setItemDelegate(new SeriesListItemDelegate(ui->treeViewSeries, this));
    ui->treeViewSeries->addAction(ui->actionAddDownload);

    m_seasonsModel = new SeasonsListModel(this);
    m_seasonsModel->setSeries(QSharedPointer<Series>()); // calling this disables updates from the global signals
    ui->treeViewSeasons->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewSeasons->setModel(m_seasonsModel);
    ui->treeViewSeasons->setItemDelegate(new SeasonsListItemDelegate(ui->treeViewSeasons, this));
    ui->treeViewSeasons->addAction(ui->actionAddDownload);

    m_episodesModel = new EpisodesListModel(this);
    m_episodesModel->setSeason(QSharedPointer<Season>()); // calling this disables updates from the global signals
    ui->treeViewEpisodes->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewEpisodes->setModel(m_episodesModel);
    ui->treeViewEpisodes->setItemDelegate(new EpisodesListItemDelegate(ui->treeViewEpisodes, this));
    ui->treeViewEpisodes->addAction(ui->actionAddDownload);

    connect(ui->treeViewSeries->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::showSeasonsForSelectedSeries);
    connect(ui->treeViewSeasons->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::showEpisodesForSelectedSeason);

    connect(ui->treeViewSeries->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActionsAccordingToSeriesSelection);
    connect(ui->treeViewSeasons->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActionsAccordingToSeriesSelection);
    connect(ui->treeViewEpisodes->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::enableActionsAccordingToSeriesSelection);
    connect(static_cast<QApplication*>(QApplication::instance()), &QApplication::focusChanged,
            this, &MainWindow::enableActionsAccordingToSeriesSelection);

    // Restore settings
    QSettings settings;
    restoreGeometry(settings.value(WINDOWGEOMETRY, "").toByteArray());
    restoreState(settings.value(WINDOWSTATE, "").toByteArray());
    ui->treeViewDownloads->header()->restoreState(settings.value(DOWNLOADSHEADERSTATE, "").toByteArray());
    //    ui->splitter_2->restoreState(settings.value(SERIESPAGESPLITTERSTATE, "").toByteArray());

    connect(Controller::downloads(), &DownloadController::statusChanged,
            this, &MainWindow::enableActionsAccordingToDownloadStatus);

    enableActionsAccordingToDownloadSelection();
}

MainWindow *MainWindow::instance()
{
    if(!s_instance) {
        s_instance = new MainWindow;
    }
    return s_instance;
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue(WINDOWGEOMETRY, saveGeometry());
    settings.setValue(WINDOWSTATE, saveState());
    settings.setValue(DOWNLOADSHEADERSTATE, ui->treeViewDownloads->header()->saveState());
    settings.setValue(SERIESPAGESPLITTERSTATE, ui->splitter_2->saveState());

    delete ui;
}

void MainWindow::on_actionDownloads_triggered()
{
    ui->centralStackedWidget->setCurrentWidget(ui->pageDownloads);
}

void MainWindow::on_actionTV_Shows_triggered()
{
    ui->centralStackedWidget->setCurrentWidget(ui->pageSeries);
}

void MainWindow::on_actionPreferences_triggered()
{
    if(!m_preferencesWindow) {
        m_preferencesWindow = new PreferencesWindow;
        connect(m_preferencesWindow, &QObject::destroyed, [&]() {
            m_preferencesWindow = nullptr;
        });
    }

    m_preferencesWindow->raise();
    m_preferencesWindow->activateWindow();
    m_preferencesWindow->show();
}

void MainWindow::on_actionDownload_Preferences_triggered()
{
    on_actionPreferences_triggered();
    if(!m_preferencesWindow)
        return;

    m_preferencesWindow->setCurrentPage(PreferencesWindow::DownloadsPage);
}

void MainWindow::on_actionPremiumizeMe_Preferences_triggered()
{
    on_actionPreferences_triggered();
    if(!m_preferencesWindow)
        return;

    m_preferencesWindow->setCurrentPage(PreferencesWindow::PremiumizeMePage);
}

void MainWindow::on_actionStart_triggered()
{
    Controller::downloads()->startDownloads();
}

void MainWindow::on_actionStopDownloads_triggered()
{
    Controller::downloads()->stopDownloads();
}

void MainWindow::enableActionsAccordingToDownloadStatus()
{
    bool running = Controller::downloads()->isDownloadRunning();

    ui->actionStart->setEnabled(!running);
    ui->actionStopDownloads->setEnabled(running);
}

void MainWindow::on_actionDeleteDownload_triggered()
{
    // TODO: confirmation
    //    QMessageBox confirmDialog(this);
    //    confirmDialog.setText(tr("Do you really want to remove the selected downloads?"));
    //    confirmDialog.setWindowTitle(tr("Confirm remove"));
    //    confirmDialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //    confirmDialog.setDefaultButton(QMessageBox::Yes);


    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();

    QList<QSharedPointer<Download> > selectedDownloads;
    QList<QSharedPointer<DownloadPackage> > selectedPackages;

    foreach(QModelIndex index, list) {
        if(index.parent().isValid()) {
            selectedDownloads.append(m_downloadsModel->downloadByIndex(index));
        }
        else if(!index.parent().isValid()) {
            selectedPackages.append(m_downloadsModel->packageByIndex(index));
        }
    }

    //    QString informativeText(tr("Packages:\n"));
    //    foreach(QSharedPointer<DownloadPackage> package, selectedPackages) {
    //        informativeText += package->name() + "\n";
    //    }
    //    informativeText += tr("Downloads:\n");
    //    foreach(QSharedPointer<Download> dl, selectedDownloads) {
    //        informativeText += dl->fileName() + "\n";
    //    }
    //    confirmDialog.setInformativeText(informativeText);
    //    int result = confirmDialog.exec();
    //    if(result != QMessageBox::Yes)
    //        return;

    foreach(QSharedPointer<Download> dl, selectedDownloads) {
        if(dl)
            Controller::downloads()->removeDownload(dl);
    }

    foreach(QSharedPointer<DownloadPackage> package, selectedPackages) {
        if(package)
            Controller::downloads()->removePackage(package);
    }
}

void MainWindow::on_actionResetDownload_triggered()
{
    // TODO: confirmation
    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();

    QList<QSharedPointer<Download> > selectedDownloads;
    QList<QSharedPointer<DownloadPackage> > selectedPackages;

    foreach(QModelIndex index, list) {
        if(index.parent().isValid()) {
            selectedDownloads.append(m_downloadsModel->downloadByIndex(index));
        }
        else if(!index.parent().isValid()) {
            selectedPackages.append(m_downloadsModel->packageByIndex(index));
        }
    }

    foreach(QSharedPointer<Download> dl, selectedDownloads) {
        if(dl)
            Controller::downloads()->resetDownload(dl);
    }

    foreach(QSharedPointer<DownloadPackage> package, selectedPackages) {
        if(package)
            Controller::downloads()->resetPackage(package);
    }
}

void MainWindow::enableActionsAccordingToDownloadSelection()
{
    bool sel = !ui->treeViewDownloads->selectionModel()->selectedRows().isEmpty()
            && ui->centralStackedWidget->currentWidget() == ui->pageDownloads;

    ui->actionDeleteDownload->setEnabled(sel);
    ui->actionResetDownload->setEnabled(sel);
    ui->actionExtract->setEnabled(sel);
}

void MainWindow::on_actionExtract_triggered()
{
    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();
    QSet<QSharedPointer<DownloadPackage> > selectedPackages;

    foreach(QModelIndex index, list) {
        if(index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->downloadByIndex(index)->package());
        }
        else if(!index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->packageByIndex(index));
        }
    }

    foreach(QSharedPointer<DownloadPackage> package, selectedPackages) {
        if(package)
            Controller::extractor()->extractPackage(package);
    }
}

void MainWindow::on_actionAdd_show_triggered()
{
    ui->actionTV_Shows->trigger();

//    NewSeriesWizard wizard(this);
//    wizard.exec();

    //    RescanCollectionDialog dialog(new NewSeriesScraper(wizard.seriesTitle(),this), this);
    //    dialog.scan();
    //    dialog.exec();
}

void MainWindow::showSeasonsForSelectedSeries()
{
    QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
    if(list.isEmpty()) {
        return;
    }

    QSharedPointer<Series> series = m_seriesModel->objectByIndex(list.first());
    if(!series) {
        return;
    }

    m_seasonsModel->setSeries(series);
    ui->treeViewSeasons->selectionModel()->select(m_seasonsModel->index(0),
                                                  QItemSelectionModel::Select);
}

void MainWindow::showEpisodesForSelectedSeason()
{
    QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();

    if(list.isEmpty()) {
        m_episodesModel->setSeason(QSharedPointer<Season>());
        return;
    }

    QSharedPointer<Season> season = m_seasonsModel->objectByIndex(list.first());
    m_episodesModel->setSeason(season);
}

void MainWindow::enableActionsAccordingToSeriesSelection()
{
    QWidget *focusWidget = QApplication::focusWidget();

    // TODO implement and enable downloading multiple episodes/seasons/series in one step

    if(focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if(!list.isEmpty()) {
            ui->actionAddDownload->setEnabled(true);
            ui->actionAddDownload->setText(tr("Download episode..."));
        }
        return;
    }

    if(focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if(!list.isEmpty()) {
            ui->actionAddDownload->setEnabled(false); // TODO implement and enable downloading of seasons
            ui->actionAddDownload->setText(tr("Download season..."));
        }
        return;
    }

    if(focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if(!list.isEmpty()) {
            ui->actionAddDownload->setEnabled(false); // TODO implement and enable downloading of complete series
            ui->actionAddDownload->setText(tr("Download complete series..."));
        }
    }
    else {
        ui->actionAddDownload->setEnabled(false);
    }
}


void MainWindow::on_actionAddDownload_triggered()
{
    QWidget *focusWidget = QApplication::focusWidget();

    QList<QSharedPointer<Episode> > episodes;

    if(focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if(list.isEmpty())
            return;

        QSharedPointer<Episode> episode = m_episodesModel->objectByIndex(list.first());
        if(!episode)
            return;

        episodes.append(episode);
    }

    if(focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if(list.isEmpty())
            return;

        QSharedPointer<Season> season = m_seasonsModel->objectByIndex(list.first());
        if(!season)
            return;

        episodes = season->episodes();
    }

    if(focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if(list.isEmpty())
            return;

        QSharedPointer<Series> series = m_seriesModel->objectByIndex(list.first());
        if(!series)
            return;

        foreach(QSharedPointer<Season> season, series->seasons()) {
            episodes.append(season->episodes());
        }
    }
    else {
        ui->actionAddDownload->setEnabled(false);
    }

    ChooseDownloadLinksDialog dialog;
    dialog.setEpisodes(episodes);
    dialog.exec();
}

void MainWindow::on_actionRescan_collection_triggered()
{
    ScraperController *scraperController = new ScraperController(this);
    scraperController->scrapeLocal();

    connect(scraperController, &ScraperController::finishedLocalScrape,
            scraperController, &ScraperController::scrapeMissingTvdbInformation);

    connect(scraperController, &ScraperController::finishedTvdbScrape,
            scraperController, &ScraperController::scrapeSerienjunkiesUrls);
}
