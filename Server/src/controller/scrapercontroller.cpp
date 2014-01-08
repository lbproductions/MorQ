#include "scrapercontroller.h"

#include "plugins/scraper/filescraper.h"
#include "plugins/informationProviders/thetvdbinformationprovider.h"
#include "plugins/downloadProviders/serienjunkiesprovider.h"
#include "preferences.h"
#include "misc/tools.h"
#include "model/episode.h"

ScraperController::ScraperController(QObject *parent) :
    QObject(parent)
{
}

void ScraperController::scrapeLocal()
{
    Qp::startBulkDatabaseQueries();
    FileScraper *scraper = new FileScraper(this);
    connect(scraper, &FileScraper::finished,
            this, &ScraperController::onFileScraperFinish);
    scraper->scan();
}

void ScraperController::onFileScraperFinish()
{
    FileScraper *scraper = static_cast<FileScraper *>(sender());
    scraper->deleteLater();

    Qp::commitBulkDatabaseQueries();

    emit finishedLocalScrape();
}


void ScraperController::scrapeMissingTvdbInformation()
{
    QList<QSharedPointer<Series> > series = Qp::readAll<Series>();

    InformationProvider *informationProvider = Plugins::informationProvider(TheTvdbInformationProvider::Name);

    connect(informationProvider, &InformationProvider::finishedAllTasks,
            this, &ScraperController::finishedTvdbScrape);

    bool hasNewSeries = false;
    foreach (QSharedPointer<Series> s , series) {
        if (s->tvdbId() < 0) {
            hasNewSeries = true;

            InformationProviderTask *task = informationProvider->searchSeries(s->title(), s);

            connect(task, &InformationProviderTask::finished,
                    this, &ScraperController::interpretTvdbSearchResults);
            connect(task, &DownloadProviderTask::error,
                    task, &QObject::deleteLater); // TODO: handle InformationTask::error
        }
    }

    if (!hasNewSeries)
        emit finishedTvdbScrape();
}

void ScraperController::interpretTvdbSearchResults()
{
    QScopedPointer<InformationProviderTask, QScopedPointerObjectDeleteLater<InformationProviderTask> >
            task(static_cast<InformationProviderTask *>(sender()));

    QSharedPointer<Series> s = task->series();
    QSharedPointer<Series> result = inferBestMatchingSeries(s, task->resultSeries());

    if (result) {
        task->provider()->saveSeriesResult(result, s);

        foreach (QSharedPointer<Series> r, task->resultSeries()) {
            Qp::remove(r);
        }

        InformationProviderTask *nextTask = task->provider()->scrapeSeries(s);
        connect(nextTask, &InformationProviderTask::finished,
                nextTask, &QObject::deleteLater);
        connect(nextTask, &DownloadProviderTask::error,
                nextTask, &QObject::deleteLater); // TODO: handle InformationTask::error
    }
    else {
        // TODO: Allow manual searching or setting of tvdbids
        qDebug() << "No TVDB entry found";
        foreach (QSharedPointer<Series> r, task->resultSeries()) {
            Qp::remove(r);
        }
    }
}

QSharedPointer<Series> ScraperController::inferBestMatchingSeries(QSharedPointer<Series> series,
                                                          QList<QSharedPointer<Series> > results)
{
    QList<QSharedPointer<Series> > bestResults;
    int bestDistance = std::numeric_limits<int>::max();

    foreach (QLocale::Language language, series->languages()) {
        foreach (QSharedPointer<Series> result, results) {
            if (!result->languages().contains(language))
                continue;

            int distance = Tools::levenshteinDistance(result->title(), series->title());

            if (distance == bestDistance) {
                bestResults.append(result);
            }
            else if (distance < bestDistance) {
                bestResults.clear();
                bestResults.append(result);
                bestDistance = distance;
            }
        }

        if (!bestResults.isEmpty())
            break;
    }

    if (bestResults.isEmpty())
        return QSharedPointer<Series>();

    if (bestResults.size() > 1)
        qDebug() << "Ambiguous TVDB entries: " << bestResults;

    if (bestDistance > 0) {
        qDebug() << "No matching TVDB entry: " << series->title();
        return QSharedPointer<Series>();
    }

    return bestResults.first();
}

void ScraperController::scrapeSerienjunkiesUrls()
{
    DownloadProvider *downloadProvider = Plugins::downloadProvider(SerienjunkiesProvider::Name);

    connect(downloadProvider, &DownloadProvider::finishedAllTasks,
            this, &ScraperController::finishedSerienjunkiesScrape);

    bool hasScrapedEpisode = false;
    foreach (QSharedPointer<Episode> e, Qp::readAll<Episode>()) {
        if (!e->videoFile().isEmpty())
            continue;

        if (!e->downloadLinks().isEmpty())
            continue;

        hasScrapedEpisode = true;

        DownloadProviderTask *task = downloadProvider->findDownloadLinks(e);

        connect(task, &DownloadProviderTask::finished,
                task, &QObject::deleteLater);
        connect(task, &DownloadProviderTask::error, [=]{
            qDebug() << task->errorMessage();
        });
        connect(task, &DownloadProviderTask::error,
                task, &QObject::deleteLater);
    }

    if (!hasScrapedEpisode)
        emit finishedSerienjunkiesScrape();
}
