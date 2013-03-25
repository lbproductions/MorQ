#include "thetvdbinformationprovider.h"

#include "controller/controller.h"
#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>

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

static const QString APIKEY("691EE4AC475E9ACB");

TheTvdbInformationProvider::TheTvdbInformationProvider(QObject *parent) :
    InformationProviderPlugin(parent)
{
}

void TheTvdbInformationProvider::searchSeries(const QString &title) const
{
    QUrl url(QString("http://thetvdb.com/api/GetSeries.php?seriesname=%1&language=all")
             .arg(title)); // TODO: use actual language, once its implemented

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProvider::parseSearchSeriesReply);
}

void TheTvdbInformationProvider::scrapeSeries(Series *series) const
{
    QUrl url(QString("http://thetvdb.com/api/%1/series/%2/%3.xml")
                      .arg(APIKEY)
                      .arg(series->tvdbId())
                      .arg("en")); // TODO: use actual language, once its implemented

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));
    reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_SERIES, QVariant::fromValue<Series *>(series));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProvider::parseScrapeSeriesReply);
}

void TheTvdbInformationProvider::scrapeSeriesIncludingEpisodes(Series *series) const
{
    QUrl url(QString("http://thetvdb.com/api/%1/series/%2/all/%3.xml")
                      .arg(APIKEY)
                      .arg(series->tvdbId())
                      .arg("en")); // TODO: use actual language, once its implemented

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));
    reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_SERIES, QVariant::fromValue<Series *>(series));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProvider::parseScrapeSeriesReply);
}

void TheTvdbInformationProvider::scrapeEpisode(Episode *episode) const
{
    Season *season = episode->season();
    Series *series = season->series();

    QUrl url(QString("http://thetvdb.com/api/%1/series/%2/default/%3/%4/%5.xml")
             .arg(APIKEY)
             .arg(series->tvdbId())
             .arg(season->number())
             .arg(episode->number())
             .arg("en")); // TODO: use actual language, once its implemented

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(url));
    reply->setProperty(QNETWORKREPLY_DYNAMICPROPERTY_EPISODE, QVariant::fromValue<Episode *>(episode));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProvider::parseScrapeEpisodeReply);
}

void TheTvdbInformationProvider::parseSearchSeriesReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

    QXmlStreamReader xml(reply);

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if(name == TOKENNAME_SERIES) {
                Series *series = QPersistence::create<Series>();
                TheTvdbInformationProvider::parseSeries(xml, series);
                InformationProviderPlugin::addResult(series);
            }
        }
    }

    emit finished();
}

void TheTvdbInformationProvider::parseScrapeSeriesReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    Series *series = reply->property(QNETWORKREPLY_DYNAMICPROPERTY_SERIES).value<Series *>();

    if(!series) {
        emit finished(); // TODO: emit error;
        return;
    }

    QXmlStreamReader xml(reply);

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if(name == TOKENNAME_SERIES) {
                TheTvdbInformationProvider::parseSeries(xml, series);
                QPersistence::update(series);
            }
            else if(name == TOKENNAME_EPISODE) {
                Episode *episode = QPersistence::create<Episode>();
                TheTvdbInformationProvider::parseEpisode(xml, episode);

                Season *season = series->season(episode->seasonNumber());

                if(!season) {
                    season = QPersistence::create<Season>();
                    season->setNumber(episode->seasonNumber());
                    series->addSeason(season);
                    QPersistence::insert(season);
                }

                Episode *originalEpisode = season->episode(episode->number());

                if(originalEpisode) {
                    copyEpisode(episode, originalEpisode);
                    QPersistence::update(originalEpisode);
                    delete episode;
                }
                else {
                    season->addEpisode(episode);
                    QPersistence::insert(episode);
                    InformationProviderPlugin::addNewEpisode(episode);
                }
            }
        }
    }

    emit finished();
}

void TheTvdbInformationProvider::parseScrapeEpisodeReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    Episode *episode = reply->property(QNETWORKREPLY_DYNAMICPROPERTY_EPISODE).value<Episode *>();

    if(!episode) {
        emit finished(); // TODO: emit error;
        return;
    }

    QXmlStreamReader xml(reply);

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if(name == TOKENNAME_EPISODE)
                TheTvdbInformationProvider::parseEpisode(xml, episode);
        }
    }

    emit finished();
}

void TheTvdbInformationProvider::copySeries(Series *source, Series *target) const
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

void TheTvdbInformationProvider::copyEpisode(Episode *source, Episode *target) const
{
    target->setTitle(source->title());

    if(target->seasonNumber() < 0) {
        target->setSeasonNumber(source->seasonNumber());
    }
    else if(source->seasonNumber() != target->seasonNumber()){
        qWarning() << Q_FUNC_INFO << "source->seasonNumber() != target->seasonNumber()";
    }

    if(target->number() < 0) {
        target->setNumber(target->number());
    }
    else if(source->number() != target->number()){
        qWarning() << Q_FUNC_INFO << "source->number() != target->number()";
    }

    target->setOverview(source->overview());
}

void TheTvdbInformationProvider::parseSeries(QXmlStreamReader &xml, Series *series)
{
    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();

        if(token == QXmlStreamReader::StartElement) {
            QString text = xml.readElementText();

            if(name == TOKENNAME_SERIES_SERIESID) {
                series->setTvdbId(text.toInt());
            }
            else if(name == TOKENNAME_SERIES_SERIESNAME) {
                series->setTitle(text);
            }
            else if(name == TOKENNAME_SERIES_BANNER) {
                series->addBannerUrl(text);
            }
            else if(name == TOKENNAME_SERIES_OVERVIEW) {
                series->setOverview(text);
            }
            else if(name == TOKENNAME_SERIES_FIRSTAIRED) {
                series->setFirstAired(QDate::fromString(text, Qt::ISODate));
            }
            else if(name == TOKENNAME_SERIES_ACTORS) {
                series->setActors(text.split("|"));
            }
            else if(name == TOKENNAME_SERIES_GENRE) {
                series->setGenres(text.split("|"));
            }

            // TODO: Add missing properties to series.
        }
        else if(token == QXmlStreamReader::EndElement) {
            break;
        }
    }
}

void TheTvdbInformationProvider::parseEpisode(QXmlStreamReader &xml, Episode *episode)
{
    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();

        if(token == QXmlStreamReader::StartElement) {
            QString text = xml.readElementText();

            if(name == TOKENNAME_EPISODE_EPISODENAME) {
                episode->setTitle(text);
            }
            else if(name == TOKENNAME_EPISODE_EPISODENUMBER) {
                bool ok = false;
                int number = text.toInt(&ok);
                if(ok) {
                    if(episode->number() < 0) {
                        episode->setNumber(number);
                    }
                    else if(episode->number() != number){
                        qWarning() << Q_FUNC_INFO << "episode->number() != number";
                    }
                }
            }
            else if(name == TOKENNAME_EPISODE_SEASONNUMBER) {
                bool ok = false;
                int number = text.toInt(&ok);
                if(ok) {
                    if(episode->seasonNumber() < 0) {
                        episode->setSeasonNumber(number);
                    }
                    else if(episode->seasonNumber() != number){
                        qWarning() << Q_FUNC_INFO << "episode->seasonNumber() != number";
                    }
                }
            }
            else if(name == TOKENNAME_EPISODE_OVERVIEW) {
              episode->setOverview(text);
            }

            // TODO: Add missing properties to episode.
        }
        else if(token == QXmlStreamReader::EndElement) {
            break;
        }
    }
}
