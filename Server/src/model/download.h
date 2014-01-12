#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>

#include <QPersistence.h>

#include <QElapsedTimer>
#include <QUrl>
#include <QTime>

#include "downloadpackage.h"

class Download : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QUrl redirectedUrl READ redirectedUrl WRITE setRedirectedUrl)
    Q_PROPERTY(QString destinationFolder READ destinationFolder WRITE setDestinationFolder)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(QString message READ message WRITE setMessage)
    Q_PROPERTY(QSharedPointer<DownloadPackage> package READ package WRITE setPackage)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize)
    Q_PROPERTY(int bytesDownloaded READ bytesDownloaded WRITE setBytesDownloaded)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:package",
                "reverserelation=downloads;")

public:
    explicit Download(QObject *parent = 0);
    ~Download();

    void reset();

    QSharedPointer<DownloadPackage> package() const;

    int order() const;

    QUrl url() const;
    void setUrl(const QUrl &redirectedUrl);

    QUrl redirectedUrl() const;
    void setRedirectedUrl(const QUrl &redirectedUrl);

    QString destinationFolder() const;
    void setDestinationFolder(const QString &destinationFolder);

    QString fileName() const;
    void setFileName(const QString &fileName);

    QString message() const;
    void setMessage(const QString &message);

    bool isEnabled() const;
    void setEnabled(bool isEnabled);

    qint64 fileSize() const;
    void setFileSize(qint64 bytes);

    qint64 bytesDownloaded() const;
    void setBytesDownloaded(qint64 bytes);

    double downloadProgress() const;
    bool isDownloadFinished() const;

    bool isExtracting() const;
    void setExtracting(bool extraction);

    qint64 speed() const;
    qint64 speedWeighted() const;
    QTime eta() const;

signals:
    void downloadFinished();
    void enabled(bool);

private:

    void setPackage(QSharedPointer<DownloadPackage> package);

    friend class DownloadPackage;
    void calculateSpeed() const;

    QUrl m_url;
    QUrl m_redirectedUrl;
    QString m_destinationFolder;
    QString m_fileName;
    qint64 m_fileSize;
    qint64 m_bytesDownloaded;
    QString m_message;
    QpWeakRelation<DownloadPackage> m_package;
    bool m_enabled;
    bool m_extracting;

    mutable qint64 m_speed;
    mutable qint64 m_weightedSpeed;
    mutable QElapsedTimer m_speedTimer;
    mutable qint64 m_bytesProcessedAtLastSpeedMeasurement;
    mutable QTime m_eta;

    static float s_speedAlpha;
};

Q_DECLARE_METATYPE(QSharedPointer<Download> )

#endif // DOWNLOAD_H
