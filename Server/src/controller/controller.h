#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QPersistenceCachedDataAccessObject.h>

#include "model/seriesdataaccessobject.h"

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
class VideoDownloadLink;

typedef QpCachedDataAccessObject<Download> DownloadsDAO;
typedef QpCachedDataAccessObject<DownloadPackage> DownloadPackagesDAO;
typedef SeriesDataAccessObject SeriesDAO;
typedef QpCachedDataAccessObject<Season> SeasonsDAO;
typedef QpCachedDataAccessObject<Episode> EpisodesDAO;
typedef QpCachedDataAccessObject<VideoDownloadLink> VideoDownloadLinksDAO;

class Controller
{
public:
    static bool initialize();

    static DownloadsDAO *downloadsDao();
    static DownloadPackagesDAO *downloadPackagesDao();
    static SeriesDAO *seriesDao();

    static DownloadController *downloads();
    static PluginController *plugins();
    static LinksController *links();
    static ExtractionController *extractor();

    static QNetworkAccessManager *networkAccessManager();

private:
    static SeasonsDAO *seasonsDao();
    static EpisodesDAO *episodesDao();
    static VideoDownloadLinksDAO *videoDownloadLinksDao();

private:
    static DownloadsDAO *s_downloadsDao;
    static DownloadPackagesDAO *s_downloadPackagesDao;
    static SeriesDAO *s_seriesDao;
    static SeasonsDAO *s_seasonDao;
    static EpisodesDAO *s_episodesDao;
    static VideoDownloadLinksDAO *s_videoDownloadLinksDao;
};

#endif // CONTROLLER_H
