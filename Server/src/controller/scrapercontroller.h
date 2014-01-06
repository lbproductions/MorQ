#ifndef SCRAPERCONTROLLER_H
#define SCRAPERCONTROLLER_H

#include <QObject>

#include "model/series.h"
#include "plugins/downloadProviders/downloadprovider.h"

class ScraperController : public QObject
{
    Q_OBJECT
public:
    explicit ScraperController(QObject *parent = 0);

    static QSharedPointer<Series> inferBestMatchingSeries(QSharedPointer<Series> series, QList<QSharedPointer<Series> > results);

public slots:
    void scrapeLocal();
    void scrapeMissingTvdbInformation();
    void scrapeSerienjunkiesUrls();

signals:
    void finishedLocalScrape();
    void finishedTvdbScrape();
    void finishedSerienjunkiesScrape();

private slots:
    void onFileScraperFinish();
    void interpretTvdbSearchResults();
//    void interpretSerienjunkiesSearchResults(QList<DownloadProvider::SeriesData> series);

private:

};

#endif // SCRAPERCONTROLLER_H
