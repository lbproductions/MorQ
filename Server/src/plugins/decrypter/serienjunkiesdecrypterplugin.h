#ifndef SERIENJUNKIESDECRYPTERPLUGIN_H
#define SERIENJUNKIESDECRYPTERPLUGIN_H

#include "decrypterplugin.h"

#include <model/downloadpackage.h>

class SerienJunkiesDecrypterPlugin : public DecrypterPlugin
{
    Q_OBJECT
public:
    explicit SerienJunkiesDecrypterPlugin(QObject *parent = 0);

    void handlePackage(QSharedPointer<DownloadPackage> package);
    bool canHandleUrl(const QUrl &url) const;

private slots:
    void packageFinished();
};

class SerienJunkiesDecryptHandler : public QObject
{
    Q_OBJECT
public:
    SerienJunkiesDecryptHandler(QSharedPointer<DownloadPackage> package, SerienJunkiesDecrypterPlugin *parent);
    void getInformation();

    QSharedPointer<DownloadPackage> package() const;

signals:
    void finished();

private slots:
    void requestCaptchaSolution();
    void replyFinished();

private:
    QSharedPointer<DownloadPackage> m_package;
    SerienJunkiesDecrypterPlugin *m_plugin;
};

#endif // SERIENJUNKIESDECRYPTERPLUGIN_H
