#include "filestorage.h"

#include "controller.h"

#include <QCryptographicHash>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QDebug>

FileStorage::FileStorage(const QUrl &url, QObject *parent) :
    QObject(parent)
{
    m_url = url;
    m_localPath = FileStorage::urlToLocalPath(url);

    QFile f(m_localPath);
    if(f.exists()) {
        emit finished();
        return;
    }

    qDebug() << "Requesting" << m_url;

    QNetworkReply *reply = Controller::networkAccessManager()->get(QNetworkRequest(m_url));
    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
}

QString FileStorage::localFilePath() const
{
    return m_localPath;
}

QUrl FileStorage::url() const
{
    return m_url;
}

QString FileStorage::urlToLocalPath(const QUrl &url)
{
    QString urlString = url.toString();
    QCryptographicHash hash( QCryptographicHash::Sha1 );
    hash.addData(urlString.toUtf8());

    QString urlHash = QString(hash.result().toHex().constData());
    QString urlFileName = urlString.right(urlString.length() - 1 - urlString.lastIndexOf('/'));

    QString fileName = QString("%1.%2")
            .arg(urlHash)
            .arg(urlFileName);

    static QDir dataDir(QStandardPaths::standardLocations(QStandardPaths::DataLocation).first());
    return dataDir.absoluteFilePath(fileName);
}

void FileStorage::onFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());

    qDebug() << "Received file" << m_url;

    if(!reply->error()) {
        QFile f(m_localPath);
        f.open(QFile::WriteOnly | QIODevice::Truncate);
        qDebug() << "Writing into " << m_localPath;
        if(f.write(reply->readAll()) != -1) {
            qDebug() << "done.";
            emit finished();
        }
        else {
            qDebug() << "error" << f.errorString();
            emit error();
        }
        f.close();
    }
    reply->deleteLater();
}

void FileStorage::onError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    qDebug() << reply->error();
    emit error();
    reply->deleteLater();
}
