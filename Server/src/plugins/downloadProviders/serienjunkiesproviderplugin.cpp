#include "serienjunkiesproviderplugin.h"

#include "controller/controller.h"

#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"
#include "model/videodownloadlink.h"

#include <qserienjunkies.h>

#include <QDebug>

SerienjunkiesProviderPlugin::SerienjunkiesProviderPlugin(QObject *parent) :
    DownloadProviderPlugin("serienjunkies.org", parent),
    m_latestReply(nullptr)
{
    QSerienJunkies::setNetworkAccessManager(Controller::networkAccessManager());
}

void SerienjunkiesProviderPlugin::searchSeries(const QString &seriesTitle)
{
    if(m_latestReply) {
        delete m_latestReply;
        m_latestReply = nullptr;
    }

    QSerienJunkiesReply *reply = QSerienJunkies::searchSeries(seriesTitle);
    m_latestReply = reply;

    connect(reply, &QSerienJunkiesReply::finished, [=]() {
        QList<DownloadProviderPlugin::SeriesData> result;
        foreach(QSerienJunkiesReply::Series series, reply->series()) {
            DownloadProviderPlugin::SeriesData s;
            s.title = series.name;
            s.url = series.url;
            result.append(s);
        }

        reply->deleteLater();
        if(reply == m_latestReply)
            m_latestReply = nullptr;
        emit this->foundSeries(result);
    });

    connect(reply, &QSerienJunkiesReply::error, [=]() {
        setErrorString(reply->errorString());

        reply->deleteLater();
        if(reply == m_latestReply)
            m_latestReply = nullptr;
        emit this->error();
    });
}

bool SerienjunkiesProviderPlugin::canHandleSeries(QSharedPointer<Series> series) const
{
    return !series->serienJunkiesUrl().isEmpty()
            && series->serienJunkiesUrl().host().startsWith("serienjunkies");
}

void SerienjunkiesProviderPlugin::findMissingEpisodes(QSharedPointer<Series> series) const
{
    SerienjunkiesSearchHandler *handler = new SerienjunkiesSearchHandler(const_cast<SerienjunkiesProviderPlugin *>(this));
    handler->findMissingEpisodes(series);
}

void SerienjunkiesProviderPlugin::searchAndSetDownloadsForSeries(QSharedPointer<Series> series)
{
    connect(this,&SerienjunkiesProviderPlugin::foundSeries, this, &SerienjunkiesProviderPlugin::onSeriesFound);

    searchSeries(series->title());
}

void SerienjunkiesProviderPlugin::onSeriesFound(QList<DownloadProviderPlugin::SeriesData> series)
{
    //TODO: Find a better way to get the right serie (with not the same name)
    QSharedPointer<Series> serie = Series::forTitle(series.first().title);

    if(serie != 0 && serie->serienJunkiesUrl().toString() == ""){
       serie->setSerienJunkiesUrl(series.first().url);
       Qp::update(serie);
    }

    findMissingEpisodes(serie);
}


SerienjunkiesSearchHandler::SerienjunkiesSearchHandler(QObject *parent) :
    QObject(parent),
    m_series(nullptr),
    m_seasonsNotFinishedCount(0)
{
}

void SerienjunkiesSearchHandler::findMissingEpisodes(QSharedPointer<Series> series)
{
    m_series = series;

    QSerienJunkiesReply *reply = QSerienJunkies::searchSeasons(series->serienJunkiesUrl());

    connect(reply, &QSerienJunkiesReply::finished,
            this, &SerienjunkiesSearchHandler::searchSeasonsFinished);

    connect(reply, &QSerienJunkiesReply::error, [=]() {
        reply->deleteLater();
        this->deleteLater();
    });

}

void SerienjunkiesSearchHandler::searchSeasonsFinished()
{
    QSerienJunkiesReply *reply = static_cast<QSerienJunkiesReply *>(sender());

    foreach(QSerienJunkiesReply::Season se, reply->seasons()) {
        int number = QSerienJunkies::seasonNumberFromTitle(se.title);
        QSharedPointer<Season> season = m_series->season(number);

        if(!season) {
            season = Qp::create<Season>();
            season->setNumber(number);
            m_series->addSeason(season);
        }

        season->setSerienJunkiesTitle(se.title);
        season->setSerienJunkiesUrl(se.url);
        Qp::update(season);

        QSerienJunkiesReply *episodeReply = QSerienJunkies::searchDownloads(se.url);
        connect(episodeReply, &QSerienJunkiesReply::finished,
                this, &SerienjunkiesSearchHandler::searchEpisodesFinished);

        ++m_seasonsNotFinishedCount;

        connect(episodeReply, &QSerienJunkiesReply::error, [=]() {
            --m_seasonsNotFinishedCount;
            if(m_seasonsNotFinishedCount == 0)
                this->deleteLater();

            //episodeReply->deleteLater();
        });
    }

    reply->deleteLater();
}

void SerienjunkiesSearchHandler::searchEpisodesFinished()
{
    QSerienJunkiesReply *reply = static_cast<QSerienJunkiesReply *>(sender());

    foreach(QSerienJunkiesReply::Format format, reply->formats()) {
        foreach(QString mirror, format.mirrors) {
            foreach(QSerienJunkiesReply::DownloadLink link, reply->downloadLinks(format, mirror)) {
                int snumber = QSerienJunkies::seasonNumberFromName(link.name);
                int enumber = QSerienJunkies::episodeNumberFromName(link.name);

                QSharedPointer<Season> season = m_series->season(snumber);
                if(!season) {
                    continue;
                }

                QSharedPointer<Episode> episode = season->episode(enumber);

                if(!episode) {
                    episode = Qp::create<Episode>();
                    episode->setNumber(enumber);
                    season->addEpisode(episode);
                    Qp::update(episode);
                }
                //episode->setSerienJunkiesTitle(link.name);
                //QPersistence::update(episode);

                QSharedPointer<VideoDownloadLink>  vLink = Qp::create<VideoDownloadLink>();
                vLink->setName(link.name);
                vLink->setFormatDescription(format.description);
                vLink->setUrl(link.url);
                vLink->setMirror(mirror);
                episode->addDownloadLink(vLink);
                Qp::update(vLink);
                Qp::update(episode);
            }
        }
    }

    reply->deleteLater();

    --m_seasonsNotFinishedCount;
    if(m_seasonsNotFinishedCount == 0)
        this->deleteLater();
}
