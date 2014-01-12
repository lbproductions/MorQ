#include "season.h"

#include "episode.h"
#include "series.h"
#include "onlineresource.h"
#include "misc/tools.h"

#include <QDebug>
#include <QPixmap>

Season::Season(QObject *parent) :
    QObject(parent),
    m_number(-1),
    m_series("series", this),
    m_episodes("episodes", this),
    m_serienjunkiesUrls("serienjunkiesUrls", this),
    m_tvdbId(-1)
{
}

Season::~Season()
{
}

int Season::number() const
{
    return m_number;
}

void Season::setNumber(int number)
{
    Q_ASSERT(m_number >= -1);
    if(number != m_number && Qp::sharedFrom(this)) {
        m_number = number;
        if(series()) {
            series()->removeSeason(Qp::sharedFrom(this));
            series()->addSeason(Qp::sharedFrom(this));
        }
    }

    m_number = number;
}

QString Season::title() const
{
    if (!m_title.isEmpty())
        return m_title;

    if (m_number < 0)
        return tr("Unkown season");

    if (m_number == 0)
        return tr("Specials");

    return tr("Season %1").arg(number());
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

QList<QSharedPointer<Episode> > Season::specialEpisodes() const
{
    episode(-1);
    return m_episodesByNumber.values(-1);
}

QSharedPointer<Episode> Season::episode(int number) const
{
    if (m_episodesByNumber.isEmpty()) {
        foreach (QSharedPointer<Episode> episode, episodes()) {
            m_episodesByNumber.insert(episode->number(), episode);
        }
    }

    return m_episodesByNumber.value(number);
}

QSharedPointer<Episode> Season::episode(const QString &title) const
{
    foreach(QSharedPointer<Episode> e, episodes()) {
        if(e->title() == title)
            return e;
    }
    return QSharedPointer<Episode>();
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

int Season::tvdbId() const
{
    return m_tvdbId;
}

void Season::setTvdbId(int arg)
{
    m_tvdbId = arg;
}

QUrl Season::tvdbUrl() const
{
    if(tvdbId() <= 0)
        return QUrl();

    return QUrl(QString("http://thetvdb.com/?tab=season&seriesid=%1&seasonid=%2&lid=7")
                .arg(series()->tvdbId())
                .arg(tvdbId()));
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

    foreach (QSharedPointer<Episode> e, episodes()) {
        switch (e->status()) {
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
        case Episode::UnkownStatus:
            break;
        }
    }

    if (downloads > 0)
        return Season::DownloadsAvailable;

    if (missing > 0)
        return Season::MissingEpisodes;

    if (okay == episodes().count())
        return Season::Complete;

    return Season::UnkownStatus;
}

QPixmap Season::statusPixmap() const
{
    switch (status()) {
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

    foreach (QSharedPointer<Episode> e, episodes()) {
        switch (e->status()) {
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

    switch (status()) {
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
    foreach (QSharedPointer<Episode> episode, episodes()) {
        foreach (QLocale::Language lang, episode->languages()) {
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
