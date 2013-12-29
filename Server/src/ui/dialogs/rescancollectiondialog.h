#ifndef RESCANCOLLECTIONDIALOG_H
#define RESCANCOLLECTIONDIALOG_H

#include <QDialog>

#include "plugins/downloadProviders/downloadproviderplugin.h"

namespace Ui {
class RescanCollectionDialog;
}

class Scraper;
class Series;
class InformationProviderPlugin;
class SeriesListModel;
class QItemSelection;
class Episode;

class RescanCollectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RescanCollectionDialog(Scraper *scraper, QWidget *parent = 0);
    ~RescanCollectionDialog();
    
    void scan();

private slots:
    void checkForNewSeries();
    void confirmNextNewSeries();

    void search();
    void displaySearchResults();
    void showSelectedSeries();

    void saveTvdbResultAndContinueToNextSeries();
    void ignoreCurrentFolderAndContinueToNextSeries();
    void continueToNextSeriesOrStartScraping();
    void skipCurrentSeries();
    void cleanupTvdbResultsPage();
    void enableContinueButtonBasedOnCheckStates(Qt::CheckState oldState, Qt::CheckState newState);

    void searchDownlaodsAtSerienjunkies();
    void downloadsFoundAtSerienjunkies(QList<DownloadProviderPlugin::SeriesData> series);

    void scrape();
    void scrapeNextSeries();
    void scrapeNextEpisode();

    void finish();

private:
    Scraper *m_scraper;
    Ui::RescanCollectionDialog *ui;

    QSharedPointer<Series> m_currentSeries;
    QList<QSharedPointer<Series> > m_newSeries;
    InformationProviderPlugin *m_provider;
    QList<QSharedPointer<Series> > m_scrapedSeries;

    SeriesListModel *m_seriesListModel;
    int m_totalNewSeries;

    QList<QSharedPointer<Episode> > m_newEpisodes;
    QSharedPointer<Episode> m_currentEpisode;
    int m_totalNewEpisodes;
    int m_currentScrapingEpisode;
};

#endif // RESCANCOLLECTIONDIALOG_H
