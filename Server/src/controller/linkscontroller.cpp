#include "linkscontroller.h"

#include "controller.h"
#include "plugincontroller.h"
#include "downloadcontroller.h"
#include "downloader.h"
#include "plugins/hoster/hosterplugin.h"
#include "plugins/decrypter/decrypterplugin.h"

#include "model/download.h"
#include "model/downloadpackage.h"
#include "model/episode.h"
#include "model/onlineresource.h"

#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QUrl>

LinksController::LinksController(QObject *parent) :
    QObject(parent)
{
    foreach(DecrypterPlugin *decrypter, Controller::plugins()->decrypterPlugins()) {
        connect(decrypter, &DecrypterPlugin::finishedPackage,
                this, &LinksController::packageFinished);
    }

#ifdef Q_OS_MAC
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            this, &LinksController::clipboardChanged);
    timer->setInterval(1000);
    timer->start();
#else
    connect(QApplication::clipboard(), &QClipboard::changed,
            this, &DownloadController::clipboardChanged);
#endif
}

void LinksController::clipboardChanged()
{
    static QString oldClipboard;
    QString clipboard = QApplication::clipboard()->text();
    if(clipboard.isEmpty() || clipboard == oldClipboard)
        return;

    oldClipboard = clipboard;

    QUrl url(clipboard);
    if(!url.isValid())
        return;

    foreach(DecrypterPlugin *decrypter, Controller::plugins()->decrypterPlugins()) {
        if(decrypter->canHandleUrl(url)) {
            QSharedPointer<DownloadPackage> package = createPackage(url);
            decrypter->handlePackage(package);
            return;
        }
    }

    foreach(HosterPlugin *hoster, Controller::plugins()->hosterPlugins()) {
        if(hoster->canHandleUrl(url)) {
            QSharedPointer<Download>  dl = createDownload(url);
            hoster->getDownloadInformation(dl);
            return;
        }
    }
}

void LinksController::packageFinished(QSharedPointer<DownloadPackage> package)
{
    foreach(QSharedPointer<Download> dl, package->downloads()) {
        foreach(HosterPlugin *hoster, Controller::plugins()->hosterPlugins()) {
            if(hoster->canHandleUrl(dl->url())) {
                hoster->getDownloadInformation(dl);
                break;
            }
        }
    }
}

QSharedPointer<Download> LinksController::createDownload(const QUrl &url)
{
    QSharedPointer<Download> download = Qp::create<Download>();
    download->setUrl(url);
    Qp::update(download);

    return download;
}

QSharedPointer<DownloadPackage> LinksController::createPackage(const QUrl &url)
{
    QSharedPointer<DownloadPackage> package = Qp::create<DownloadPackage>();
    package->setSourceUrl(url);
    Qp::update(package);

    return package;
}

void LinksController::downloadEpisode(QSharedPointer<Episode> episode)
{
//    QUrl url = episode->downloadLinks();

//    foreach(DecrypterPlugin *decrypter, Controller::plugins()->decrypterPlugins()) {
//        if(decrypter->canHandleUrl(url)) {
//            QSharedPointer<DownloadPackage> package = createPackage(url);
//            decrypter->handlePackage(package);
//            return;
//        }
    //    }
}

void LinksController::downloadVideos(QList<QSharedPointer<OnlineResource> > links)
{
    foreach(QSharedPointer<OnlineResource>  link, links) {
        QUrl url = link->url();
        foreach(DecrypterPlugin *decrypter, Controller::plugins()->decrypterPlugins()) {
            if(decrypter->canHandleUrl(url)) {
                QSharedPointer<DownloadPackage> package = createPackage(url);
                package->setExtractFolder(link->extractFolder());
                decrypter->handlePackage(package);
                link->episode()->setStatus(Episode::Downloading);
                return;
            }
        }
    }
}
