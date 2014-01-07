#include "controller.h"

#include "downloadcontroller.h"
#include "plugincontroller.h"
#include "linkscontroller.h"
#include "extractioncontroller.h"
#include "plugins/plugins.h"

#include "model/download.h"
#include "model/downloadpackage.h"
#include "model/series.h"
#include "model/season.h"
#include "model/episode.h"
#include "model/onlineresource.h"

#include <QPersistenceDatabaseSchema.h>
#include <QPersistencePersistentDataAccessObject.h>

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QNetworkAccessManager>

bool Controller::initialize()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/niklaswulf/.morq/sqlitedb");
    if(!db.open()) {
        qCritical() << db.lastError();
        return false;
    }
    Qp::setDatabase(db);

    Qp::registerSetType<int>();
    Qp::registerClass<Download>();
    Qp::registerClass<DownloadPackage>();
    Qp::registerClass<Series>();
    Qp::registerClass<Season>();
    Qp::registerClass<Episode>();
    Qp::registerClass<OnlineResource>();

    // Adjust database
    QpDatabaseSchema databaseSchema;
    databaseSchema.createCleanSchema();
//    databaseSchema.adjustSchema();


    // Call these methods, to once initialize each controller
    plugins();
    links();
    downloads();
    extractor();

    Plugins::init();

    return true;
}

DownloadController *Controller::downloads()
{
    static DownloadController *controller = nullptr;
    static QObject GUARD;

    if(!controller)
        controller = new DownloadController(&GUARD);

    return controller;
}
PluginController *Controller::plugins()
{
    static QObject GUARD;
    static PluginController *controller = nullptr;

    if(!controller)
        controller = new PluginController(&GUARD);

    return controller;
}

LinksController *Controller::links()
{
    static QObject GUARD;
    static LinksController *controller = nullptr;

    if(!controller)
        controller = new LinksController(&GUARD);

    return controller;
}

ExtractionController *Controller::extractor()
{
    static QObject GUARD;
    static ExtractionController *controller = nullptr;

    if(!controller)
        controller = new ExtractionController(&GUARD);

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


