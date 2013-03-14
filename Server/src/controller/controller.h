#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QPersistenceCachedDataAccessObject.h>

class DownloadController;
class PluginController;
class QNetworkAccessManager;
class Download;
class DownloadPackage;
class LinksController;
class ExtractionController;
class SeriesController;
class Series;
class Season;
class Episode;
class VideoDownloadLink;

typedef QPersistenceCachedDataAccessObject<Download> DownloadsDAO;
typedef QPersistenceCachedDataAccessObject<DownloadPackage> DownloadPackagesDAO;
typedef QPersistenceCachedDataAccessObject<Series> SeriesDAO;
typedef QPersistenceCachedDataAccessObject<Season> SeasonsDAO;
typedef QPersistenceCachedDataAccessObject<Episode> EpisodesDAO;
typedef QPersistenceCachedDataAccessObject<VideoDownloadLink> VideoDownloadLinksDAO;

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
    static SeriesController *series();

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
