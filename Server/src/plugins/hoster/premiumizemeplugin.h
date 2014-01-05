#ifndef PREMIUMIZEMEPLUGIN_H
#define QSHAREHOSTERPLUGIN_H

#include "hosterplugin.h"

#include <model/download.h>

#include <QHash>

class QNetworkReply;
class PremiumizeMeDownloadHandler;
class Downloader;
class QTimer;

class PremuimizeMePlugin : public HosterPlugin
{
    Q_OBJECT
public:
    explicit PremuimizeMePlugin(QObject *parent = 0);

    void getDownloadInformation(QSharedPointer<Download> download) Q_DECL_OVERRIDE;
    Downloader *handleDownload(QSharedPointer<Download> download) Q_DECL_OVERRIDE;
    bool canHandleUrl(const QUrl &url) const Q_DECL_OVERRIDE;
};

class PremiumizeMeDownloadHandler : public QObject
{
    Q_OBJECT
public:
    PremiumizeMeDownloadHandler(QSharedPointer<Download> download, PremuimizeMePlugin *parent);
    ~PremiumizeMeDownloadHandler();
    void getDownloadInformation();
    void download();

    Downloader *downloader() const;

signals:
    void downloadInformationReady();

private slots:
    void generateLinkReplyFinished();

private:
    QSharedPointer<Download> m_download;
    PremuimizeMePlugin *m_plugin;
    Downloader *m_downloader;
    static QTimer s_timer;

    QNetworkReply *getDownloadInformationReply();
};

#endif // PREMIUMIZEMEPLUGIN_H
