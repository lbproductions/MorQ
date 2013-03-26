#include "videodownloadlink.h"

#include "episode.h"

#include <QDebug>

VideoDownloadLink::VideoDownloadLink(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_episode(nullptr)
{
}

VideoDownloadLink::~VideoDownloadLink()
{
    qDebug() << "~VideoDownloadLink(" << m_id << ")=" << this;

    if(m_episode) {
        m_episode->removeDownloadLink(this);
    }
}

int VideoDownloadLink::id() const
{
    return m_id;
}

void VideoDownloadLink::setId(int id)
{
    m_id  = id;
}

QString VideoDownloadLink::name() const
{
    return m_name;
}

void VideoDownloadLink::setName(const QString &name)
{
    m_name  = name;
}

QString VideoDownloadLink::formatDescription() const
{
    return m_formatDescription;
}

void VideoDownloadLink::setFormatDescription(const QString &description)
{
    m_formatDescription  = description;
}

QUrl VideoDownloadLink::url() const
{
    return m_url;
}

void VideoDownloadLink::setUrl(const QUrl &url)
{
    m_url  = url;
}

QString VideoDownloadLink::mirror() const
{
    return m_mirror;
}

Episode *VideoDownloadLink::episode() const
{
    return m_episode;
}

void VideoDownloadLink::setEpisode(Episode *episode)
{
    m_episode  = episode;
}


void VideoDownloadLink::setMirror(const QString &mirror)
{
    m_mirror = mirror;
}

QString VideoDownloadLink::extractFolder() const
{
    return m_extractFolder;
}

void VideoDownloadLink::setExtractFolder(const QString &extractFolder)
{
    m_extractFolder = extractFolder;
}
