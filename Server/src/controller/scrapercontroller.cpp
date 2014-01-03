#include "scrapercontroller.h"

#include "plugins/scraper/filescraper.h"
#include "plugins/informationProviders/thetvdbinformationprovider.h"
#include "plugins/downloadProviders/serienjunkiesproviderplugin.h"
#include "preferences.h"
#include "misc/tools.h"
#include "model/episode.h"

ScraperController::ScraperController(QObject *parent) :
    QObject(parent)
{
}

void ScraperController::scanSeriesLocationsForNewSeries()
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

    Qp::commitBulkDatabaseQueries();

    if(!scraper->newSeries().isEmpty())
        emit foundNewSeries();
}


void ScraperController::scrapeMissingTvdbInformation()
{
    QList<QSharedPointer<Series> > series = Qp::readAll<Series>();

    foreach(QSharedPointer<Series> s , series) {
        if(s->tvdbId() < 0) {
            InformationProvider *informationProvider = Plugins::informationProvider(TheTvdbInformationProvider::Name);


            InformationProviderTask *task = informationProvider->searchSeries(s->title(), s);

            connect(task, &InformationProviderTask::finished,
                    this, &ScraperController::interpretTvdbSearchResults);
        }
    }
}

void ScraperController::interpretTvdbSearchResults()
{
    InformationProviderTask *task = static_cast<InformationProviderTask *>(sender());

    QSharedPointer<Series> s = task->series();
    QSharedPointer<Series> result = findCorrectResult(s, task->resultSeries());

    if(result) {
        task->provider()->saveSeriesResult(result, s);

        foreach(QSharedPointer<Series> r, task->resultSeries()) {
            Qp::remove(r);
        }

        InformationProviderTask *nextTask = task->provider()->scrapeSeries(s);

        connect(nextTask, &InformationProviderTask::finished,
                nextTask, &QObject::deleteLater);
    }
    else {
        // TODO: Allow manual searching or setting of tvdbids
        qDebug() << "No TVDB entry found";
        foreach(QSharedPointer<Series> r, task->resultSeries()) {
            Qp::remove(r);
        }
    }

    task->deleteLater();
}

QSharedPointer<Series> ScraperController::findCorrectResult(QSharedPointer<Series> series,
                                                            QList<QSharedPointer<Series> > results)
{
    QList<QSharedPointer<Series> > bestResults;
    int bestDistance = std::numeric_limits<int>::max();

    foreach(QLocale::Language language, series->languages()) {
        foreach(QSharedPointer<Series> result, results) {
            if(!result->languages().contains(language))
                continue;

            int distance = Tools::levenshteinDistance(result->title(), series->title());

            if(distance == bestDistance) {
                bestResults.append(result);
            }
            else if(distance < bestDistance) {
                bestResults.clear();
                bestResults.append(result);
                bestDistance = distance;
            }
        }

        if(!bestResults.isEmpty())
            break;
    }

    if(bestResults.isEmpty())
        return QSharedPointer<Series>();

    // TODO: Allow manual searching or setting of tvdbids
    if(bestResults.size() > 1)
        qDebug() << "Ambiguous TVDB entries: " << bestResults;

    if(bestDistance > 0) {

    }

    return bestResults.first();
}

void ScraperController::scrapeSerienjunkiesUrls()
{

    QList<QSharedPointer<Series> > series = Qp::readAll<Series>();

    foreach(QSharedPointer<Series> s , series) {
        if(!s->serienJunkiesUrl().isValid()) {
            SerienjunkiesProviderPlugin *provider = new SerienjunkiesProviderPlugin(this);

            connect(provider, &SerienjunkiesProviderPlugin::foundSeries,
                    this, &ScraperController::interpretSerienjunkiesSearchResults);

            provider->searchSeries(s->title());
        }
    }
}


void ScraperController::interpretSerienjunkiesSearchResults(QList<DownloadProviderPlugin::SeriesData> series)
{

}
