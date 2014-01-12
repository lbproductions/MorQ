#include "downloadcontroller.h"

#include "controller.h"
#include "downloader.h"
#include "plugincontroller.h"

#include "plugins/hoster/hosterplugin.h"
#include "preferences.h"

#include "model/download.h"
#include "model/downloadpackage.h"

#include <QDir>

DownloadController::DownloadController(QObject *parent) :
    QObject(parent)
{
}

DownloadController::~DownloadController()
{
    stopDownloads();
}

QSharedPointer<Download> DownloadController::findNextUnfinishedDownload()
{
    QList<QSharedPointer<DownloadPackage> > packages = Qp::readAll<DownloadPackage>();

    foreach (QSharedPointer<DownloadPackage> package, packages) {
        if (package->isDownloadFinished())
            continue;

        foreach (QSharedPointer<Download> dl , package->downloads()) {
            if (!dl->isDownloadFinished()
                    && !m_runningDownloaders.contains(dl)
                    && dl->isEnabled()) {
                return dl;
            }

        }
    }

    return QSharedPointer<Download>();
}

bool DownloadController::startNextDownload()
{
    // find the next download
    QSharedPointer<Download> dl = findNextUnfinishedDownload();
    if (!dl) {
        emit statusChanged();
        return false;
    }

    foreach (HosterPlugin *hoster, Controller::plugins()->hosterPlugins()) {
        if (hoster->canHandleUrl(dl->url())) {
            Downloader *downloader = hoster->handleDownload(dl);
            downloader->setDownload(dl);
            m_runningDownloaders.insert(dl, downloader);
            connect(downloader, &Downloader::finished,
                    this, &DownloadController::handleFinishedDownloader);

            emit statusChanged();
            return true;
        }
    }

    return false;
}

void DownloadController::handleFinishedDownloader()
{
    Downloader *downloader = static_cast<Downloader *>(sender());
    m_runningDownloaders.remove(downloader->download());
    downloader->deleteLater();
    startNextDownload();
}

void DownloadController::removeDownload(QSharedPointer<Download> download)
{
    bool wasRunning = isDownloadRunning();

    if (m_runningDownloaders.contains(download)) {
        stopDownload(download);
    }

    download->package()->removeDownload(download);
    Qp::remove(download);

    if (wasRunning)
        startDownloads();
}

void DownloadController::removePackage(QSharedPointer<DownloadPackage> package)
{
    foreach (QSharedPointer<Download> dl, package->downloads()) {
        if (dl)
            removeDownload(dl);
    }

    Qp::remove(package);
}

void DownloadController::resetDownload(QSharedPointer<Download> download)
{
    bool wasRunning = false;

    if (m_runningDownloaders.contains(download)) {
        stopDownload(download);
        wasRunning = true;
    }

    QFile file(Preferences::downloadFolder() + QDir::separator() + download->fileName());
    if (file.exists())
        file.remove();

    download->reset();
    Qp::update(download);
    foreach (HosterPlugin *hoster, Controller::plugins()->hosterPlugins()) {
        if (hoster->canHandleUrl(download->url())) {
            hoster->getDownloadInformation(download);
            break;
        }
    }

    if (wasRunning
            || isDownloadRunning())
        startDownloads();
}

void DownloadController::resetPackage(QSharedPointer<DownloadPackage> package)
{
    foreach (QSharedPointer<Download> dl, package->downloads()) {
        resetDownload(dl);
    }
}

bool DownloadController::isDownloadRunning()
{
    return !m_runningDownloaders.isEmpty();
}

void DownloadController::startDownloads()
{
    while (m_runningDownloaders.size() < Preferences::maxDownloads()) {
        if (!startNextDownload())
            break;
    }
}

void DownloadController::stopDownloads()
{
    foreach (QSharedPointer<Download> download, m_runningDownloaders.keys()) {
        stopDownload(download);
    }
}

void DownloadController::stopDownload(QSharedPointer<Download> download)
{
    if (m_runningDownloaders.contains(download)) {
        Downloader *downloader = m_runningDownloaders.value(download);
        downloader->abortDownload();
        m_runningDownloaders.remove(download);
        downloader->deleteLater();
        emit statusChanged();
    }
}
