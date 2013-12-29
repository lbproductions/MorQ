#ifndef SERIENJUNKIESPROVIDERPLUGIN_H
#define SERIENJUNKIESPROVIDERPLUGIN_H

#include "downloadproviderplugin.h"
#include "model/series.h"

class QSerienJunkiesReply;

class SerienjunkiesProviderPlugin : public DownloadProviderPlugin
{
    Q_OBJECT
public:
    SerienjunkiesProviderPlugin(QObject *parent = 0);

    void searchSeries(const QString &seriesName);
    bool canHandleSeries(QSharedPointer<Series> series) const;
    void findMissingEpisodes(QSharedPointer<Series> series) const;

    void searchAndSetDownloadsForSeries(QSharedPointer<Series>  series);

private slots:
    void onSeriesFound(QList<DownloadProviderPlugin::SeriesData> series);

private:
    QSerienJunkiesReply *m_latestReply;
};

class SerienjunkiesSearchHandler : public QObject
{
    Q_OBJECT
public:
    SerienjunkiesSearchHandler(QObject *parent);
    void findMissingEpisodes(QSharedPointer<Series> series);

private slots:
    void searchSeasonsFinished();
    void searchEpisodesFinished();

private:
    QSharedPointer<Series> m_series;
    int m_seasonsNotFinishedCount;
};

#endif // SERIENJUNKIESPROVIDERPLUGIN_H
