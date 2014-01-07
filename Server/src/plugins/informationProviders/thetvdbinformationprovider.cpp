#include "thetvdbinformationprovider.h"

#include "controller/controller.h"
#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"
#include "preferences.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include <QtConcurrent>

static const QString TOKENNAME_SERIES("Series");
static const QString TOKENNAME_SERIES_SERIESID("seriesid");
static const QString TOKENNAME_SERIES_LANGUAGE("language");
static const QString TOKENNAME_SERIES_SERIESNAME("SeriesName");
static const QString TOKENNAME_SERIES_BANNER("banner");
static const QString TOKENNAME_SERIES_OVERVIEW("Overview");
static const QString TOKENNAME_SERIES_FIRSTAIRED("FirstAired");
static const QString TOKENNAME_SERIES_ID("id");

static const QString TOKENNAME_SERIES_ACTORS("Actors");
static const QString TOKENNAME_SERIES_AIRS_DAYOFWEEK("Airs_DayOfWeek");
static const QString TOKENNAME_SERIES_AIRS_TIME("Airs_Time");
static const QString TOKENNAME_SERIES_GENRE("Genre");
static const QString TOKENNAME_SERIES_IMDB_ID("IMDB_ID");
static const QString TOKENNAME_SERIES_NETWORK("Network");
static const QString TOKENNAME_SERIES_RATING("Rating");
static const QString TOKENNAME_SERIES_RUNTIME("Runtime");
static const QString TOKENNAME_SERIES_STATUS("Status");
static const QString TOKENNAME_SERIES_LASTUPDATED("lastupdated");

static const QString TOKENNAME_EPISODE("Episode");
static const QString TOKENNAME_EPISODE_ID("id");
static const QString TOKENNAME_EPISODE_DVD_CHAPTER("DVD_chapter");
static const QString TOKENNAME_EPISODE_DVD_DISCID("DVD_discid");
static const QString TOKENNAME_EPISODE_DVD_EPISODENUMBER("DVD_episodenumber");
static const QString TOKENNAME_EPISODE_DVD_SEASON("DVD_season");
static const QString TOKENNAME_EPISODE_DIRECTOR("Director");
static const QString TOKENNAME_EPISODE_EPISODENAME("EpisodeName");
static const QString TOKENNAME_EPISODE_EPISODENUMBER("EpisodeNumber");
static const QString TOKENNAME_EPISODE_FIRSTAIRED("FirstAired");
static const QString TOKENNAME_EPISODE_GUESTSTARS("GuestStars");
static const QString TOKENNAME_EPISODE_IMDB_ID("IMDB_ID");
static const QString TOKENNAME_EPISODE_LANGUAGE("Language");
static const QString TOKENNAME_EPISODE_OVERVIEW("Overview");
static const QString TOKENNAME_EPISODE_PRODUCTIONCODE("ProductionCode");
static const QString TOKENNAME_EPISODE_RATING("Rating");
static const QString TOKENNAME_EPISODE_SEASONNUMBER("SeasonNumber");
static const QString TOKENNAME_EPISODE_WRITER("Writer");
static const QString TOKENNAME_EPISODE_ABSOLUTE_NUMBER("absolute_number");
static const QString TOKENNAME_EPISODE_AIRSAFTER_SEASON("airsafter_season");
static const QString TOKENNAME_EPISODE_AIRSBEFORE_EPISODE("airsbefore_episode");
static const QString TOKENNAME_EPISODE_AIRSBEFORE_SEASON("airsbefore_season");
static const QString TOKENNAME_EPISODE_FILENAME("filename");
static const QString TOKENNAME_EPISODE_LASTUPDATED("lastupdated");
static const QString TOKENNAME_EPISODE_SEASONID("seasonid");
static const QString TOKENNAME_EPISODE_SERIESID("seriesid");

static const QByteArray QNETWORKREPLY_DYNAMICPROPERTY_SERIES("THETVDBINFORMATIONSCRAPER_SERIES");
static const QByteArray QNETWORKREPLY_DYNAMICPROPERTY_EPISODE("THETVDBINFORMATIONSCRAPER_EPISODE");
static const QByteArray QNETWORKREPLY_DYNAMICPROPERTY_LANGUAGE("THETVDBINFORMATIONSCRAPER_LANGUAGE");

static const QString APIKEY("691EE4AC475E9ACB");

Q_DECLARE_METATYPE(QLocale::Language)

const QString TheTvdbInformationProvider::Name("thetvdb.com");

TheTvdbInformationProvider::TheTvdbInformationProvider(QObject *parent) :
    InformationProvider(TheTvdbInformationProvider::Name, parent)
{
}

InformationProviderTask *TheTvdbInformationProvider::searchSeries(const QString &title, QSharedPointer<Series> series)
{
    TheTvdbInformationProviderTask *task = new TheTvdbInformationProviderTask(this);
    task->setSeries(series); // have to set this from here, because we only pass the title into searchSeries()
    task->searchSeries(title);
    return task;
}

InformationProviderTask *TheTvdbInformationProvider::scrapeSeries(QSharedPointer<Series> series)
{
    TheTvdbInformationProviderTask *task = new TheTvdbInformationProviderTask(this);
    task->scrapeSeries(series);
    return task;
}

void TheTvdbInformationProvider::saveSeriesResult(QSharedPointer<Series> source, QSharedPointer<Series> target) const
{
    target->setTvdbId(source->tvdbId());
    target->setTitle(source->title());
    target->setBannerUrls(source->bannerUrls());
    target->setOverview(source->overview());
    target->setFirstAired(source->firstAired());
    target->setGenres(source->genres());
    target->setImdbId(source->imdbId());

    // TODO: Add missing properties to series.
}

void TheTvdbInformationProvider::saveEpisodeResult(QSharedPointer<Episode> source, QSharedPointer<Episode> target) const
{
    target->setTitle(source->title());

    if (target->seasonNumber() < 0) {
        target->setSeasonNumber(source->seasonNumber());
    }
    else if (source->seasonNumber() != target->seasonNumber()){
        qWarning() << Q_FUNC_INFO << "source->seasonNumber() != target->seasonNumber()";
    }

    if (target->number() < 0) {
        target->setNumber(source->number());
    }
    else if (source->number() != target->number()){
        qWarning() << Q_FUNC_INFO << "source->number() != target->number()";
    }

    target->setOverview(source->overview());
}

TheTvdbInformationProviderTask::TheTvdbInformationProviderTask(TheTvdbInformationProvider *parent) :
    InformationProviderTask(parent),
    m_activeRequestsCount(0)
{
}

static void (QNetworkReply:: *ERRORSIGNAL)(QNetworkReply::NetworkError) = &QNetworkReply::error;

void TheTvdbInformationProviderTask::searchSeries(const QString &title)
{
    foreach (QLocale::Language language, Preferences::languages()) {
        ++m_activeRequestsCount;
        QUrl url(QString("http://thetvdb.com/api/GetSeries.php?seriesname=%1&language=%2")
                 .arg(title)
                 .arg(Series::tvdbLanguage(language)));

        QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished,
                this, &TheTvdbInformationProviderTask::parseSearchSeriesReply);

        connect(reply, ERRORSIGNAL,
                this, &TheTvdbInformationProviderTask::handleNetworkError);
    }
}

void TheTvdbInformationProviderTask::parseSearchSeriesReply()
{
    QScopedPointer<QNetworkReply, QScopedPointerObjectDeleteLater<QNetworkReply> >
            reply(static_cast<QNetworkReply *>(sender()));

    QXmlStreamReader xml(reply.data());

    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if (name == TOKENNAME_SERIES) {
                QSharedPointer<Series> series = Qp::create<Series>();
                parseSeries(xml, series);

                if (m_tvdbIds.contains(series->tvdbId())) {
                    Qp::remove(series);
                }
                else {
                    m_tvdbIds.insert(series->tvdbId());
                    Qp::update(series);
                    addResultSeries(series);
                }
            }
        }
    }

    decreaseActiveRequestCountAndMaybeEmitFinished();
}

void TheTvdbInformationProviderTask::scrapeSeries(QSharedPointer<Series> series)
{
    setSeries(series);
    foreach (QLocale::Language language, series->languages()) {
        ++m_activeRequestsCount;

        QUrl url(QString("http://thetvdb.com/api/%1/series/%2/all/%3.xml")
                 .arg(APIKEY)
                 .arg(series->tvdbId())
                 .arg(Series::tvdbLanguage(language)));

        QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));
        reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_SERIES, QVariant::fromValue<QSharedPointer<Series> >(series));
        reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_LANGUAGE, QVariant::fromValue<int>(language));

        connect(reply, &QNetworkReply::finished,
                this, &TheTvdbInformationProviderTask::parseScrapeSeriesReply);

        connect(reply, ERRORSIGNAL,
                this, &TheTvdbInformationProviderTask::handleNetworkError);
    }
}

void TheTvdbInformationProviderTask::parseScrapeSeriesReply()
{
    QScopedPointer<QNetworkReply, QScopedPointerObjectDeleteLater<QNetworkReply> >
            reply(static_cast<QNetworkReply *>(sender()));
    QSharedPointer<Series> series = reply->property(QNETWORKREPLY_DYNAMICPROPERTY_SERIES).value<QSharedPointer<Series> >();
    QLocale::Language language = static_cast<QLocale::Language>(reply->property(QNETWORKREPLY_DYNAMICPROPERTY_LANGUAGE).toInt());

    if (!series) {
        decreaseActiveRequestCountAndMaybeEmitFinished();
        return;
    }

    QXmlStreamReader xml(reply.data());

    Qp::startBulkDatabaseQueries();
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if (name == TOKENNAME_SERIES
                    && language == series->languages().first()) {
                parseSeries(xml, series);
                Qp::update(series);
            }
            else if (name == TOKENNAME_EPISODE) {
                QSharedPointer<Episode> episode(new Episode());
                parseEpisode(xml, episode);

                QSharedPointer<Season> season = series->season(episode->seasonNumber());

                if (!season) {
                    season = Qp::create<Season>();
                    season->setNumber(episode->seasonNumber());

                    if (season->number() == 0) {
                        season->setTitle("Specials (TheTVDB)");
                    }

                    series->addSeason(season);
                    Qp::update(season);
                }

                QSharedPointer<Episode> originalEpisode = season->episode(episode->number());

                if (originalEpisode) {
                    static_cast<TheTvdbInformationProvider *>(provider())->saveEpisodeResult(episode, originalEpisode);
                    Qp::update(originalEpisode);
                }
                else {
                    originalEpisode = Qp::create<Episode>();
                    originalEpisode->setStatus(Episode::Missing);
                    static_cast<TheTvdbInformationProvider *>(provider())->saveEpisodeResult(episode, originalEpisode);
                    season->addEpisode(originalEpisode);
                    Qp::update(originalEpisode);
                }
            }
        }
    }

    Qp::commitBulkDatabaseQueries();
    decreaseActiveRequestCountAndMaybeEmitFinished();
}

void TheTvdbInformationProviderTask::handleNetworkError()
{
    QScopedPointer<QNetworkReply, QScopedPointerObjectDeleteLater<QNetworkReply> >
            reply(static_cast<QNetworkReply *>(sender()));
    setErrorMessage(reply->errorString());
    decreaseActiveRequestCountAndMaybeEmitFinished();
}

void TheTvdbInformationProviderTask::scrapeEpisode(QSharedPointer<Episode> episode)
{
    QSharedPointer<Season> season = episode->season();
    QSharedPointer<Series> series = season->series();

    ++m_activeRequestsCount;
    QUrl url(QString("http://thetvdb.com/api/%1/series/%2/default/%3/%4/%5.xml")
             .arg(APIKEY)
             .arg(series->tvdbId())
             .arg(season->number())
             .arg(episode->number())
             .arg(episode->tvdbLanguage()));

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));
    reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_EPISODE, QVariant::fromValue<QSharedPointer<Episode> >(episode));
    reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_LANGUAGE, QVariant::fromValue<int>(episode->primaryLanguage()));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProviderTask::parseScrapeEpisodeReply);

    connect(reply, ERRORSIGNAL ,
            this, &TheTvdbInformationProviderTask::handleNetworkError);
}

void TheTvdbInformationProviderTask::parseScrapeEpisodeReply()
{
    QScopedPointer<QNetworkReply, QScopedPointerObjectDeleteLater<QNetworkReply> >
            reply(static_cast<QNetworkReply *>(sender()));
    QSharedPointer<Episode> episode = reply->property(QNETWORKREPLY_DYNAMICPROPERTY_EPISODE).value<QSharedPointer<Episode> >();

    if (!episode) {
        decreaseActiveRequestCountAndMaybeEmitFinished();
        return;
    }

    QXmlStreamReader xml(reply.data());

    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if (name == TOKENNAME_EPISODE)
                parseEpisode(xml, episode);
        }
    }

    decreaseActiveRequestCountAndMaybeEmitFinished();
}

void TheTvdbInformationProviderTask::decreaseActiveRequestCountAndMaybeEmitFinished()
{
    --m_activeRequestsCount;
    if (m_activeRequestsCount == 0) {
        emit finished();
    }
}

void TheTvdbInformationProviderTask::parseSeries(QXmlStreamReader &xml, QSharedPointer<Series> series)
{
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();

        if (token == QXmlStreamReader::StartElement) {
            QString text = xml.readElementText();

            if (name == TOKENNAME_SERIES_SERIESID) {
                series->setTvdbId(text.toInt());
            }
            else if (name == TOKENNAME_SERIES_SERIESNAME) {
                series->setTitle(text);
            }
            else if (name == TOKENNAME_SERIES_BANNER) {
                series->addBannerUrl(text);
            }
            else if (name == TOKENNAME_SERIES_OVERVIEW) {
                series->setOverview(text);
            }
            else if (name == TOKENNAME_SERIES_FIRSTAIRED) {
                series->setFirstAired(QDate::fromString(text, Qt::ISODate));
            }
            else if (name == TOKENNAME_SERIES_ACTORS) {
                series->setActors(text.split("|"));
            }
            else if (name == TOKENNAME_SERIES_GENRE) {
                series->setGenres(text.split("|"));
            }

            // TODO: Add missing properties to series.
        }
        else if (token == QXmlStreamReader::EndElement) {
            break;
        }
    }
}

void TheTvdbInformationProviderTask::parseEpisode(QXmlStreamReader &xml, QSharedPointer<Episode> episode)
{
    while (!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();

        if (token == QXmlStreamReader::StartElement) {
            QString text = xml.readElementText();

            if (name == TOKENNAME_EPISODE_EPISODENAME) {
                episode->setTitle(text);
            }
            else if (name == TOKENNAME_EPISODE_EPISODENUMBER) {
                bool ok = false;
                int number = text.toInt(&ok);
                if (ok) {
                    if (episode->number() < 0) {
                        episode->setNumber(number);
                    }
                    else if (episode->number() != number){
                        qWarning() << Q_FUNC_INFO << "episode->number() != number";
                    }
                }
            }
            else if (name == TOKENNAME_EPISODE_SEASONNUMBER) {
                bool ok = false;
                int number = text.toInt(&ok);
                if (ok) {
                    if (episode->seasonNumber() < 0) {
                        episode->setSeasonNumber(number);
                    }
                    else if (episode->seasonNumber() != number){
                        qWarning() << Q_FUNC_INFO << "episode->seasonNumber() != number";
                    }
                }
            }
            else if (name == TOKENNAME_EPISODE_OVERVIEW) {
                episode->setOverview(text);
            }
            else if (name == TOKENNAME_EPISODE_LANGUAGE) {
                episode->setPrimaryLanguage(QLocale(text).language());
            }

            // TODO: Add missing properties to episode.
        }
        else if (token == QXmlStreamReader::EndElement) {
            break;
        }
    }
}
