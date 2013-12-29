#include "downloadpackage.h"

#include "download.h"
#include "videodownloadlink.h"

#include <QTime>
#include <QDebug>

float DownloadPackage::s_speedAlpha(0.1);

DownloadPackage::DownloadPackage(QObject *parent) :
    QObject(parent),
    m_downloads("downloads", this),
    m_videoDownloadLinks("videoDownloadLinks", this),
    m_bytesExtracted(-1),
    m_extractedFilesSize(-1),
    m_speed(0),
    m_weightedSpeed(0),
    m_speedTimer(QElapsedTimer()),
    m_bytesDownloadedAtLastSpeedMeasurement(-1)
{
}

DownloadPackage::~DownloadPackage()
{
}

int DownloadPackage::id() const
{
    return Qp::primaryKey(Qp::sharedFrom(this));
}

QString DownloadPackage::name() const
{
    return m_name;
}

void DownloadPackage::setName(const QString &name)
{
    m_name = name;
}

QString DownloadPackage::message() const
{
    return m_message;
}

void DownloadPackage::setMessage(const QString &message)
{
    m_message = message;
}

QUrl DownloadPackage::sourceUrl() const
{
    return m_sourceUrl;
}

void DownloadPackage::setSourceUrl(const QUrl &url)
{
    m_sourceUrl = url;
}

QList<QSharedPointer<Download> > DownloadPackage::downloads() const
{
    return m_downloads.resolveList();
}

void DownloadPackage::addDownload(QSharedPointer<Download> download)
{
    connect(download.data(), &Download::downloadFinished,
            this, &DownloadPackage::maybeEmitDownloadFinished);
    download->setPackage(Qp::sharedFrom(this));
    m_downloads.relate(download);
}

void DownloadPackage::removeDownload(QSharedPointer<Download> download)
{
    disconnect(download.data(), 0, this, 0);
    download->setPackage(QSharedPointer<DownloadPackage>());
    m_downloads.unrelate(download);
}

QList<QSharedPointer<VideoDownloadLink> > DownloadPackage::videoDownloadLinks() const
{
    return m_videoDownloadLinks.resolveList();
}

QByteArray DownloadPackage::captcha() const
{
    return m_captcha;
}

void DownloadPackage::setCaptcha(const QByteArray &captcha)
{
    m_captcha = captcha;
}

QString DownloadPackage::captchaString() const
{
    return m_captchaString;
}

void DownloadPackage::setCaptchaString(const QString &string)
{
    m_captchaString = string;
    emit captchaStringChanged();
}

void DownloadPackage::setDownloads(const QList<QSharedPointer<Download> > downloads)
{
    foreach(QSharedPointer<Download> download, this->downloads()){
        disconnect(download.data(), 0, this, 0);
    }

    foreach(QSharedPointer<Download> download, downloads){
        connect(download.data(), &Download::downloadFinished,
                this, &DownloadPackage::maybeEmitDownloadFinished);
    }

    m_downloads.relate(downloads);
}

void DownloadPackage::setVideoDownloadLinks(const QList<QSharedPointer<VideoDownloadLink> > downloads)
{
    m_videoDownloadLinks.relate(downloads);
}

qint64 DownloadPackage::totalFileSize() const
{
    qint64 total = 0;
    foreach(QSharedPointer<Download> dl, differentDownloads()) {
        total += dl->fileSize();
    }

    return total;
}

qint64 DownloadPackage::bytesDownloaded() const
{
    qint64 total = 0;
    foreach(QSharedPointer<Download> dl, differentDownloads()) {
        total += dl->bytesDownloaded();
    }

    return total;
}

double DownloadPackage::progress() const
{
    if(isExtracting())
        return extractionProgress();

    return downloadProgress();
}

double DownloadPackage::downloadProgress() const
{
    qint64 byteDown = bytesDownloaded();
    qint64 total = totalFileSize();
    if(byteDown <= 0
            || total <= 0)
        return 0.0;

    return double(byteDown) / double(total);
}

bool DownloadPackage::isDownloadFinished() const
{
    return totalFileSize() == bytesDownloaded();
}

bool DownloadPackage::isExtracting() const
{
    return m_extractedFilesSize > 0 && m_bytesExtracted > 0;
}

bool DownloadPackage::isExtractionFinished() const
{
    return m_extractedFilesSize == m_bytesExtracted;
}

double DownloadPackage::extractionProgress() const
{
    if(m_extractedFilesSize <= 0
            || m_bytesExtracted <= 0)
        return 0.0;

    return double(m_bytesExtracted) / double(m_extractedFilesSize);
}

qint64 DownloadPackage::bytesExtracted() const
{
    return m_bytesExtracted;
}

void DownloadPackage::setBytesExtracted(qint64 bytesExtracted)
{
    m_bytesExtracted = bytesExtracted;
}

qint64 DownloadPackage::extractedFileSize() const
{
    return m_extractedFilesSize;
}

void DownloadPackage::setExtractedFileSize(qint64 extractedFileSize)
{
    m_extractedFilesSize = extractedFileSize;
}

void DownloadPackage::calculateSpeed() const
{
    if(!m_speedTimer.isValid()) {
        m_speedTimer.start();
        return;
    }

    if(isDownloadFinished()) {
        m_speed = 0;
        m_weightedSpeed = 0;
        m_eta = QTime();
        return;
    }


    qint64 elapsedTime = m_speedTimer.elapsed();
    if(elapsedTime > 100) {
        qint64 bytes = 0;
        if(isExtracting())
            bytes = m_bytesExtracted;
        else
            bytes = bytesDownloaded();

        qint64 bytesWritten = bytes - m_bytesDownloadedAtLastSpeedMeasurement;

        if(bytesWritten == 0
                && elapsedTime < 3000)
            return;

        if(m_bytesDownloadedAtLastSpeedMeasurement < 0) {
            m_bytesDownloadedAtLastSpeedMeasurement = bytes;
            return;
        }

        m_bytesDownloadedAtLastSpeedMeasurement = bytes;

        m_speed = bytesWritten * 1000 / elapsedTime;
        m_speed = qMax(qint64(0), m_speed);

        if(m_weightedSpeed < 1000) // if weighted is less than 1kb/s just reset it.
            m_weightedSpeed = m_speed;
        else if(m_speed <= 0)
            m_weightedSpeed /= 10;
        else
            m_weightedSpeed = m_speed * s_speedAlpha + m_weightedSpeed * (1 - s_speedAlpha);

        m_speed = qMax(qint64(0), m_speed);
        m_speedTimer.restart();

        if(m_weightedSpeed <= 0) {
            m_eta = QTime();
            return;
        }

        qint64 bytesLeft = totalFileSize() - bytes;

        if(bytesLeft < 0) {
            m_eta = QTime();
            return;
        }

        if(bytesLeft == 0) {
            m_eta = QTime(0,0,0);
            return;
        }

        m_eta = QTime(0,0,0);
        m_eta = m_eta.addSecs(int(bytesLeft / m_weightedSpeed));

        QTime maxDownloadEta = QTime();
        foreach(QSharedPointer<Download> dl, downloads()) {
            maxDownloadEta = qMax(dl->eta(), maxDownloadEta);
        }
        m_eta = qMax(maxDownloadEta, m_eta);
    }
}

QList<QSharedPointer<Download> > DownloadPackage::differentDownloads() const
{
    QList<QSharedPointer<Download> > differentFiles;
    QStringList fileNames;
    foreach(QSharedPointer<Download>  dl, downloads()) {
        if(!fileNames.contains(dl->fileName())) {
            differentFiles.append(dl);
            fileNames.append(dl->fileName());
        }
    }
    return differentFiles;
}

qint64 DownloadPackage::speed() const
{
    calculateSpeed();
    return m_speed;
}

qint64 DownloadPackage::speedWeighted() const
{
    calculateSpeed();
    return m_weightedSpeed;
}

QTime DownloadPackage::eta() const
{
    calculateSpeed();
    return m_eta;
}

QString DownloadPackage::extractFolder() const
{
    return m_extractFolder;
}


void DownloadPackage::maybeEmitDownloadFinished()
{
    if(isDownloadFinished())
        emit downloadFinished();
}


void DownloadPackage::setExtractFolder(const QString &extractFolder)
{
    m_extractFolder = extractFolder;
}


void DownloadPackage::addVideoDownloadLink(QSharedPointer<VideoDownloadLink> download)
{
    m_videoDownloadLinks.relate(download);
}
