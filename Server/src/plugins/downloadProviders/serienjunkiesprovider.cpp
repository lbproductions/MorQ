#include "serienjunkiesprovider.h"

#include "controller/controller.h"

#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"
#include "model/onlineresource.h"
#include "controller/scrapercontroller.h"

#include <qserienjunkies.h>

#include <QDebug>

const QString SerienjunkiesProvider::Name("serienjunkies.org");

SerienjunkiesProvider::SerienjunkiesProvider(QObject *parent) :
    DownloadProvider(SerienjunkiesProvider::Name, parent)
{
    QSerienJunkies::setNetworkAccessManager(Controller::networkAccessManager());
}

DownloadProviderTask *SerienjunkiesProvider::findDownloadLinks(QSharedPointer<Episode> episode)
{
    SerienjunkiesProviderTask *task = new SerienjunkiesProviderTask(this);
    task->findDownloadLinks(episode);
    return task;
}

QHash<QSharedPointer<Series>, SerienjunkiesProviderTask *> SerienjunkiesProviderTask::s_seriesSearchTasks;
QHash<QSharedPointer<Series>, SerienjunkiesProviderTask *> SerienjunkiesProviderTask::s_seasonSearchTasks;
QHash<QSharedPointer<Season>, SerienjunkiesProviderTask *> SerienjunkiesProviderTask::s_linkSearchTasks;

SerienjunkiesProviderTask::SerienjunkiesProviderTask(SerienjunkiesProviderTask *parent) :
    DownloadProviderTask(parent->provider()),
    m_requestCount(0)
{
}

SerienjunkiesProviderTask::SerienjunkiesProviderTask(SerienjunkiesProvider *parent) :
    DownloadProviderTask(parent),
    m_requestCount(0)
{
}

SerienjunkiesProviderTask::~SerienjunkiesProviderTask()
{
}

QSharedPointer<Series> SerienjunkiesProviderTask::series() const
{
    return m_series;
}

void SerienjunkiesProviderTask::setSeries(const QSharedPointer<Series> &series)
{
    m_series = series;
}

QSharedPointer<Season> SerienjunkiesProviderTask::season() const
{
    return m_season;
}

void SerienjunkiesProviderTask::setSeason(const QSharedPointer<Season> &season)
{
    setSeries(season->series());
    m_season = season;
}

QList<QSharedPointer<OnlineResource> > SerienjunkiesProviderTask::takeUrlsForEpisode(QSharedPointer<Episode> episode) const
{
    return m_urlsByEpisode.values(episode);
}

void SerienjunkiesProviderTask::findSeriesUrl(QSharedPointer<Series> series)
{
    setSeries(series);
    QSerienJunkiesReply *reply = QSerienJunkies::searchSeries(series->title());
    connect(reply, &QSerienJunkiesReply::finished,
            this, &SerienjunkiesProviderTask::seriesSearchFinished);
    connect(reply, &QSerienJunkiesReply::error,
            this, &SerienjunkiesProviderTask::handleSerienjunkiesError);
}

void SerienjunkiesProviderTask::seriesSearchFinished()
{
    QScopedPointer<QSerienJunkiesReply, QScopedPointerObjectDeleteLater<QSerienJunkiesReply> >
            reply(static_cast<QSerienJunkiesReply *>(sender()));

    QList<QSharedPointer<Series> > results;
    foreach (QSerienJunkiesReply::Series s, reply->series()) {
        QSharedPointer<Series> result = QSharedPointer<Series>(new Series(this));
        result->setTitle(s.name);
        result->setSerienJunkiesUrl(s.url);
        results.append(result);
    }

    QSharedPointer<Series> result = ScraperController::inferBestMatchingSeries(series(), results);

    if (!result) {
        setErrorMessage(QString("No Seriesjunkies entry: %1")
                        .arg(series()->title()));
        return;
    }

    series()->setSerienJunkiesUrl(result->serienJunkiesUrl());
    Qp::update(series());

    s_seriesSearchTasks.remove(series());

    emit finished();
}

void SerienjunkiesProviderTask::findSeasonUrls(QSharedPointer<Series> series)
{
    setSeries(series);

    // Needs the series url
    if (!series->serienJunkiesUrl().isValid()) {
        SerienjunkiesProviderTask *task = nullptr;

        if (!s_seriesSearchTasks.contains(series)) {
            task = new SerienjunkiesProviderTask(this);
            s_seriesSearchTasks.insert(series, task);
            task->findSeriesUrl(series);
        }
        else {
            task = s_seriesSearchTasks.value(series);
        }

        connect(task, &SerienjunkiesProviderTask::error,
                this, &SerienjunkiesProviderTask::setErrorMessage);
        connect(task, &SerienjunkiesProviderTask::finished,
                this, &SerienjunkiesProviderTask::_findSeasonUrls);

        return;
    }

    _findSeasonUrls();
}

void SerienjunkiesProviderTask::_findSeasonUrls()
{
    QSerienJunkiesReply *reply = QSerienJunkies::searchSeasons(series()->serienJunkiesUrl());
    connect(reply, &QSerienJunkiesReply::finished,
            this, &SerienjunkiesProviderTask::seasonSearchFinished);
    connect(reply, &QSerienJunkiesReply::error,
            this, &SerienjunkiesProviderTask::handleSerienjunkiesError);
}

void SerienjunkiesProviderTask::seasonSearchFinished()
{
    QScopedPointer<QSerienJunkiesReply, QScopedPointerObjectDeleteLater<QSerienJunkiesReply> >
            reply(static_cast<QSerienJunkiesReply *>(sender()));

    foreach (QSerienJunkiesReply::Season s, reply->seasons()) {
        int number = QSerienJunkies::seasonNumberFromTitle(s.title);
        if (s.title.contains("COMPLETE")) {
            number = 1;
        }

        QSharedPointer<Season> season = series()->season(number);

        if (!season) {
            season = Qp::create<Season>();
            season->setNumber(number);
            series()->addSeason(season);
            Qp::update(season);

            // Since this is a new season, we have to start its own task
            SerienjunkiesProviderTask *task = new SerienjunkiesProviderTask(this);
            s_linkSearchTasks.insert(season, task);
            task->findAllDownloadLinks(season);

            connect(task, &SerienjunkiesProviderTask::error,
                    this, &SerienjunkiesProviderTask::setErrorMessage);
        }

        QSharedPointer<OnlineResource> serienjunkiesUrl = Qp::create<OnlineResource>();
        serienjunkiesUrl->setUrl(s.url);
        serienjunkiesUrl->setName(s.title);
        season->addSerienjunkiesUrl(serienjunkiesUrl);
        Qp::update(serienjunkiesUrl);
    }

    s_seasonSearchTasks.remove(series());

    emit finished();
}

void SerienjunkiesProviderTask::findAllDownloadLinks(QSharedPointer<Season> season)
{
    setSeason(season);

    // Needs the season's URL
    if (season->serienjunkiesUrls().isEmpty()) {
        SerienjunkiesProviderTask *task = nullptr;

        if (!s_seasonSearchTasks.contains(season->series())) {
            task = new SerienjunkiesProviderTask(this);
            s_seasonSearchTasks.insert(season->series(), task);
            task->findSeasonUrls(season->series());
        }
        else {
            task = s_seasonSearchTasks.value(season->series());
        }

        connect(task, &SerienjunkiesProviderTask::error,
                this, &SerienjunkiesProviderTask::setErrorMessage);
        connect(task, &SerienjunkiesProviderTask::finished,
                this, &SerienjunkiesProviderTask::_findAllDownloadLinks);

        return;
    }

    _findAllDownloadLinks();
}

void SerienjunkiesProviderTask::_findAllDownloadLinks()
{
    if (season()->serienjunkiesUrls().isEmpty()) {
        s_linkSearchTasks.remove(season());
        emit error(tr("No serienjunkies URL for show '%1' season %2")
                   .arg(season()->series()->title())
                   .arg(season()->number()));
        return;
    }

    foreach (QSharedPointer<OnlineResource> url, season()->serienjunkiesUrls()) {
        ++m_requestCount;
        QSerienJunkiesReply *reply = QSerienJunkies::searchDownloads(url->url());
        connect(reply, &QSerienJunkiesReply::finished,
                this, &SerienjunkiesProviderTask::downloadLinkSearchFinished);
        connect(reply, &QSerienJunkiesReply::error,
                this, &SerienjunkiesProviderTask::handleSerienjunkiesError);
    }
}

void SerienjunkiesProviderTask::downloadLinkSearchFinished()
{
    QScopedPointer<QSerienJunkiesReply, QScopedPointerObjectDeleteLater<QSerienJunkiesReply> >
            reply(static_cast<QSerienJunkiesReply *>(sender()));

    foreach (QSerienJunkiesReply::Format f, reply->formats()) {
        foreach (QString mirror, f.mirrors) {
            foreach (QSerienJunkiesReply::DownloadLink link, reply->downloadLinks(f, mirror)) {
                int episodeNumber = QSerienJunkies::episodeNumberFromName(link.name);
                int seasonNumber = QSerienJunkies::seasonNumberFromName(link.name);

                // The seasonnumber from the links's name might be different from the one that has been guessed from the title
                if(seasonNumber != season()->number()) {
                    // If we previously didnt know our number change it now
                    if(season()->number() == -1) {
                        int possiblyYear = QString("%1%2")
                                        .arg(seasonNumber,2,10,QChar('0'))
                                        .arg(episodeNumber,2,10,QChar('0')).toInt();

                        if(possiblyYear > 1950 && possiblyYear < QDate::currentDate().year() + 1) {
                            episodeNumber = -1;
                            seasonNumber = -1;
                        }
                        else {
                            season()->setNumber(seasonNumber);
                            Qp::update(season());
                        }
                    }
                }

                QSharedPointer<Episode> episode = season()->episode(episodeNumber);

                if (!episode || episodeNumber < 0) {
                    episode = season()->episode(link.name);
                    if(!episode) {
                        episode = Qp::create<Episode>();
                        episode->setTitle(link.name);
                        episode->setNumber(episodeNumber);
                        season()->addEpisode(episode);
                        Qp::update(episode);

                        // Since this is a new episode, we have to start its own task
                        SerienjunkiesProviderTask *task = new SerienjunkiesProviderTask(this);
                        task->findDownloadLinks(episode);

                        connect(task, &SerienjunkiesProviderTask::error,
                                this, &SerienjunkiesProviderTask::setErrorMessage);
                    }
                }

                QSharedPointer<OnlineResource> url = QSharedPointer<OnlineResource>(new OnlineResource(this));
                url->setUrl(link.url);
                url->setName(link.name);
                url->setFormatDescription(f.description);
                url->setMirror(mirror);
                m_urlsByEpisode.insert(episode, url);
            }
        }
    }

    --m_requestCount;
    if (m_requestCount == 0) {
        s_linkSearchTasks.remove(season());
        emit finished();
    }
}

void SerienjunkiesProviderTask::findDownloadLinks(QSharedPointer<Episode> episode)
{
    setEpisode(episode);
    setSeason(episode->season());

    // Searching for all links of the whole season in one request is cheaper
    SerienjunkiesProviderTask *task = nullptr;

    if (!s_linkSearchTasks.contains(season())) {
        task = new SerienjunkiesProviderTask(this);
        s_linkSearchTasks.insert(season(), task);
        task->findAllDownloadLinks(season());
    }
    else {
        task = s_linkSearchTasks.value(season());
    }

    connect(task, &SerienjunkiesProviderTask::error,
            this, &SerienjunkiesProviderTask::setErrorMessage);
    connect(task, &SerienjunkiesProviderTask::finished,
            this, &SerienjunkiesProviderTask::_findDownloadLinks);
}

void SerienjunkiesProviderTask::_findDownloadLinks()
{
    QScopedPointer<SerienjunkiesProviderTask, QScopedPointerObjectDeleteLater<SerienjunkiesProviderTask> >
            task(static_cast<SerienjunkiesProviderTask *>(sender()));

    QList<QSharedPointer<OnlineResource> > urls = task->takeUrlsForEpisode(episode());

    if (urls.isEmpty()) {
        emit error(tr("No serienjunkies URL for show '%1' season %2 episode %3")
                   .arg(episode()->season()->series()->title())
                   .arg(episode()->season()->number())
                   .arg(episode()->number()));
        return;
    }

    QSharedPointer<OnlineResource> bestMirror;

    // TODO: Get preferred mirrors from preferences
    QStringList preferredMirrors = QStringList()
            << "uploaded.to";

    foreach(QString mirror, preferredMirrors) {
        foreach (QSharedPointer<OnlineResource> url, urls) {
            if (url->mirror().contains(mirror)) {
                bestMirror = url;
                break;
            }
        }

        if(bestMirror)
            break;
    }

    // No URL with one of the preferred mirrors
    if(!bestMirror)
        bestMirror = urls.first();

    QSharedPointer<OnlineResource> newurl = Qp::create<OnlineResource>();
    newurl->setUrl(bestMirror->url());
    newurl->setName(bestMirror->name());
    newurl->setFormatDescription(bestMirror->formatDescription());
    newurl->setMirror(bestMirror->mirror());
    episode()->addDownloadLink(newurl);
    episode()->setStatus(Episode::DownloadAvailable);
    Qp::update(episode());
    Qp::update(newurl);

    emit finished();
}

void SerienjunkiesProviderTask::handleSerienjunkiesError()
{
    QScopedPointer<QSerienJunkiesReply, QScopedPointerObjectDeleteLater<QSerienJunkiesReply> >
            reply(static_cast<QSerienJunkiesReply *>(sender()));
    setErrorMessage(reply->errorString());

    if(s_linkSearchTasks.value(season()) == this)
        s_linkSearchTasks.remove(season());
    if(s_seasonSearchTasks.value(series()) == this)
        s_seasonSearchTasks.remove(series());
    if(s_seriesSearchTasks.value(series()) == this)
        s_seasonSearchTasks.remove(series());
}
