#include "filescraper.h"

#include <controller/controller.h>

#include "preferences.h"
#include "model/season.h"
#include "model/episode.h"
#include "model/series.h"

#include <QRegularExpression>
#include <QStringList>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include <qserienjunkies.h>

static QList<QRegularExpression> SERIESTITLE_REGEXPS()
{
    static const QList<QRegularExpression> list =
            QList<QRegularExpression>() << QRegularExpression("^\\/(.*) \\(\\d+\\)\\/.*") // <location>/Title (Year)/[...]
                                        << QRegularExpression("^\\/([^\\/]*).*$"); // <location>/Title/[...]
    return list;
}

static QList<QPair<QLocale::Language, QRegularExpression> > LANGUAGE_REGEXPS()
{
    QList<QPair<QLocale::Language, QRegularExpression> > result;
    result.append(QPair<QLocale::Language, QRegularExpression>(QLocale::German, QRegularExpression("/Staffel \\d+/")));
    result.append(QPair<QLocale::Language, QRegularExpression>(QLocale::English, QRegularExpression(".*\\/Season \\d+\\/.*")));
    return result;
}

QStringList FileScraper::VIDEOEXTENSIONS()
{
    static const QStringList extensions =
            QStringList() << "m4v" << "3gp" << "nsv" << "ts" << "ty"
                          << "strm" << "rm" << "rmvb" << "m3u" << "ifo"
                          << "mov" << "qt" << "divx" << "xvid" << "bivx"
                          << "vob" << "nrg" << "img" << "iso" << "pva"
                          << "wmv" << "asf" << "asx" << "ogm" << "m2v"
                          << "avi" << "bin" << "dat" << "dvr-ms" << "mpg"
                          << "mpeg" << "mp4" << "mkv" << "avc" << "vp3"
                          << "svq3" << "nuv" << "viv" << "dv" << "fli"
                          << "flv" << "rar" << "001" << "wpl" << "zip";
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
    Scraper(parent),
    m_locationCount(-1),
    m_currentLocationCount(-1)
{
    connect(this, &FileScraper::result,
            this, &FileScraper::consumeResult);

    qRegisterMetaType<FileScraperPrivate::Result>();
}

int FileScraper::locationCount() const
{
    return m_locationCount;
}

int FileScraper::currentLocationCount() const
{
    return m_currentLocationCount;
}

QString FileScraper::currentLocation() const
{
    return m_currentLocation;
}

void FileScraper::scan()
{
    scanAllLocationsForShowsSeasonsAndEpisodesAsync();
}

void FileScraper::scanAllLocationsForShowsSeasonsAndEpisodesAsync()
{
    QtConcurrent::run(this, &FileScraper::scanAllLocationsForShowsSeasonsAndEpisodes);
}

void FileScraper::scanAllLocationsForShowsSeasonsAndEpisodes()
{
    QStringList locations = Preferences::seriesLocations();
    m_locationCount = locations.size();
    m_currentLocationCount = 0;
    foreach(QString location, locations) {
        ++m_currentLocationCount;
        m_currentLocation = location;
        scanLocationForShowsSeasonsAndEpisodes(location);
    }

    emit finished();
}

void FileScraper::scanLocationForShowsSeasonsAndEpisodes(const QString &location)
{
    QDirIterator it(location, QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QString path = it.next();
        path.remove(0, location.length());
        QStringList folders = path.split("/");
        emit scrapingFile(path);


        QString extension = fileExtension(path);
        if(!VIDEOEXTENSIONS().contains(extension))
            continue;

        FileScraperPrivate::Result r;
        r.seasonNumber = QSerienJunkies::seasonNumberFromName(path);
        r.episodeNumber = QSerienJunkies::episodeNumberFromName(path);
        r.seriesPath = location + "/" + folders.at(1);
        if(folders.size() >= 3){
            r.seasonPath = r.seriesPath + "/" + folders.at(2);
        }
        r.seriesTitle = seriesTitleFromPath(path);
        r.absolutePath = location + path;
        r.language = FileScraper::languageFromPath(path);
        emit result(r);
    }
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

QList<QSharedPointer<Series> > FileScraper::newSeries()
{
    return m_newSeries;
}

QList<QSharedPointer<Season> > FileScraper::newSeasons()
{
    return m_newSeasons;
}

QList<QSharedPointer<Episode> > FileScraper::newEpisodes()
{
    return m_newEpisodes;
}

void FileScraper::consumeResult(const FileScraperPrivate::Result &result)
{
    QSharedPointer<Series> series = Series::forTitle(result.seriesTitle);

    if(!series) {
        series = Qp::create<Series>();
        series->setTitle(result.seriesTitle);
        Qp::update(series);
        m_newSeries.append(series);
    }
    if(!series->folders().contains(result.seriesPath)) {
        series->addFolder(result.seriesPath);
    }

    QSharedPointer<Season> season = series->season(result.seasonNumber);
    if(!season) {
        season = Qp::create<Season>();
        season->setNumber(result.seasonNumber);
        series->addSeason(season);
        Qp::update(season);
        m_newSeasons.append(season);
    }
    if(!season->folders().contains(result.seasonPath)) {
        season->addFolder(result.seasonPath);
        Qp::update(season);
    }

    QSharedPointer<Episode> episode = season->episode(result.episodeNumber);

    if(!episode) {
        episode = Qp::create<Episode>();
        episode->setNumber(result.episodeNumber);
        season->addEpisode(episode);
        episode->setVideoFile(result.absolutePath);
        episode->setPrimaryLanguage(result.language);
        episode->setStatus(Episode::Ok);
        Qp::update(episode);
        m_newEpisodes.append(episode);
    }
    else if(episode->videoFile() != result.absolutePath) {
        qDebug() << QString("Duplicate episode: %1 and %2")
                    .arg(episode->videoFile())
                    .arg(result.absolutePath);
        // TODO: Allow duplicate episodes
    }
}

#define COMMA ,

QLocale::Language FileScraper::languageFromPath(const QString &path)
{
    foreach(QPair<QLocale::Language COMMA QRegularExpression> pair, LANGUAGE_REGEXPS()) {
        QRegularExpression regExp = pair.second;
        QRegularExpressionMatch match = regExp.match(path);
        if(match.hasMatch())
            return pair.first;
    }

    return QLocale::AnyLanguage;
}

#undef COMMA
