#ifndef FILESCRAPER_H
#define FILESCRAPER_H

#include <QObject>
#include <QLocale>

#include "scraper.h"

namespace FileScraperPrivate {
struct Result {
    int seasonNumber;
    int episodeNumber;
    QString seriesTitle;
    QString seriesPath;
    QString seasonPath;
    QString absolutePath;
    QLocale::Language language;
};
}

class Series;
class Season;
class Episode;

class FileScraper : public Scraper
{
    Q_OBJECT
public:
    explicit FileScraper(QObject *parent = 0);

    int locationCount() const;
    int currentLocationCount() const;
    QString currentLocation() const;

    void scan();

    void scanAllLocationsForShowsSeasonsAndEpisodesAsync();

    void scanAllLocationsForShowsSeasonsAndEpisodes();
    void scanLocationForShowsSeasonsAndEpisodes(const QString &location);

    QString seriesTitleFromPath(const QString &path);

    QList<Series *> newSeries();
    QList<Season *> newSeasons();
    QList<Episode *> newEpisodes();

    static QStringList VIDEOEXTENSIONS();

signals:
    void scrapingFile(const QString &folder);

    void result(const FileScraperPrivate::Result &result);

private slots:
    void consumeResult(const FileScraperPrivate::Result &result);

private:
    int m_locationCount;
    int m_currentLocationCount;
    QString m_currentLocation;
    QList<Series *> m_newSeries;
    QList<Season *> m_newSeasons;
    QList<Episode *> m_newEpisodes;

    static QLocale::Language languageFromPath(const QString &path);
};

Q_DECLARE_METATYPE(FileScraperPrivate::Result)

#endif // FILESCRAPER_H
