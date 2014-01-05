#ifndef DOWNLOADCONTROLLER_H
#define DOWNLOADCONTROLLER_H

#include <QObject>
#include <QHash>

#include <model/download.h>

class Downloader;
class DownloadPackage;

class DownloadController : public QObject
{
    Q_OBJECT
public:
    void startDownloads();
    void stopDownloads();
    void stopDownload(QSharedPointer<Download> download);
    void removeDownload(QSharedPointer<Download> download);
    void removePackage(QSharedPointer<DownloadPackage> package);
    void resetDownload(QSharedPointer<Download> download);
    void resetPackage(QSharedPointer<DownloadPackage> package);

    bool isDownloadRunning();

signals:
    void statusChanged();

private:
    friend class Controller;
    explicit DownloadController(QObject *parent = 0);

    bool startNextDownload();

    QSharedPointer<Download> findNextUnfinishedDownload();

    QList<QSharedPointer<Download> > m_runningDownloads;
    QHash<int, Downloader *> m_runningDownloaders;

    Q_DISABLE_COPY(DownloadController)
};

#endif // DOWNLOADCONTROLLER_H
