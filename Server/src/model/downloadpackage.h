#ifndef DOWNLOADPACKAGE_H
#define DOWNLOADPACKAGE_H

#include <QObject>

#include <QPersistence.h>

#include <QElapsedTimer>
#include <QTime>
#include <QUrl>

class Download;
class VideoDownloadLink;

class DownloadPackage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString message READ message WRITE setMessage)
    Q_PROPERTY(QList<Download*> downloads READ downloads WRITE setDownloads)
    Q_PROPERTY(QUrl sourceUrl READ sourceUrl WRITE setSourceUrl)
    //Q_PROPERTY(QList<VideoDownloadLink*> videoDownloadLinks READ videoDownloadLinks WRITE setVideoDownloadLinks)

    Q_CLASSINFO(QPERSISTENCE_PRIMARYKEY, "id")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:id",
                "autoincremented=true;")
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

    QList<Download *> downloads() const;
    void addDownload(Download *download);
    void removeDownload(Download *download);

    QList<VideoDownloadLink *> videoDownloadLinks() const;
    void addVideoDownloadLink(VideoDownloadLink *download);


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
    void setId(int id);
    void setDownloads(const QList<Download *> downloads);
    void setVideoDownloadLinks(const QList<VideoDownloadLink *> downloads);

    void calculateSpeed() const;

    qint64 m_id;
    QString m_name;
    QString m_message;
    QList<Download *> m_downloads;
    QList<VideoDownloadLink*> m_videoDownloadLinks;
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

Q_DECLARE_METATYPE(DownloadPackage *)

#endif // DOWNLOADPACKAGE_H
