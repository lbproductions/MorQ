#ifndef SERIENJUNKIESPROVIDERPLUGIN_H
#define SERIENJUNKIESPROVIDERPLUGIN_H

#include "downloadprovider.h"
#include "model/series.h"

class QSerienJunkiesReply;

class SerienjunkiesProvider : public DownloadProvider
{
    Q_OBJECT
public:
    static const QString Name;

    DownloadProviderTask *findDownloadLinks(QSharedPointer<Episode> episode) Q_DECL_OVERRIDE;

private:
    friend class Plugins;
    explicit SerienjunkiesProvider(QObject *parent = 0);
};

class SerienjunkiesProviderTask : public DownloadProviderTask
{
    Q_OBJECT
public:
    ~SerienjunkiesProviderTask();

private slots:
    void seriesSearchFinished();
    void seasonSearchFinished();
    void downloadLinkSearchFinished();

    void handleSerienjunkiesError();

    void _findSeasonUrls();
    void _findAllDownloadLinks();
    void _findDownloadLinks();

private:
    friend class SerienjunkiesProvider;
    explicit SerienjunkiesProviderTask(SerienjunkiesProvider *parent);
    explicit SerienjunkiesProviderTask(SerienjunkiesProviderTask *parent);

    QSharedPointer<Series> series() const;
    void setSeries(const QSharedPointer<Series> &series);
    QSharedPointer<Season> season() const;
    void setSeason(const QSharedPointer<Season> &season);

    void findSeriesUrl(QSharedPointer<Series> series);
    void findSeasonUrls(QSharedPointer<Series> series);
    void findAllDownloadLinks(QSharedPointer<Season> season);
    void findDownloadLinks(QSharedPointer<Episode> episode);

    QList<QSharedPointer<OnlineResource> > takeUrlsForEpisode(QSharedPointer<Episode> episode) const;

    static QHash<QSharedPointer<Series>, SerienjunkiesProviderTask *> s_seriesSearchTasks;
    static QHash<QSharedPointer<Series>, SerienjunkiesProviderTask *> s_seasonSearchTasks;
    static QHash<QSharedPointer<Season>, SerienjunkiesProviderTask *> s_linkSearchTasks;

    QSharedPointer<Series> m_series;
    QSharedPointer<Season> m_season;

    QMultiMap<QSharedPointer<Episode>, QSharedPointer<OnlineResource> > m_urlsByEpisode;

    int m_requestCount;
};

#endif // SERIENJUNKIESPROVIDERPLUGIN_H
