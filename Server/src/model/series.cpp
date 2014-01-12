#include "series.h"

#include "season.h"
#include "preferences.h"
#include "misc/tools.h"
#include "episode.h"

#include <QDebug>
#include <QPixmapCache>

QSharedPointer<Series> Series::forTitle(const QString &title)
{
    foreach (QSharedPointer<Series> series, Qp::readAll<Series>()) {
        if (series->title() == title)
            return series;
    }

    return QSharedPointer<Series>();
}

Series::Series(QObject *parent) :
    QObject(parent),
    m_tvdbId(-1),
    m_seasons("seasons", this),
    m_checkState(Qt::Unchecked)
{
    m_languages.append(Qp::castList<int>(Preferences::languages()));
}

Series::~Series()
{
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
    if(tvdbId() <= 0)
        return QUrl();

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

QList<QSharedPointer<Season> > Series::seasons() const
{
    return m_seasons.resolveList();
}

QSharedPointer<Season> Series::season(int number) const
{
    return seasonsByNumber().value(number);
}

void Series::removeSeason(QSharedPointer<Season> season)
{
    season->setSeries(QSharedPointer<Series>());
    m_seasonsByNumber.remove(season->number());
    m_seasons.unrelate(season);
}

void Series::addSeason(QSharedPointer<Season> season)
{
    season->setSeries(Qp::sharedFrom(this));
    m_seasonsByNumber.insert(season->number(), season);
    m_seasons.relate(season);
}

QList<QSharedPointer<Episode> > Series::episodes() const
{
    QList<QSharedPointer<Episode> > result;
    foreach (QSharedPointer<Season> season, seasons()) {
        result.append(season->episodes());
    }
    return result;
}

bool sortByDate(const QSharedPointer<Episode> &e1, const QSharedPointer<Episode> &e2)
{
    return e1->firstAired() < e2->firstAired();
}

QSharedPointer<Episode> Series::latestEpisode() const
{
    QList<QSharedPointer<Episode> > es = episodes();
    if(es.isEmpty())
        return QSharedPointer<Episode>();

    qSort(es.begin(), es.end(), sortByDate);
    return es.last();
}

QList<QLocale::Language> Series::languages() const
{
    QList<QLocale::Language> result;
    result.append(Qp::castList<QLocale::Language>(m_languages));

    foreach (QSharedPointer<Season> season, seasons()) {
        foreach (QLocale::Language lang, season->languages()) {
            if (!result.contains(lang))
                result.append(lang);
        }
    }

    return result;
}

void Series::addLanguage(QLocale::Language language)
{
    if (!m_languages.contains(language))
        m_languages.append(static_cast<int>(language));
}

void Series::addLanguages(QList<QLocale::Language> languages)
{
    foreach (auto l, languages) {
        addLanguage(l);
    }
}

QPixmap Series::languageFlag(QLocale::Language language)
{
    QString flag = QString(":/flags/%1.png").arg(Series::tvdbLanguage(language));
    return Tools::cachedPixmap(flag);
}

QString Series::tvdbLanguage(QLocale::Language language)
{
    if (language == QLocale::AnyLanguage)
        return "?";

    QString lang = QLocale(language).name();
    return lang.left(lang.lastIndexOf('_'));
}

void Series::setSeasons(const QList<QSharedPointer<Season> > &seasons)
{
    m_seasonsByNumber.clear();
    m_seasons.clear();
    m_seasons.relate(seasons);
}

void Series::_setLanguages(const QList<int> &languages)
{
    m_languages = languages;
}

QList<int> Series::_languages() const
{
    return m_languages;
}

QMap<int, QSharedPointer<Season> > Series::seasonsByNumber() const
{
    if (m_seasonsByNumber.isEmpty()) {
        foreach (QSharedPointer<Season> season, seasons()) {
            m_seasonsByNumber.insert(season->number(), season);
        }
    }
    return m_seasonsByNumber;
}

Series::Status Series::status() const
{
    int downloads = 0;
    int missing = 0;
    int okay = 0;

    foreach (QSharedPointer<Season> s, seasons()) {
        switch (s->status()) {
        case Season::DownloadsAvailable:
            ++downloads;
            break;
        case Season::MissingEpisodes:
            ++missing;
            break;
        case Season::Complete:
            ++okay;
        default:
        case Season::UnkownStatus:
            break;
        }
    }

    if (downloads > 0)
        return Series::DownloadsAvailable;

    if (missing > 0)
        return Series::MissingEpisodes;

    if (okay == seasons().size())
        return Series::Complete;

    return Series::UnkownStatus;
}

QPixmap Series::statusPixmap() const
{
    switch (status()) {
    case DownloadsAvailable:
        return Tools::cachedPixmap(":/icons/download_available");
    case MissingEpisodes:
        return Tools::cachedPixmap(":/icons/missing");
    case Complete:
        return Tools::cachedPixmap(":/icons/okay");
    case UnkownStatus:
    default:
        break;
    }

    return Tools::cachedPixmap(":/icons/questionmark");
}

QString Series::statusMessage() const
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
        default:
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
        return tr("Complete");
    default:
    case UnkownStatus:
        break;
    }

    return tr("");
}

Qt::CheckState Series::checkState() const
{
    return m_checkState;
}

void Series::setCheckState(const Qt::CheckState &checkState)
{
    if (checkState == m_checkState)
        return;

    Qt::CheckState oldState = m_checkState;
    m_checkState = checkState;
    emit checkStateChanged(oldState, m_checkState);
}

QStringList Series::folders() const
{
    return m_folders;
}

void Series::setFolders(const QStringList &folders)
{
    m_folders = folders;
}

void Series::addFolder(const QString &folder)
{
    m_folders.append(folder);
}
