#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QPersistence.h>

class DownloadController;
class PluginController;
class QNetworkAccessManager;
class Download;
class DownloadPackage;
class LinksController;
class ExtractionController;
class Series;
class Season;
class Episode;
class OnlineResource;

class Controller
{
public:
    static bool initialize(bool cleanDatabase);

    static DownloadController *downloads();
    static PluginController *plugins();
    static LinksController *links();
    static ExtractionController *extractor();

    static QNetworkAccessManager *networkAccessManager();
};

#endif // CONTROLLER_H
