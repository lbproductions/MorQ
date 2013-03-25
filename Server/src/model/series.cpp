#include "series.h"

#include "season.h"

#include <QDebug>
#include <QPixmapCache>

Series::Series(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_primaryLanguage(QLocale::AnyLanguage),
    m_tvdbId(-1),
    m_checkState(Qt::Unchecked)
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

QList<Season *> Series::seasons(QLocale::Language language) const
{
    if(language == QLocale::AnyLanguage)
        return m_seasons.values();

    QList<Season *> result;
    foreach(Season *season, m_seasons) {
        if(season->primaryLanguage() == language) {
            result.append(season);
        }
    }
    return result;
}

Season *Series::season(int number, QLocale::Language language) const
{
    QList<Season*> seasons = m_seasons.values(number);

    if(seasons.isEmpty())
        return nullptr;

    QLocale::Language lang = language;
    if(lang == QLocale::AnyLanguage)
        lang = m_primaryLanguage;

    foreach(Season *season, seasons) {
        if(season->primaryLanguage() == lang) {
            return season;
        }
    }

    if(lang == QLocale::AnyLanguage)
        return seasons.first();

    return nullptr;
}

void Series::removeSeason(Season *season)
{
    Q_ASSERT(m_seasons.contains(season->number()));

    season->setSeries(nullptr);
    m_seasons.remove(season->number());
}

void Series::addSeason(Season *season)
{
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

QLocale::Language Series::primaryLanguage() const
{
    return m_primaryLanguage;
}

void Series::setPrimaryLanguage(QLocale::Language language)
{
    m_primaryLanguage = language;
}

QSet<QLocale::Language> Series::languages() const
{
    QSet<QLocale::Language> result;
    result.insert(m_primaryLanguage);
    foreach(int lang, m_additionalLanguages) {
        result.insert(static_cast<QLocale::Language>(lang));
    }
    foreach(Season *season, m_seasons) {
        foreach(QLocale::Language lang, season->languages()) {
            result.insert(lang);
        }
    }

    return result;
}

void Series::addLanguage(QLocale::Language language)
{
    m_additionalLanguages.insert(language);
}

QPixmap Series::primaryLanguageFlag() const
{
    return Series::languageFlag(m_primaryLanguage);
}

QPixmap Series::languageFlag(QLocale::Language language)
{
    QString flag = QString(":/flags/%1.png").arg(Series::tvdbLanguage(language));
    QPixmap pm;
    if(!QPixmapCache::find(flag, pm)) {
        pm = QPixmap(flag);
        if(pm.isNull()) {
            qWarning() << QString("Could not find flag '%1'")
                          .arg(flag);
        }

        QPixmapCache::insert(flag, pm);
    }

    return pm;
}

QString Series::tvdbLanguage(QLocale::Language language)
{
    if(language == QLocale::AnyLanguage)
        return "en";

    QString lang = QLocale(language).name();
    return lang.left(lang.lastIndexOf('_'));
}

void Series::setSeasons(const QList<Season *> &seasons)
{
    m_seasons.clear();

    foreach(Season *season, seasons){
        addSeason(season);
    }
}

void Series::setAdditionalLanguages(const QSet<int> &languages)
{
    m_additionalLanguages = languages;
}

QSet<int> Series::additionalLanguages() const
{
    return m_additionalLanguages;
}

Qt::CheckState Series::checkState() const
{
    return m_checkState;
}

void Series::setCheckState(const Qt::CheckState &checkState)
{
    if(checkState == m_checkState)
        return;

    Qt::CheckState oldState = m_checkState;
    m_checkState = checkState;
    emit checkStateChanged(oldState, m_checkState);
}
