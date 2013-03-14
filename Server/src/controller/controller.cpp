#include "controller.h"

#include "downloadcontroller.h"
#include "plugincontroller.h"
#include "linkscontroller.h"
#include "extractioncontroller.h"
#include "seriescontroller.h"

#include "model/download.h"
#include "model/downloadpackage.h"
#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"
#include "model/videodownloadlink.h"

#include <QPersistenceDatabaseSchema.h>
#include <QPersistencePersistentDataAccessObject.h>

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QNetworkAccessManager>

DownloadsDAO *Controller::s_downloadsDao = nullptr;
DownloadPackagesDAO *Controller::s_downloadPackagesDao = nullptr;
SeriesDAO *Controller::s_seriesDao = nullptr;
SeasonsDAO *Controller::s_seasonDao = nullptr;
EpisodesDAO *Controller::s_episodesDao = nullptr;
VideoDownloadLinksDAO *Controller::s_videoDownloadLinksDao = nullptr;

static QObject GUARD;

bool Controller::initialize()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("test.sqlite");
    if(!db.open()) {
        qCritical() << db.lastError();
        return false;
    }

    // Download
    s_downloadsDao = new DownloadsDAO(new QPersistencePersistentDataAccessObject<Download>(), &GUARD);
    QPersistence::registerDataAccessObject<Download>(s_downloadsDao);


    // DownloadPackage
    s_downloadPackagesDao = new DownloadPackagesDAO(new QPersistencePersistentDataAccessObject<DownloadPackage>(), &GUARD);
    QPersistence::registerDataAccessObject<DownloadPackage>(s_downloadPackagesDao);


    // Series
    s_seriesDao = new SeriesDAO(new QPersistencePersistentDataAccessObject<Series>(), &GUARD);
    QPersistence::registerDataAccessObject<Series>(s_seriesDao);


    // Season
    s_seasonDao = new SeasonsDAO(new QPersistencePersistentDataAccessObject<Season>(), &GUARD);
    QPersistence::registerDataAccessObject<Season>(s_seasonDao);


    // Episode
    s_episodesDao = new EpisodesDAO(new QPersistencePersistentDataAccessObject<Episode>(), &GUARD);
    QPersistence::registerDataAccessObject<Episode>(s_episodesDao);


    // VideoDownloadLink
    s_videoDownloadLinksDao = new VideoDownloadLinksDAO(new QPersistencePersistentDataAccessObject<VideoDownloadLink>(), &GUARD);
    QPersistence::registerDataAccessObject<VideoDownloadLink>(s_videoDownloadLinksDao);


    // Adjust database
    QPersistenceDatabaseSchema databaseSchema;
    //    databaseSchema.createCleanSchema();
    databaseSchema.adjustSchema();


    // Call these methods, to once initialize each controller
    plugins();
    links();
    downloads();
    extractor();
    series();

    return true;
}

DownloadController *Controller::downloads()
{
    static DownloadController *controller = nullptr;

    if(!controller)
        controller = new DownloadController(&GUARD);

    return controller;
}

DownloadsDAO *Controller::downloadsDao()
{
    return s_downloadsDao;
}

DownloadPackagesDAO *Controller::downloadPackagesDao()
{
    return s_downloadPackagesDao;
}

SeriesDAO *Controller::seriesDao()
{
    return s_seriesDao;
}

SeasonsDAO *Controller::seasonsDao()
{
    return s_seasonDao;
}

EpisodesDAO *Controller::episodesDao()
{
    return s_episodesDao;
}

VideoDownloadLinksDAO *Controller::videoDownloadLinksDao()
{
    return s_videoDownloadLinksDao;
}

PluginController *Controller::plugins()
{
    static PluginController *controller = nullptr;

    if(!controller)
        controller = new PluginController(&GUARD);

    return controller;
}

LinksController *Controller::links()
{
    static LinksController *controller = nullptr;

    if(!controller)
        controller = new LinksController(&GUARD);

    return controller;
}

ExtractionController *Controller::extractor()
{
    static ExtractionController *controller = nullptr;

    if(!controller)
        controller = new ExtractionController(&GUARD);

    return controller;
}

SeriesController *Controller::series()
{
    static SeriesController *controller = nullptr;

    if(!controller)
        controller = new SeriesController(&GUARD);

    return controller;
}

QNetworkAccessManager *Controller::networkAccessManager()
{
    static QNetworkAccessManager *nam = nullptr;

    if(!nam)
        nam = new QNetworkAccessManager;

    static QObject guard;
    QObject::connect(&guard, &QObject::destroyed, [&]() {
        nam->deleteLater();
    });

    return nam;
}


