#ifndef DOWNLOADPROVIDERPLUGIN_H
#define DOWNLOADPROVIDERPLUGIN_H

#include "../plugins.h"

#include "model/episode.h"

#include <QUrl>

class Series;
class DownloadProviderTask;

class DownloadProvider : public Plugin
{
    Q_OBJECT
public:
    virtual DownloadProviderTask *findDownloadLinks(QSharedPointer<Episode> episode) = 0;

protected:
    explicit DownloadProvider(const QString &name, QObject *parent = 0);
};

class DownloadProviderTask : public PluginTask
{
    Q_OBJECT
public:
    QList<QSharedPointer<OnlineResource> > results() const;
    QSharedPointer<Episode> episode() const;

    DownloadProvider *provider() const;

protected:
    explicit DownloadProviderTask(DownloadProvider *parent);

    void setEpisode(const QSharedPointer<Episode> &episode);
    void addResult(QSharedPointer<OnlineResource> results);

    QList<QSharedPointer<OnlineResource> > m_results;
    QSharedPointer<Episode> m_episode;
};

#endif // DOWNLOADPROVIDERPLUGIN_H
