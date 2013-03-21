#include "thetvdbinformationprovider.h"

#include "controller/controller.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>

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

static const QString APIKEY("691EE4AC475E9ACB");

TheTvdbInformationProvider::TheTvdbInformationProvider(QObject *parent) :
    InformationProviderPlugin(parent)
{
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
    // TODO: Add language to series
    // TODO: Add air day of week to series
    // TODO: Add air time to series
    // TODO: Add network to series
    // TODO: Add rating to series
    // TODO: Add runtime to series
    // TODO: Add status to series
    // TODO: Add last updated to series
}

void TheTvdbInformationProvider::searchSeries(const QString &title) const
{
    QUrl urlGetSeries(QString("http://thetvdb.com/api/GetSeries.php?seriesname=%1&language=all").arg(title));

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(urlGetSeries));

    connect(reply, &QNetworkReply::finished,
            this, &TheTvdbInformationProvider::parseGetSeriesReply);
}

void TheTvdbInformationProvider::parseGetSeriesReply()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

    QXmlStreamReader xml(reply);

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement) {
            QStringRef name = xml.name();
            if(name == TOKENNAME_SERIES)
                parseSeries(xml);
        }
    }

    emit finished();
}

void TheTvdbInformationProvider::parseSeries(QXmlStreamReader &xml)
{
    Series *series = QPersistence::create<Series>();

    while(!xml.atEnd()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();

        if(token == QXmlStreamReader::StartElement) {
            QString text = xml.readElementText();

            if(name == TOKENNAME_SERIES_SERIESID) {
                series->setTvdbId(text.toInt());
            }
            else if(name == TOKENNAME_SERIES_LANGUAGE) {
                // TODO: Add language to series
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
            else if(name == TOKENNAME_SERIES_ID) {
                // TODO: Same as SERIESID??
            }
            else if(name == TOKENNAME_SERIES_ACTORS) {
                series->setActors(text.split("|"));
            }
            else if(name == TOKENNAME_SERIES_AIRS_DAYOFWEEK) {
                // TODO: Add air day of week to series
            }
            else if(name == TOKENNAME_SERIES_AIRS_TIME) {
                // TODO: Add air time to series
            }
            else if(name == TOKENNAME_SERIES_GENRE) {
                series->setGenres(text.split("|"));
            }
            else if(name == TOKENNAME_SERIES_IMDB_ID) {
                series->setImdbId(text);
            }
            else if(name == TOKENNAME_SERIES_NETWORK) {
                // TODO: Add network to series
            }
            else if(name == TOKENNAME_SERIES_RATING) {
                // TODO: Add rating to series
            }
            else if(name == TOKENNAME_SERIES_RUNTIME) {
                // TODO: Add runtime to series
            }
            else if(name == TOKENNAME_SERIES_STATUS) {
                // TODO: Add status to series
            }
            else if(name == TOKENNAME_SERIES_LASTUPDATED) {
                // TODO: Add last updated to series
            }
        }
        else if(token == QXmlStreamReader::EndElement) {
            break;
        }
    }

    InformationProviderPlugin::addResult(series);
}

void TheTvdbInformationProvider::scrapeSeries(Series *series) const
{

}
