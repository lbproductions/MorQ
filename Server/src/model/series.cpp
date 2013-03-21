#include "series.h"

#include "season.h"

#include <QDebug>

Series::Series(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_tvdbId(-1)
{
}

Series::~Series()
{
    qDebug() << "~Series(" << m_id << ")=" << this;

    foreach(Season *season, m_seasons.values()) {
        season->setSeries(nullptr);
    }
}

int Series::id() const
{
    return m_id;
}

void Series::setId(int id)
{
    m_id = id;
}

QString Series::title() const
{
    return m_title;
}

void Series::setTitle(const QString &title)
{
    m_title = title;
}

QUrl Series::serienJunkiesUrl() const
{
    return m_serienJunkiesUrl;
}

void Series::setSerienJunkiesUrl(const QUrl &url)
{
    m_serienJunkiesUrl = url;
}

int Series::tvdbId() const
{
    return m_tvdbId;
}

void Series::setTvdbId(int id)
{
    m_tvdbId = id;
}

QUrl Series::tvdbUrl() const
{
    return QUrl(QString("http://thetvdb.com/?tab=series&id=%1").arg(tvdbId()));
}

QString Series::imdbId() const
{
    return m_imdbId;
}

void Series::setImdbId(const QString &id)
{
    m_imdbId = id;
}

QUrl Series::imdbUrl() const
{
    return QUrl(QString("http://www.imdb.com/title/%1").arg(imdbId()));
}

QString Series::overview() const
{
    return m_overview;
}

void Series::setOverview(const QString &overview)
{
    m_overview = overview;
}

QDate Series::firstAired() const
{
    return m_firstAired;
}

void Series::setFirstAired(const QDate &date)
{
    m_firstAired = date;
}

QStringList Series::genres() const
{
    return m_genres;
}

void Series::setGenres(const QStringList &genres)
{
    m_genres = genres;
}

QStringList Series::actors() const
{
    return m_actors;
}

void Series::setActors(const QStringList &actors)
{
    m_actors = actors;
}

QStringList Series::bannerUrls() const
{
    return m_bannerUrls;
}

void Series::setBannerUrls(const QStringList &urls)
{
    m_bannerUrls = urls;
}

void Series::addBannerUrl(const QString &banner)
{
    m_bannerUrls.append(banner);
}

QStringList Series::posterUrls() const
{
    return m_posterUrls;
}

void Series::setPosterUrls(const QStringList &urls)
{
    m_posterUrls = urls;
}

QList<Season *> Series::seasons() const
{
    return m_seasons.values();
}

Season *Series::season(int number) const
{
    return m_seasons.value(number);
}

void Series::removeSeason(Season *season)
{
    Q_ASSERT(m_seasons.contains(season->number()));

    season->setSeries(nullptr);
    m_seasons.remove(season->number());
}

void Series::addSeason(Season *season)
{
    Q_ASSERT(!m_seasons.contains(season->number()));

    season->setSeries(this);
    m_seasons.insert(season->number(), season);
}

QList<Episode *> Series::episodes() const
{
    QList<Episode *> result;
    foreach(Season *season, seasons()) {
        result.append(season->episodes());
    }
    return result;
}

void Series::setSeasons(const QList<Season *> &seasons)
{
    m_seasons.clear();

    foreach(Season *season, seasons){
        addSeason(season);
    }
}
