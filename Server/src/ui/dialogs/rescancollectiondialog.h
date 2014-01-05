//#ifndef RESCANCOLLECTIONDIALOG_H
//#define RESCANCOLLECTIONDIALOG_H

//#include <QDialog>

//#include "plugins/downloadProviders/downloadprovider.h"

//namespace Ui {
//class RescanCollectionDialog;
//}

//class Scraper;
//class Series;
//class InformationProvider;
//class SeriesListModel;
//class QItemSelection;
//class Episode;

//class RescanCollectionDialog : public QDialog
//{
//    Q_OBJECT
    
//public:
//    explicit RescanCollectionDialog(Scraper *scraper, QWidget *parent = 0);
//    ~RescanCollectionDialog();
    
//    void scan();

//private slots:
//    void checkForNewSeries();
//    void confirmNextNewSeries();

//    void search();
//    void displaySearchResults();
//    void showSelectedSeries();

//    void saveTvdbResultAndContinueToSerienjunkiesSearch();
//    void ignoreCurrentFolderAndContinueToNextSeries();
//    void continueToNextSeriesOrStartScraping();
//    void skipCurrentSeries();
//    void clear();
//    void enableContinueButtonBasedOnCheckStates(Qt::CheckState oldState, Qt::CheckState newState);

//    void searchDownlaodsAtSerienjunkies();
//    void seriesFoundAtSerienjunkies(QList<DownloadProviderPlugin::SeriesData> series);
//    void enableContinueButtonBasedOnSerienJunkiesURL();
//    void saveSerienjunkiesUrlAndContinueToNextSeries();

//    void scrape();
//    void scrapeNextSeries();
//    void scrapeNextEpisode();

//    void finish();

//private:
//    Scraper *m_scraper;
//    Ui::RescanCollectionDialog *ui;

//    QSharedPointer<Series> m_currentSeries;
//    QList<QSharedPointer<Series> > m_newSeries;
//    InformationProvider *m_provider;
//    QList<QSharedPointer<Series> > m_scrapedSeries;

//    SeriesListModel *m_seriesListModel;
//    int m_totalNewSeries;

//    QList<QSharedPointer<Episode> > m_newEpisodes;
//    QSharedPointer<Episode> m_currentEpisode;
//    int m_totalNewEpisodes;
//    int m_currentScrapingEpisode;
//};

//#endif // RESCANCOLLECTIONDIALOG_H
