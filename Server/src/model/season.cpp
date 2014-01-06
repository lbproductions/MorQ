#include "season.h"

#include "episode.h"
#include "series.h"
#include "onlineresource.h"
#include "misc/tools.h"

#include <QDebug>
#include <QPixmap>

Season::Season(QObject *parent) :
    QObject(parent),
    m_number(0),
    m_series("series", this),
    m_episodes("episodes", this),
    m_serienjunkiesUrls("serienjunkiesUrls", this)
{
}

Season::~Season()
{
}

int Season::id() const
{
    return Qp::primaryKey(Qp::sharedFrom(this));
}

int Season::number() const
{
    return m_number;
}

void Season::setNumber(int number)
{
    m_number = number;
}

QString Season::title() const
{
    if(!m_title.isEmpty())
        return m_title;

    if(m_number <= 0)
        return QString("Specials");

    return QString(tr("Season %1")).arg(number());
}

void Season::setTitle(const QString title)
{
    m_title = title;
}

QSharedPointer<Series> Season::series() const
{
    return m_series.resolve();
}

void Season::setSeries(QSharedPointer<Series> series)
{
    m_series.relate(series);
}

QList<QSharedPointer<Episode> > Season::episodes() const
{
    return m_episodes.resolveList();
}

QSharedPointer<Episode> Season::episode(int number) const
{
    if(m_episodesByNumber.isEmpty()) {
        foreach(QSharedPointer<Episode> episode, episodes()) {
            m_episodesByNumber.insert(episode->number(), episode);
        }
    }

    return m_episodesByNumber.value(number);
}

void Season::addEpisode(QSharedPointer<Episode> episode)
{
    episode->setSeason(Qp::sharedFrom(this));
    m_episodesByNumber.insert(episode->number(), episode);
    m_episodes.relate(episode);
}

void Season::removeEpisode(QSharedPointer<Episode> episode)
{
    episode->setSeason(QSharedPointer<Season>());
    m_episodesByNumber.remove(episode->number());
    m_episodes.unrelate(episode);
}

void Season::setEpisodes(const QList<QSharedPointer<Episode> > &episodes)
{
    m_episodesByNumber.clear();
    m_episodes.clear();
    m_episodes.relate(episodes);
}
QList<QSharedPointer<OnlineResource> > Season::serienjunkiesUrls() const
{
    return m_serienjunkiesUrls.resolveList();
}

void Season::setSerienjunkiesUrls(const QList<QSharedPointer<OnlineResource> > &serienjunkiesUrls)
{
    m_serienjunkiesUrls.clear();
    m_serienjunkiesUrls.relate(serienjunkiesUrls);
}

Season::Status Season::status() const
{
    int downloads = 0;
    int missing = 0;
    int okay = 0;

    foreach(QSharedPointer<Episode> e, episodes()) {
        switch(e->status()) {
        case Episode::DownloadAvailable:
            ++downloads;
            break;
        case Episode::Missing:
            ++missing;
            break;
        case Episode::Ok:
            ++okay;
        case Episode::Downloading:
        case Episode::Extracting:
        default:
        case UnkownStatus:
            break;
        }
    }

    if(downloads > 0)
        return Season::DownloadsAvailable;

    if(missing > 0)
        return Season::MissingEpisodes;

    if(okay == episodes().count())
        return Season::Complete;

    return Season::UnkownStatus;
}

QPixmap Season::statusPixmap() const
{
    switch(status()) {
    case DownloadsAvailable:
        return Tools::cachedPixmap(":/icons/download_available");
    case MissingEpisodes:
        return Tools::cachedPixmap(":/icons/missing");
    case Complete:
        return Tools::cachedPixmap(":/icons/okay");
    default:
    case UnkownStatus:
        break;
    }

    return Tools::cachedPixmap(":/icons/questionmark");
}

QString Season::statusMessage() const
{
    int downloads = 0;
    int missing = 0;

    foreach(QSharedPointer<Episode> e, episodes()) {
        switch(e->status()) {
        case Episode::DownloadAvailable:
            ++downloads;
            break;
        case Episode::Missing:
            ++missing;
            break;
        case Episode::Ok:
        case Episode::Downloading:
        case Episode::Extracting:
        default:
        case UnkownStatus:
            break;
        }
    }

    switch(status()) {
    case DownloadsAvailable:
        return tr("%1 downloads available")
                .arg(downloads);
    case MissingEpisodes:
        return tr("%1 missing episodes")
                .arg(missing);
    case Complete:
        return tr("%1 episodes")
                .arg(episodes().count());
    default:
    case UnkownStatus:
        break;
    }

    return tr("");
}

QSet<QLocale::Language> Season::languages() const
{
    QSet<QLocale::Language> result;
    foreach(QSharedPointer<Episode> episode, episodes()) {
        foreach(QLocale::Language lang, episode->languages()) {
            result.insert(lang);
        }
    }
    return result;
}

QStringList Season::folders() const
{
    return m_folders;
}

void Season::setFolders(const QStringList &folders)
{
    m_folders = folders;
}

void Season::addFolder(const QString &folder)
{
    m_folders.append(folder);
}

void Season::addSerienjunkiesUrl(QSharedPointer<OnlineResource> serienjunkiesUrl)
{
    serienjunkiesUrl->setSeason(Qp::sharedFrom(this));
    m_serienjunkiesUrls.relate(serienjunkiesUrl);
}
