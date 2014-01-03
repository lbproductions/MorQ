#ifndef SCRAPERCONTROLLER_H
#define SCRAPERCONTROLLER_H

#include <QObject>

#include "model/series.h"
#include "plugins/downloadProviders/downloadproviderplugin.h"

class ScraperController : public QObject
{
    Q_OBJECT
public:
    explicit ScraperController(QObject *parent = 0);

public slots:
    void scanSeriesLocationsForNewSeries();
    void scrapeMissingTvdbInformation();
    void scrapeSerienjunkiesUrls();

signals:
    void foundNewSeries();

private slots:
    void onFileScraperFinish();
    void interpretTvdbSearchResults();
    void interpretSerienjunkiesSearchResults(QList<DownloadProviderPlugin::SeriesData> series);

private:
    QSharedPointer<Series> findCorrectResult(QSharedPointer<Series> series, QList<QSharedPointer<Series> > results);

};

#endif // SCRAPERCONTROLLER_H
