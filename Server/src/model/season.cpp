#include "season.h"

#include "episode.h"
#include "series.h"

#include <QDebug>
#include <QPixmap>

Season::Season(QObject *parent) :
    QObject(parent),
    m_number(0),
    m_series("series", this),
    m_episodes("episodes", this)
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

QString Season::serienJunkiesTitle() const
{
    return m_serienJunkiesTitle;
}

void Season::setSerienJunkiesTitle(const QString &title)
{
    m_serienJunkiesTitle = title;
}

QUrl Season::serienJunkiesUrl() const
{
    return m_serienJunkiesUrl;
}

void Season::setSerienJunkiesUrl(const QUrl &serienJunkiesUrl)
{
    m_serienJunkiesUrl = serienJunkiesUrl;
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
