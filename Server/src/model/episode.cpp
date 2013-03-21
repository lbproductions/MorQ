#include "episode.h"

#include "season.h"
#include "videodownloadlink.h"

#include <QDebug>

Episode::Episode(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_number(0),
    m_season(nullptr)
{
}

Episode::~Episode()
{
    qDebug() << "~Episode(" << m_id << ")=" << this;

    if(m_season) {
        m_season->removeEpisode(this);
    }

    foreach(VideoDownloadLink *link, m_downloadLinks) {
        link->setEpisode(nullptr);
    }
}

int Episode::id() const
{
    return m_id;
}

void Episode::setId(int id)
{
    m_id = id;
}

int Episode::number() const
{
    return m_number;
}

void Episode::setNumber(int number)
{
    m_number = number;
}

QString Episode::serienJunkiesTitle() const
{
    return m_serienJunkiesTitle;
}

void Episode::setSerienJunkiesTitle(const QString &title)
{
    m_serienJunkiesTitle = title;
}

Season *Episode::season() const
{
    return m_season;
}

void Episode::setSeason(Season *season)
{
    m_season = season;
}

QList<VideoDownloadLink *> Episode::downloadLinks() const
{
    return m_downloadLinks;
}

void Episode::addDownloadLink(VideoDownloadLink *link)
{
    Q_ASSERT(!m_downloadLinks.contains(link));

    link->setEpisode(this);
    m_downloadLinks.append(link);
}

void Episode::removeDownloadLink(VideoDownloadLink *link)
{
    Q_ASSERT(!m_downloadLinks.contains(link));

    link->setEpisode(nullptr);
    m_downloadLinks.removeAll(link);
}

void Episode::setDownloadLinks(const QList<VideoDownloadLink *> &links)
{
    m_downloadLinks.clear();

    foreach(VideoDownloadLink *link, links){
        addDownloadLink(link);
    }
}

QStringList Episode::videoFiles() const
{
    return m_videoFiles;
}

void Episode::addVideoFile(const QString &fileName)
{
    if(m_videoFiles.contains(fileName))
        return;

    m_videoFiles.append(fileName);
}

void Episode::setVideoFiles(const QStringList &files)
{
    m_videoFiles = files;
}

QString Episode::title() const
{
    return m_title;
}

void Episode::setTitle(const QString &title)
{
    m_title = title;
}
