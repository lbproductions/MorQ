#ifndef LINKSCONTROLLER_H
#define LINKSCONTROLLER_H

#include <QObject>

class Download;
class DownloadPackage;
class Episode;
class VideoDownloadLink;

class LinksController : public QObject
{
    Q_OBJECT
public:
    explicit LinksController(QObject *parent = 0);

    Download *createDownload(const QUrl &url);
    DownloadPackage *createPackage(const QUrl &url);

    void downloadEpisode(Episode *episode);

    void downloadVideos(QList<VideoDownloadLink*> links);

private slots:
    void clipboardChanged();
    void packageFinished(DownloadPackage *package);

};

#endif // LINKSCONTROLLER_H
