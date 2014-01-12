#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "downloadsitemdelegate.h"
#include "serieslistitemdelegate.h"
#include "seasonslistitemdelegate.h"
#include "episodeslistitemdelegate.h"
#include "headerview.h"
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
#include "misc/tools.h"
#include "controller/linkscontroller.h"
#include "model/onlineresource.h"

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
#include <QDesktopServices>

static const QString WINDOWGEOMETRY("ui/mainwindow/geometry");
static const QString WINDOWSTATE("ui/mainwindow/state");
static const QString DOWNLOADSHEADERSTATE("ui/mainwindow/downloads/headerstate");
static const QString SERIESPAGESPLITTERSTATE("ui/mainwindow/series/splitterstate");
static const QString SERIESSORTINDICATOR("ui/mainwindow/series/sortindicator");
static const QString SERIESSORTROLE("ui/mainwindow/series/sortrole");
static const QString SEASONSSORTINDICATOR("ui/mainwindow/seasons/sortindicator");
static const QString SEASONSSORTROLE("ui/mainwindow/seasons/sortrole");
static const QString EPISODESSSORTINDICATOR("ui/mainwindow/episodes/sortindicator");
static const QString EPISODESSORTROLE("ui/mainwindow/episodes/sortrole");

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
    SeriesListModel *seriesModel = new SeriesListModel(this);
    m_seriesProxyModel = new SeriesSortFilterProxyModel(seriesModel, this);
    ui->treeViewSeries->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewSeries->setModel(m_seriesProxyModel);
    ui->treeViewSeries->setItemDelegate(new SeriesListItemDelegate(ui->treeViewSeries, this));
    ui->treeViewSeries->addAction(ui->actionAddDownload);
    ui->treeViewSeries->addAction(ui->actionShow_in_Finder);
    ui->treeViewSeries->addAction(ui->actionShow_TheTVDB_page);
    ui->treeViewSeries->addAction(ui->actionOpen_SerienJunkies_page);
    m_seriesHeaderView = new HeaderView(ui->treeViewSeries);
    m_seriesHeaderView->setSortModel(m_seriesProxyModel);
    m_seriesHeaderView->setStretchLastSection(true);
    ui->treeViewSeries->setHeader(m_seriesHeaderView);

    m_seasonsModel = new SeasonsListModel(this);
    m_seasonsModel->setSeries(QSharedPointer<Series>()); // calling this disables updates from the global signals
    m_seasonsProxyModel = new SeasonSortFilterProxyModel(m_seasonsModel, this);
    ui->treeViewSeasons->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewSeasons->setModel(m_seasonsProxyModel);
    ui->treeViewSeasons->setItemDelegate(new SeasonsListItemDelegate(ui->treeViewSeasons, this));
    ui->treeViewSeasons->addAction(ui->actionAddDownload);
    ui->treeViewSeasons->addAction(ui->actionShow_in_Finder);
    ui->treeViewSeasons->addAction(ui->actionShow_TheTVDB_page);
    ui->treeViewSeasons->addAction(ui->actionOpen_SerienJunkies_page);
    m_seasonsHeaderView = new HeaderView(ui->treeViewSeasons);
    m_seasonsHeaderView->setSortModel(m_seasonsProxyModel);
    m_seasonsHeaderView->setStretchLastSection(true);
    ui->treeViewSeasons->setHeader(m_seasonsHeaderView);

    m_episodesModel = new EpisodesListModel(this);
    m_episodesModel->setSeason(QSharedPointer<Season>()); // calling this disables updates from the global signals
    m_episodesProxyModel = new EpisodesSortFilterProxyModel(m_episodesModel, this);
    ui->treeViewEpisodes->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->treeViewEpisodes->setModel(m_episodesProxyModel);
    ui->treeViewEpisodes->setItemDelegate(new EpisodesListItemDelegate(ui->treeViewEpisodes, this));
    ui->treeViewEpisodes->addAction(ui->actionAddDownload);
    ui->treeViewEpisodes->addAction(ui->actionShow_in_Finder);
    ui->treeViewEpisodes->addAction(ui->actionShow_TheTVDB_page);
    m_episodesHeaderView = new HeaderView(ui->treeViewEpisodes);
    m_episodesHeaderView->setSortModel(m_episodesProxyModel);
    m_episodesHeaderView->setStretchLastSection(true);
    ui->treeViewEpisodes->setHeader(m_episodesHeaderView);

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
    ui->splitter_2->restoreState(settings.value(SERIESPAGESPLITTERSTATE, "").toByteArray());
    m_seriesProxyModel->setSortRole(settings.value(SERIESSORTROLE, 0).toInt());
    m_seriesHeaderView->setSortIndicator(0, static_cast<Qt::SortOrder>(settings.value(SERIESSORTINDICATOR, Qt::AscendingOrder).toInt()));
    m_seasonsProxyModel->setSortRole(settings.value(SEASONSSORTROLE, 0).toInt());
    m_seasonsHeaderView->setSortIndicator(0, static_cast<Qt::SortOrder>(settings.value(SEASONSSORTINDICATOR, Qt::AscendingOrder).toInt()));
    m_episodesProxyModel->setSortRole(settings.value(EPISODESSORTROLE, 0).toInt());
    m_episodesHeaderView->setSortIndicator(0, static_cast<Qt::SortOrder>(settings.value(EPISODESSSORTINDICATOR, Qt::AscendingOrder).toInt()));

    connect(Controller::downloads(), &DownloadController::statusChanged,
            this, &MainWindow::enableActionsAccordingToDownloadStatus);

    enableActionsAccordingToDownloadSelection();
}

MainWindow *MainWindow::instance()
{
    if (!s_instance) {
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
    settings.setValue(SERIESSORTINDICATOR, static_cast<int>(m_seriesHeaderView->sortIndicatorOrder()));
    settings.setValue(SERIESSORTROLE, m_seriesProxyModel->sortRole());
    settings.setValue(SEASONSSORTINDICATOR, static_cast<int>(m_seasonsHeaderView->sortIndicatorOrder()));
    settings.setValue(SEASONSSORTROLE, m_seasonsProxyModel->sortRole());
    settings.setValue(EPISODESSSORTINDICATOR, static_cast<int>(m_episodesHeaderView->sortIndicatorOrder()));
    settings.setValue(EPISODESSORTROLE, m_episodesProxyModel->sortRole());

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
    if (!m_preferencesWindow) {
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
    if (!m_preferencesWindow)
        return;

    m_preferencesWindow->setCurrentPage(PreferencesWindow::DownloadsPage);
}

void MainWindow::on_actionPremiumizeMe_Preferences_triggered()
{
    on_actionPreferences_triggered();
    if (!m_preferencesWindow)
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

    foreach (QModelIndex index, list) {
        if (index.parent().isValid()) {
            selectedDownloads.append(m_downloadsModel->downloadByIndex(index));
        }
        else if (!index.parent().isValid()) {
            selectedPackages.append(m_downloadsModel->packageByIndex(index));
        }
    }

    //    QString informativeText(tr("Packages:\n"));
    //    foreach (QSharedPointer<DownloadPackage> package, selectedPackages) {
    //        informativeText += package->name() + "\n";
    //    }
    //    informativeText += tr("Downloads:\n");
    //    foreach (QSharedPointer<Download> dl, selectedDownloads) {
    //        informativeText += dl->fileName() + "\n";
    //    }
    //    confirmDialog.setInformativeText(informativeText);
    //    int result = confirmDialog.exec();
    //    if (result != QMessageBox::Yes)
    //        return;

    foreach (QSharedPointer<Download> dl, selectedDownloads) {
        if (dl)
            Controller::downloads()->removeDownload(dl);
    }

    foreach (QSharedPointer<DownloadPackage> package, selectedPackages) {
        if (package)
            Controller::downloads()->removePackage(package);
    }
}

void MainWindow::on_actionResetDownload_triggered()
{
    // TODO: confirmation
    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();

    QList<QSharedPointer<Download> > selectedDownloads;
    QList<QSharedPointer<DownloadPackage> > selectedPackages;

    foreach (QModelIndex index, list) {
        if (index.parent().isValid()) {
            selectedDownloads.append(m_downloadsModel->downloadByIndex(index));
        }
        else if (!index.parent().isValid()) {
            selectedPackages.append(m_downloadsModel->packageByIndex(index));
        }
    }

    foreach (QSharedPointer<Download> dl, selectedDownloads) {
        if (dl)
            Controller::downloads()->resetDownload(dl);
    }

    foreach (QSharedPointer<DownloadPackage> package, selectedPackages) {
        if (package)
            Controller::downloads()->resetPackage(package);
    }
}

void MainWindow::enableActionsAccordingToDownloadSelection()
{
    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();

    bool sel = !list.isEmpty()
            && ui->centralStackedWidget->currentWidget() == ui->pageDownloads;

    ui->actionDeleteDownload->setEnabled(sel);
    ui->actionResetDownload->setEnabled(sel);
    ui->actionExtract->setEnabled(sel);

    QSet<QSharedPointer<DownloadPackage> > selectedPackages;
    foreach (QModelIndex index, list) {
        if (index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->downloadByIndex(index)->package());
        }
        else if (!index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->packageByIndex(index));
        }
    }
    bool enableExtract = false;
    foreach (QSharedPointer<DownloadPackage> package, selectedPackages) {
        if (package->isDownloadFinished())
            enableExtract = true;
    }
    ui->actionExtract->setEnabled(enableExtract);

    bool downloadsAvailable = false;
    foreach (QSharedPointer<DownloadPackage> p, Qp::readAll<DownloadPackage>()) {
        if (!p->isDownloadFinished())
            downloadsAvailable = true;
    }

    bool running = Controller::downloads()->isDownloadRunning();
    ui->actionStart->setEnabled(/*downloadsAvailable &&*/ !running);
}

void MainWindow::on_actionExtract_triggered()
{
    QModelIndexList list = ui->treeViewDownloads->selectionModel()->selectedRows();
    QSet<QSharedPointer<DownloadPackage> > selectedPackages;

    foreach (QModelIndex index, list) {
        if (index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->downloadByIndex(index)->package());
        }
        else if (!index.parent().isValid()) {
            selectedPackages.insert(m_downloadsModel->packageByIndex(index));
        }
    }

    foreach (QSharedPointer<DownloadPackage> package, selectedPackages) {
        if (package)
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
    if (list.isEmpty()) {
        return;
    }

    QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
    if (!series) {
        return;
    }

    m_seasonsModel->setSeries(series);
    ui->treeViewSeasons->selectionModel()->select(m_seasonsProxyModel->index(0,0),
                                                  QItemSelectionModel::Select);
}

void MainWindow::showEpisodesForSelectedSeason()
{
    QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();

    if (list.isEmpty()) {
        m_episodesModel->setSeason(QSharedPointer<Season>());
        return;
    }

    QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
    m_episodesModel->setSeason(season);
}

void MainWindow::enableActionsAccordingToSeriesSelection()
{
    QWidget *focusWidget = QApplication::focusWidget();
    ui->actionAddDownload->setEnabled(false);
    ui->actionShow_in_Finder->setEnabled(false);
    ui->actionShow_TheTVDB_page->setEnabled(false);
    ui->actionOpen_SerienJunkies_page->setEnabled(false);

    if (focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if (!list.isEmpty()) {
            ui->actionAddDownload->setText(tr("Download episode..."));

            QSharedPointer<Episode> episode = m_episodesProxyModel->objectByIndex(list.first());
            if (!episode)
                return;

            ui->actionAddDownload->setEnabled(!episode->downloadLinks().isEmpty()
                                              && episode->videoFile().isEmpty());
            ui->actionShow_in_Finder->setEnabled(!episode->videoFile().isEmpty());
            ui->actionShow_TheTVDB_page->setEnabled(episode->tvdbUrl().isValid());
        }
        return;
    }

    if (focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if (!list.isEmpty()) {
            ui->actionAddDownload->setEnabled(false); // TODO implement and enable downloading of seasons
            ui->actionAddDownload->setText(tr("Download season..."));

            QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
            if(!season)
                return;

            ui->actionAddDownload->setEnabled(season->status() == Season::DownloadsAvailable);

            ui->actionShow_in_Finder->setEnabled(season && !season->folders().isEmpty());
            ui->actionShow_TheTVDB_page->setEnabled(season->tvdbUrl().isValid());
            ui->actionOpen_SerienJunkies_page->setEnabled(!season->serienjunkiesUrls().isEmpty());
        }
        return;
    }

    if (focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if (!list.isEmpty()) {
            ui->actionAddDownload->setEnabled(false); // TODO implement and enable downloading of complete series
            ui->actionAddDownload->setText(tr("Download complete series..."));

            QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
            if(!series)
                return;

            ui->actionAddDownload->setEnabled(series->status() == Series::DownloadsAvailable);
            ui->actionShow_in_Finder->setEnabled(!series->folders().isEmpty());
            ui->actionShow_TheTVDB_page->setEnabled(series->tvdbUrl().isValid());
            ui->actionOpen_SerienJunkies_page->setEnabled(series->serienJunkiesUrl().isValid());
        }
    }
}


void MainWindow::on_actionAddDownload_triggered()
{
    QWidget *focusWidget = QApplication::focusWidget();

    QList<QSharedPointer<Episode> > episodes;

    if (focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Episode> episode = m_episodesProxyModel->objectByIndex(list.first());
        if (!episode)
            return;

        episodes.append(episode);
    }

    if (focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
        if (!season)
            return;

        episodes = season->episodes();
    }

    if (focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
        if (!series)
            return;

        foreach (QSharedPointer<Season> season, series->seasons()) {
            episodes.append(season->episodes());
        }
    }
    else {
        ui->actionAddDownload->setEnabled(false);
    }

    Controller::links()->downloadEpisodes(episodes);
}

void MainWindow::on_actionRescan_collection_triggered()
{
    ui->statusbar->setWorking(true);
    ui->statusbar->setLeftMessage(tr("Scraping disk..."));
    ui->actionRescan_collection->setEnabled(false);
    ui->actionRescan_collection->setText(tr("Scraping..."));

    ScraperController *scraperController = new ScraperController(this);
    scraperController->scrapeLocal();

    connect(scraperController, &ScraperController::finishedLocalScrape, [=]{
        ui->statusbar->setLeftMessage(tr("Scraping TheTVDB..."));
        scraperController->scrapeMissingTvdbInformation();
    });

    connect(scraperController, &ScraperController::finishedTvdbScrape, [=]{
        ui->statusbar->setLeftMessage(tr("Scraping serienjunkies.org..."));
        scraperController->scrapeSerienjunkiesUrls();
    });

    connect(scraperController, &ScraperController::finishedSerienjunkiesScrape, [=]{
        ui->statusbar->setLeftMessage(tr(""));
        ui->statusbar->setWorking(false);
        ui->actionRescan_collection->setEnabled(true);
        ui->actionRescan_collection->setText(tr("Scrape collection"));
        scraperController->deleteLater();
    });
}

void MainWindow::on_actionShow_in_Finder_triggered()
{
    QWidget *focusWidget = QApplication::focusWidget();

    if (focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Episode> episode = m_episodesProxyModel->objectByIndex(list.first());
        if (!episode || episode->videoFile().isEmpty())
            return;

        Tools::showInGraphicalShell(this, episode->videoFile());
    }

    if (focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
        if (!season || season->folders().isEmpty())
            return;

        Tools::showInGraphicalShell(this, season->folders().first());
    }

    if (focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
        if (!series || series->folders().isEmpty())
            return;

        Tools::showInGraphicalShell(this, series->folders().first());
    }
}

void MainWindow::on_actionShow_TheTVDB_page_triggered()
{
    QWidget *focusWidget = QApplication::focusWidget();

    if (focusWidget == ui->treeViewEpisodes) {
        QModelIndexList list = ui->treeViewEpisodes->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Episode> episode = m_episodesProxyModel->objectByIndex(list.first());
        if (!episode || !episode->tvdbUrl().isValid())
            return;

        QDesktopServices::openUrl(episode->tvdbUrl());
    }

    if (focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
        if (!season || !season->tvdbUrl().isValid())
            return;

        QDesktopServices::openUrl(season->tvdbUrl());;
    }

    if (focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
        if (!series || !series->tvdbUrl().isValid())
            return;

        QDesktopServices::openUrl(series->tvdbUrl());
    }
}

void MainWindow::on_actionOpen_SerienJunkies_page_triggered()
{
    QWidget *focusWidget = QApplication::focusWidget();

    if (focusWidget == ui->treeViewSeasons) {
        QModelIndexList list = ui->treeViewSeasons->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Season> season = m_seasonsProxyModel->objectByIndex(list.first());
        if (!season || season->serienjunkiesUrls().isEmpty())
            return;

        QDesktopServices::openUrl(season->serienjunkiesUrls().first()->url());
    }

    if (focusWidget == ui->treeViewSeries) {
        QModelIndexList list = ui->treeViewSeries->selectionModel()->selectedRows();
        if (list.isEmpty())
            return;

        QSharedPointer<Series> series = m_seriesProxyModel->objectByIndex(list.first());
        if (!series || !series->serienJunkiesUrl().isValid())
            return;

        QDesktopServices::openUrl(series->serienJunkiesUrl());
    }
}
