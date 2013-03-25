#include "rescancollectiondialog.h"
#include "ui_rescancollectiondialog.h"

#include "seriessearchresultitemdelegate.h"

#include "controller/filescraper.h"
#include "controller/controller.h"
#include "controller/plugincontroller.h"
#include "plugins/downloadProviders/downloadproviderplugin.h"
#include "plugins/downloadProviders/serienjunkiesproviderplugin.h"
#include "model/series.h"
#include "model/episode.h"
#include "model/season.h"
#include "plugins/informationProviders/thetvdbinformationprovider.h"
#include "ui/mainwindow/model/serieslistmodel.h"

#include <QPersistenceSimpleDataAccessObject.h>

#include <QPushButton>

RescanCollectionDialog::RescanCollectionDialog(QWidget *parent) :
    QDialog(parent),
    m_scraper(new FileScraper(this)),
    ui(new Ui::RescanCollectionDialog),
    m_currentSeries(nullptr),
    m_provider(nullptr),
    m_seriesDao(nullptr),
    m_seriesListModel(nullptr),
    m_totalNewSeries(0),
    m_currentEpisode(nullptr),
    m_totalNewEpisodes(0),
    m_currentScrapingEpisode(0)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    ui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(m_scraper, &FileScraper::finished,
            this, &RescanCollectionDialog::checkForNewSeries);

    connect(ui->lineEditSearch, &QLineEdit::returnPressed,
            ui->pushButtonSearch, &QPushButton::click);
    connect(ui->pushButtonSearch, &QPushButton::clicked,
            this, &RescanCollectionDialog::search);

    connect(ui->pushButtonContinue, &QPushButton::clicked,
            this, &RescanCollectionDialog::saveTvdbResultAndContinueToNextSeries);
    connect(ui->pushButtonIgnoreFolder, &QPushButton::clicked,
            this, &RescanCollectionDialog::ignoreCurrentFolderAndContinueToNextSeries);
    connect(ui->pushButtonSkip, &QPushButton::clicked,
            this, &RescanCollectionDialog::skipCurrentSeries);

    connect(ui->pushButtonClose, &QPushButton::clicked,
            this, &QDialog::accept);
}

RescanCollectionDialog::~RescanCollectionDialog()
{
    delete ui;
}

void RescanCollectionDialog::scan()
{
    QString message = tr("Scraping folders...");
    ui->progressBar->setRange(0, 0);
    ui->labelStatus->setText(message);
    ui->textEdit->append(message);
    m_scraper->scanAllLocationsForShowsSeasonsAndEpisodesAsync();
}

void RescanCollectionDialog::checkForNewSeries()
{
    QString message = tr("Checking for new series...");
    ui->labelStatus->setText(message);
    ui->textEdit->append(message);

    if(m_scraper->newSeries().isEmpty()) {
        finish();
    }
    else {
        ui->textEdit->append(tr("Found %1 new seasons and %2 new episodes.")
                             .arg(m_scraper->newSeasons().size())
                             .arg(m_scraper->newEpisodes().size()));

        m_newSeries = m_scraper->newSeries();
        m_totalNewSeries = m_newSeries.size();
        confirmNextNewSeries();
    }
}

void RescanCollectionDialog::confirmNextNewSeries()
{
    m_currentSeries = m_newSeries.takeFirst();
    ui->labelProgress->setText(tr("TV Show %1 of %2")
                               .arg(m_totalNewSeries - m_newSeries.size())
                               .arg(m_totalNewSeries));

    ui->stackedWidget->setCurrentWidget(ui->pageConfirmTvdb);
    ui->lineEditSearch->setText(m_currentSeries->title());
    ui->lineEditSearch->setFocus();
    ui->pushButtonSearch->setDefault(true);
    ui->labelSeriesTitle->setText(m_currentSeries->title());

    search();
}

void RescanCollectionDialog::search()
{
    delete m_provider;
    m_provider = new TheTvdbInformationProvider(this);

    connect(m_provider, &InformationProviderPlugin::finished,
            this, &RescanCollectionDialog::displaySearchResults);

    ui->pushButtonSearch->setEnabled(false);
    ui->lineEditSearch->setEnabled(false);
    ui->pushButtonContinue->setEnabled(false);
    ui->pushButtonIgnoreFolder->setEnabled(false);
    ui->pushButtonSkip->setEnabled(false);

    m_provider->searchSeries(ui->lineEditSearch->text());
}

void RescanCollectionDialog::displaySearchResults()
{
    ui->treeView->setModel(nullptr);
    delete m_seriesDao;
    delete m_seriesListModel;

    m_seriesDao = new QPersistenceSimpleDataAccessObject<Series>(this);
    m_seriesListModel = new SeriesListModel(m_seriesDao, this);
//    m_seriesListModel->setCheckable(true);

    foreach(Series *series, m_provider->seriesSearchResults()) {
        m_seriesDao->insert(series);
        connect(series, &Series::checkStateChanged,
                this, &RescanCollectionDialog::enableContinueButtonBasedOnCheckStates);
    }

    ui->treeView->setModel(m_seriesListModel);

    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &RescanCollectionDialog::showSelectedSeries);

    // Reset the delegate, because our noisygradientitemdelegate needs a valid selection model to work,
    // which only exists, after the model has been set.
    delete ui->treeView->itemDelegate();
    ui->treeView->setItemDelegate(new SeriesSearchResultItemDelegate(ui->treeView, this));

    ui->seriesWidget->setVisible(false);

    if(m_seriesDao->count() > 0) {
        ui->treeView->selectionModel()->select(m_seriesListModel->index(0), QItemSelectionModel::Select);
        ui->pushButtonContinue->setEnabled(true);
    }
    else {
        // TODO: TVDB search: Show "No results" page
    }

    ui->pushButtonSearch->setEnabled(true);
    ui->lineEditSearch->setEnabled(true);
    ui->pushButtonIgnoreFolder->setEnabled(false); // TODO: Enable Ignore button, when it is implemented
    ui->pushButtonSkip->setEnabled(true);
}

void RescanCollectionDialog::showSelectedSeries()
{
    Series *series = m_seriesListModel->objectByIndex(ui->treeView->selectionModel()->selectedIndexes().first());
    ui->seriesWidget->setSeries(series);
    ui->seriesWidget->setVisible(true);
}

void RescanCollectionDialog::saveTvdbResultAndContinueToNextSeries()
{
    Series *series = m_seriesListModel->checkedSeries();
    m_provider->copySeries(series, m_currentSeries);
    ui->textEdit->append(tr("Set TVDB id of %1 to %2.")
                         .arg(series->title())
                         .arg(series->tvdbId()));

    foreach(series, m_seriesListModel->partiallyCheckedSeries()) {
        m_currentSeries->addLanguage(series->primaryLanguage());
    }

//    searchDownlaodsAtSerienjunkies();

    QPersistence::update(m_currentSeries);
    m_scrapedSeries.append(m_currentSeries);
    m_currentSeries = nullptr;

    continueToNextSeriesOrStartScraping();
}

void RescanCollectionDialog::ignoreCurrentFolderAndContinueToNextSeries()
{
    // TODO: remove the current series and add it to the ignored folders
    skipCurrentSeries();
}

void RescanCollectionDialog::continueToNextSeriesOrStartScraping()
{
    cleanupTvdbResultsPage();

    if(!m_newSeries.isEmpty()) {
        confirmNextNewSeries();
    }
    else {
        scrape();
    }
}

void RescanCollectionDialog::skipCurrentSeries()
{
    continueToNextSeriesOrStartScraping();
}

void RescanCollectionDialog::cleanupTvdbResultsPage()
{
    ui->treeView->setModel(nullptr);

    delete m_provider;
    delete m_seriesDao;
    delete m_seriesListModel;

    m_provider = nullptr;
    m_seriesDao = nullptr;
    m_seriesListModel = nullptr;
}

void RescanCollectionDialog::enableContinueButtonBasedOnCheckStates(Qt::CheckState oldState, Qt::CheckState newState)
{
    if(oldState == Qt::Checked) {
        ui->pushButtonContinue->setEnabled(false);
    }

    if(newState == Qt::Checked) {
        ui->pushButtonContinue->setEnabled(true);
    }
}

void RescanCollectionDialog::searchDownlaodsAtSerienjunkies()
{
    DownloadProviderPlugin *plugin = Controller::plugins()->downloadProviderPluginByName("serienjunkies.org");

    plugin->searchSeries(m_currentSeries->title());

    connect(plugin, &DownloadProviderPlugin::foundSeries, this, &RescanCollectionDialog::downloadsFoundAtSerienjunkies);
}

void RescanCollectionDialog::downloadsFoundAtSerienjunkies(QList<DownloadProviderPlugin::SeriesData> series)
{

    //TODO: Find a better way to get the right serie (with not the same name)
    Series* serie = Controller::seriesDao()->byTitle(series.first().title);

    if(serie != 0 && serie->serienJunkiesUrl().toString() == ""){
       serie->setSerienJunkiesUrl(series.first().url);
       Controller::plugins()->downloadProviderPluginByName("serienjunkies.org")->findMissingEpisodes(serie);
       ui->textEdit->append("Serienjunkies-Url for " + series.first().title + " found: " + series.first().url.toString());
       QPersistence::update(serie);
    }
}

void RescanCollectionDialog::scrape()
{
    ui->stackedWidget->setCurrentWidget(ui->pageScanFiles);
    ui->labelProgress->setVisible(false);
    ui->pushButtonContinue->setEnabled(false);
    ui->pushButtonIgnoreFolder->setEnabled(false);
    ui->pushButtonSkip->setEnabled(false);

    m_newEpisodes = m_scraper->newEpisodes();

    if(!m_scrapedSeries.isEmpty()) {
        ui->textEdit->append(tr("Scraping new series..."));
    }
    else {
        ui->textEdit->append(tr("No new series to scrape."));
    }

    scrapeNextSeries();
}

void RescanCollectionDialog::scrapeNextSeries()
{
    if(m_currentSeries) {
        QPersistence::update(m_currentSeries);

        foreach(Season *season, m_currentSeries->seasons()) {
            foreach(Episode *episode, season->episodes()) {
                m_newEpisodes.removeAll(episode);
            }
        }

        m_currentSeries = nullptr;
    }

    delete m_provider;
    m_provider = nullptr;

    if(m_scrapedSeries.isEmpty()) {
        if(m_newEpisodes.isEmpty()) {
            ui->textEdit->append(tr("No new episodes left for scraping."));
        }
        else {
            m_totalNewEpisodes = m_newEpisodes.size();
            ui->textEdit->append(tr("Scraping new episodes..."));
        }
        scrapeNextEpisode();
        return;
    }

    while(!m_scrapedSeries.isEmpty()) {
        m_currentSeries = m_scrapedSeries.takeFirst();

        if(m_currentSeries->tvdbId() <= 0) {
            ui->textEdit->append(tr("Skipping %1").arg(m_currentSeries->title()));
            m_currentSeries = nullptr;
        }
        else {
            break;
        }
    }

    if(m_currentSeries) {
        m_provider = new TheTvdbInformationProvider(this);
        connect(m_provider, &InformationProviderPlugin::finished,
                this, &RescanCollectionDialog::scrapeNextSeries);

        QString message = tr("Scraping series: %1")
                .arg(m_currentSeries->title());
        ui->textEdit->append(message);
        ui->labelStatus->setText(message);
        m_provider->scrapeSeriesIncludingEpisodes(m_currentSeries);
    }
    else {
        scrapeNextSeries(); // this should only happen, when there are no series left
        // i.e. this leads to the episode scraping
    }
}

void RescanCollectionDialog::scrapeNextEpisode()
{
    delete m_provider;
    m_provider = nullptr;

    if(m_currentEpisode) {
        QPersistence::update(m_currentEpisode);
        m_currentEpisode = nullptr;
    }

    if(m_newEpisodes.isEmpty()) {
        finish();
        return;
    }

    while(!m_newEpisodes.isEmpty()) {
        m_currentEpisode = m_newEpisodes.takeFirst();
        ++m_currentScrapingEpisode;

        if(m_currentEpisode->season()->series()->tvdbId() <= 0) {
            ui->textEdit->append(tr("Skipping %1").arg(m_currentEpisode->videoFile()));
            m_currentEpisode = nullptr;
        }
        else {
            break;
        }
    }

    if(m_currentEpisode) {
        m_provider = new TheTvdbInformationProvider(this);
        connect(m_provider, &InformationProviderPlugin::finished,
                this, &RescanCollectionDialog::scrapeNextEpisode);

        QString message = tr("Scraping episode %1 of %2:")
                .arg(m_currentScrapingEpisode)
                .arg(m_totalNewEpisodes);
        ui->textEdit->append(message);
        ui->textEdit->append(m_currentEpisode->videoFile());
        ui->labelStatus->setText(message);
        m_provider->scrapeEpisode(m_currentEpisode);
    }
    else {
        finish(); // this should only happen, when there are no episodes left
                  // i.e. this leads to the finish()
    }
}

void RescanCollectionDialog::finish()
{
    QString message = tr("Finished rescan!");
    ui->textEdit->append(message);
    ui->labelStatus->setText(message);
    ui->pushButtonClose->setEnabled(true);
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(1);
}
