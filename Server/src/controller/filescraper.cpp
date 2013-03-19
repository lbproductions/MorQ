#include "filescraper.h"

#include "controller.h"

#include "preferences.h"
#include "model/seriesdataaccessobject.h"
#include "model/season.h"
#include "model/episode.h"

#include <QRegularExpression>
#include <QStringList>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include <qserienjunkies.h>

static QList<QRegularExpression> SERIESTITLE_REGEXPS()
{
    static const QList<QRegularExpression> list =
            QList<QRegularExpression>() << QRegularExpression("^(.*) \\(\\d+\\)/.*") // <location>/Title (Year)/[...]
                                        << QRegularExpression("^\\/([^\\/]*).*$"); // <location>/Title/[...]
    return list;
}

static QStringList VIDEOEXTENSIONS()
{
    static const QStringList extensions =
            QStringList() << "m4v" << "3gp" << "nsv" << "ts" << "ty" << "strm" << "rm" << "rmvb" << "m3u" << "ifo" << "mov" << "qt" << "divx" << "xvid" << "bivx" << "vob" << "nrg" << "img"
                          << "iso" << "pva" << "wmv" << "asf" << "asx" << "ogm" << "m2v" << "avi" << "bin" << "dat" << "dvr-ms" << "mpg" << "mpeg" << "mp4" << "mkv" << "avc" << "vp3"
                          << "svq3" << "nuv" << "viv" << "dv" << "fli" << "flv" << "rar" << "001" << "wpl" << "zip";
    return extensions;
}

static QString fileExtension(const QString &name)
{
    int index = name.lastIndexOf('.');
    if(index < 0)
        return QString();

    return name.right(name.length() - index - 1);
}

FileScraper::FileScraper(QObject *parent) :
    QObject(parent),
    m_folderCount(-1),
    m_locationCount(-1),
    m_currentFolder(-1),
    m_currentLocation(-1)
{
    connect(this, &FileScraper::result,
            this, &FileScraper::consumeResult);

    qRegisterMetaType<FileScraperPrivate::Result>();
}

int FileScraper::folderCount() const
{
    return m_folderCount;
}

int FileScraper::currentFolder() const
{
    return m_currentFolder;
}

int FileScraper::locationCount() const
{
    return m_locationCount;
}

int FileScraper::currentLocation() const
{
    return m_currentLocation;
}

void FileScraper::scanAllLocationsForShowsSeasonsAndEpisodesAsync()
{
    QtConcurrent::run(this, &FileScraper::scanAllLocationsForShowsSeasonsAndEpisodes);
}

void FileScraper::scanAllLocationsForShowsSeasonsAndEpisodes()
{
    QStringList locations = Preferences::seriesLocations();
    m_locationCount = locations.size();
    m_currentLocation = 0;
    foreach(QString location, locations) {
        ++m_currentLocation;
        scanLocationForShowsSeasonsAndEpisodes(location);
    }

    emit finished();
}

void FileScraper::scanLocationForShowsSeasonsAndEpisodes(const QString &location)
{
    QDir dir(location);
    m_folderCount = dir.count();
    QDirIterator it(location, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString path = it.next();
        path.remove(0, location.length());
        QString extension = fileExtension(path);
        if(!VIDEOEXTENSIONS().contains(extension))
            continue;

        FileScraperPrivate::Result r;
        r.seasonNumber = QSerienJunkies::seasonNumberFromName(path);
        r.episodeNumber = QSerienJunkies::episodeNumberFromName(path);
        r.seriesTitle = seriesTitleFromPath(path);
        r.absolutePath = location + path;
        emit result(r);
    }


//    QStringList entryList = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
//    m_folderCount = entryList.size();

//    m_currentFolder = 0;
//    foreach(QString folder, entryList) {
//        ++m_currentFolder;
//        emit enteredFolder(folder);
//        emit foundSeriesFolder(folder);
//    }
}

QString FileScraper::seriesTitleFromPath(const QString &path)
{
    foreach(QRegularExpression regExp, SERIESTITLE_REGEXPS()) {
        QRegularExpressionMatch match = regExp.match(path);
        if(match.hasMatch()) {
            QStringList matches = match.capturedTexts();
            if(matches.size() > 1)
                return matches.at(1);
            return matches.first();
        }
    }

    return path;
}

void FileScraper::consumeResult(const FileScraperPrivate::Result &result)
{
    Series *series = Controller::seriesDao()->byTitle(result.seriesTitle);

    if(!series) {
        series = QPersistence::create<Series>();
        series->setTitle(result.seriesTitle);
        QPersistence::insert(series);
    }

    Season *season = series->season(result.seasonNumber);

    if(!season) {
        season = QPersistence::create<Season>();
        season->setNumber(result.seasonNumber);
        series->addSeason(season);
        QPersistence::insert(season);
    }

    Episode *episode = season->episode(result.episodeNumber);

    if(!episode) {
        episode = QPersistence::create<Episode>();
        episode->setNumber(result.episodeNumber);
        season->addEpisode(episode);
        episode->addVideoFile(result.absolutePath);
        QPersistence::insert(episode);
    }
    else {
        episode->addVideoFile(result.absolutePath);
        QPersistence::update(episode);
    }
}
