#ifndef DOWNLOADPACKAGE_H
#define DOWNLOADPACKAGE_H

#include <QObject>

#include <QPersistence.h>
#include <QPersistenceRelations.h>

#include <QElapsedTimer>
#include <QTime>
#include <QUrl>

class Download;

class DownloadPackage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString message READ message WRITE setMessage)
    Q_PROPERTY(QList<QSharedPointer<Download> > downloads READ downloads WRITE setDownloads)
    Q_PROPERTY(QUrl sourceUrl READ sourceUrl WRITE setSourceUrl)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:downloads",
                "reverserelation=package;")

public:
    explicit DownloadPackage(QObject *parent = 0);
    ~DownloadPackage();

    int id() const;

    QString name() const;
    void setName(const QString &name);

    QString message() const;
    void setMessage(const QString &message);

    QUrl sourceUrl() const;
    void setSourceUrl(const QUrl& url);

    QList<QSharedPointer<Download> > downloads() const;
    void addDownload(QSharedPointer<Download> download);
    void removeDownload(QSharedPointer<Download> download);

    QByteArray captcha() const;
    void setCaptcha(const QByteArray &captcha);

    QString captchaString() const;
    void setCaptchaString(const QString &string);

    qint64 totalFileSize() const;
    qint64 bytesDownloaded() const;

    double progress() const;
    double downloadProgress() const;

    bool isDownloadFinished() const;

    bool isExtracting() const;
    bool isExtractionFinished() const;
    double extractionProgress() const;

    qint64 bytesExtracted() const;
    void setBytesExtracted(qint64 bytesExtracted);

    qint64 extractedFileSize() const;
    void setExtractedFileSize(qint64 extractedFileSize);

    qint64 speed() const;
    qint64 speedWeighted() const;
    QTime eta() const;

    QString extractFolder() const;
    void setExtractFolder(const QString &extractFolder);



signals:
    void captchaStringChanged();
    void downloadFinished();

private slots:
    void maybeEmitDownloadFinished();

private:
    void setDownloads(const QList<QSharedPointer<Download> > downloads);

    void calculateSpeed() const;
    QList<QSharedPointer<Download> > differentDownloads() const;

    QString m_name;
    QString m_message;
    QpStrongRelation<Download> m_downloads;
    QUrl m_sourceUrl;
    QByteArray m_captcha;
    QString m_captchaString;

    qint64 m_bytesExtracted;
    qint64 m_extractedFilesSize;

    mutable qint64 m_speed;
    mutable qint64 m_weightedSpeed;
    mutable QElapsedTimer m_speedTimer;
    mutable qint64 m_bytesDownloadedAtLastSpeedMeasurement;
    mutable QTime m_eta;

    static float s_speedAlpha;

    QString m_extractFolder;
};

Q_DECLARE_METATYPE(QSharedPointer<DownloadPackage> )

#endif // DOWNLOADPACKAGE_H
