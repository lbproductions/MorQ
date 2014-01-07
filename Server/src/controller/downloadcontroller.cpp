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

QSharedPointer<Download> DownloadController::findNextUnfinishedDownload()
{
    QList<QSharedPointer<DownloadPackage> > packages = Qp::readAll<DownloadPackage>();

    foreach (QSharedPointer<DownloadPackage> package, packages) {
        if (package->isDownloadFinished())
            continue;
        QStringList fileNames;
        foreach (QSharedPointer<Download> dl , package->downloads()) {
            if (fileNames.contains(dl->fileName())) {
                removeDownload(dl);
            }
            else{
                fileNames.append(dl->fileName());
            }
        }
        foreach (QSharedPointer<Download> dl , package->downloads()) {
            if (!dl->isDownloadFinished()
               && !m_runningDownloaders.contains(dl->id())
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
            m_runningDownloaders.insert(dl->id(), downloader);
            m_runningDownloads.append(dl);

            connect(dl.data(), &Download::downloadFinished, [=]() {
                m_runningDownloaders.remove(dl->id());
                m_runningDownloads.removeAll(dl);
                startNextDownload();
            });

            connect(dl.data(), &Download::enabled, [=]() {
                if (!dl->isEnabled()) {
                    m_runningDownloads.removeAll(dl);
                    m_runningDownloaders.remove(dl->id());
                    startNextDownload();
                }
            });

            emit statusChanged();
            return true;
        }
    }

    return false;
}

void DownloadController::removeDownload(QSharedPointer<Download> download)
{
    bool wasRunning = false;

    if (m_runningDownloaders.contains(download->id())) {
        stopDownload(download);
        wasRunning = true;
    }

    Qp::remove(download);

    if (download->package()) {
        download->package()->removeDownload(download);
    }

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

    if (m_runningDownloaders.contains(download->id())) {
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
    foreach (QSharedPointer<Download> download, m_runningDownloads) {
        stopDownload(download);
    }
}

void DownloadController::stopDownload(QSharedPointer<Download> download)
{
    if (m_runningDownloaders.contains(download->id())) {
        m_runningDownloaders.value(download->id())->abortDownload();
        m_runningDownloaders.remove(download->id());
        m_runningDownloads.removeAll(download);

        emit statusChanged();
    }
}
