#include "downloadprovider.h"

DownloadProvider::DownloadProvider(const QString &name, QObject *parent) :
    Plugin(name, parent)
{
}

DownloadProviderTask::DownloadProviderTask(DownloadProvider *parent) :
    PluginTask(parent)
{
}

QList<QSharedPointer<OnlineResource> > DownloadProviderTask::results() const
{
    return m_results;
}

void DownloadProviderTask::addResult(QSharedPointer<OnlineResource> result)
{
    m_results.append(result);
}

QSharedPointer<Episode> DownloadProviderTask::episode() const
{
    return m_episode;
}

DownloadProvider *DownloadProviderTask::provider() const
{
    return static_cast<DownloadProvider *>(parent());
}

void DownloadProviderTask::setEpisode(const QSharedPointer<Episode> &episode)
{
    m_episode = episode;
}

