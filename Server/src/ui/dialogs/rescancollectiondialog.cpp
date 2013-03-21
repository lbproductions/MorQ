#include "rescancollectiondialog.h"
#include "ui_rescancollectiondialog.h"

#include "seriessearchresultitemdelegate.h"

#include "controller/filescraper.h"
#include "model/series.h"
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
    m_pushButtonRemoveSeries(nullptr),
    m_pushButtonNextSeries(nullptr),
    m_seriesDao(nullptr),
    m_seriesListModel(nullptr)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    ui->treeView->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(m_scraper, &FileScraper::finished,
            this, &RescanCollectionDialog::checkForNewSeries);
    connect(ui->pushButtonSearch, &QPushButton::clicked,
            this, &RescanCollectionDialog::search);
    connect(ui->lineEditSearch, &QLineEdit::returnPressed,
            ui->pushButtonSearch, &QPushButton::click);

    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(false);

    m_pushButtonRemoveSeries = ui->buttonBox->addButton(tr("Remove show"), QDialogButtonBox::ApplyRole);
    m_pushButtonNextSeries = ui->buttonBox->addButton(tr("Next >"), QDialogButtonBox::YesRole);
    m_pushButtonRemoveSeries->setEnabled(false); // TODO: Enable the user to remove newly found series
    m_pushButtonRemoveSeries->setVisible(false);
    m_pushButtonNextSeries->setVisible(false);

    connect(m_pushButtonNextSeries, &QPushButton::clicked,
            this, &RescanCollectionDialog::saveTvdbResult);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked,
            this, &RescanCollectionDialog::saveTvdbResult);
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
        ui->textEdit->append(tr("Found %1 new seasons and %2 new episodes.")
                             .arg(m_scraper->newSeasons().size())
                             .arg(m_scraper->newEpisodes().size()));

        message = tr("Done");
        ui->labelStatus->setText(message);
        ui->textEdit->append(message);
        ui->buttonBox->button(QDialogButtonBox::Close)->setVisible(true);
        ui->progressBar->setRange(0,1);
        ui->progressBar->setValue(1);
        return;
    }

    m_newSeries = m_scraper->newSeries();
    confirmNextNewSeries();
}

void RescanCollectionDialog::confirmNextNewSeries()
{
    if(m_newSeries.isEmpty())
        return;

    m_currentSeries = m_newSeries.takeFirst();

    if(m_newSeries.isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Close)->setVisible(true);
        ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(true);
        m_pushButtonNextSeries->setVisible(false);
    }
    else {
        ui->buttonBox->button(QDialogButtonBox::Close)->setVisible(false);
        m_pushButtonNextSeries->setVisible(true);
    }

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
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(false);
    m_pushButtonNextSeries->setEnabled(false);
    m_provider->searchSeries(ui->lineEditSearch->text());
}

void RescanCollectionDialog::displaySearchResults()
{
    ui->treeView->setModel(nullptr);

    delete m_seriesDao;
    delete m_seriesListModel;
    m_seriesDao = new QPersistenceSimpleDataAccessObject<Series>(this);
    m_seriesListModel = new SeriesListModel(m_seriesDao, this);

    foreach(Series *series, m_provider->results()) {
        m_seriesDao->insert(series);
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
        ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(true);
        m_pushButtonNextSeries->setEnabled(true);
    }
    else {
        // TODO: TVDB search: Show "No results" page
    }

    ui->pushButtonSearch->setEnabled(true);
    ui->lineEditSearch->setEnabled(true);
}

void RescanCollectionDialog::showSelectedSeries()
{
    ui->seriesWidget->setVisible(true);
    Series *series = m_seriesListModel->objectByIndex(ui->treeView->selectionModel()->selectedIndexes().first());
    ui->seriesWidget->setSeries(series);
}

void RescanCollectionDialog::saveTvdbResult()
{
    Series *series = m_seriesListModel->objectByIndex(ui->treeView->selectionModel()->selectedIndexes().first());
    m_provider->copySeries(series, m_currentSeries);

    delete m_provider;
    m_provider = nullptr;

    QPersistence::update(m_currentSeries);

    QList<Series *> seriesList = m_seriesDao->readAll();

    ui->treeView->setModel(nullptr);
    delete m_seriesDao;
    delete m_seriesListModel;
    m_seriesDao = nullptr;
    m_seriesListModel = nullptr;

    foreach(Series *series, seriesList) {
        delete series;
    }

    confirmNextNewSeries();
}

void RescanCollectionDialog::accept()
{
    if(m_newSeries.isEmpty())
        QDialog::accept();
}
