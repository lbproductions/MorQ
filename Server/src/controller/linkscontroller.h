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

    QSharedPointer<Download> createDownload(const QUrl &url);
    QSharedPointer<DownloadPackage> createPackage(const QUrl &url);

    void downloadEpisode(QSharedPointer<Episode> episode);

    void downloadVideos(QList<QSharedPointer<VideoDownloadLink> > links);

private slots:
    void clipboardChanged();
    void packageFinished(QSharedPointer<DownloadPackage> package);

};

#endif // LINKSCONTROLLER_H
