#ifndef FILESCRAPER_H
#define FILESCRAPER_H

#include <QObject>

namespace FileScraperPrivate {
struct Result {
    int seasonNumber;
    int episodeNumber;
    QString seriesTitle;
    QString absolutePath;
};
}

class FileScraper : public QObject
{
    Q_OBJECT

public:
    explicit FileScraper(QObject *parent = 0);

    int folderCount() const;
    int currentFolder() const;
    int locationCount() const;
    int currentLocation() const;

    void scanAllLocationsForShowsSeasonsAndEpisodesAsync();

    void scanAllLocationsForShowsSeasonsAndEpisodes();
    void scanLocationForShowsSeasonsAndEpisodes(const QString &location);

    QString seriesTitleFromPath(const QString &path);

signals:
    void enteredFolder(const QString &folder);
    void finished();

    void result(const FileScraperPrivate::Result &result);

private slots:
    void consumeResult(const FileScraperPrivate::Result &result);

private:
    int m_folderCount;
    int m_locationCount;
    int m_currentFolder;
    int m_currentLocation;
};

Q_DECLARE_METATYPE(FileScraperPrivate::Result)

#endif // FILESCRAPER_H
