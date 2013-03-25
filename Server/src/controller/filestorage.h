#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <QObject>

#include <QFile>
#include <QNetworkReply>

class QNetworkAccessManager;
class FileStorage;

class FileStorage : public QObject
{
    Q_OBJECT
public:
    FileStorage(const QUrl &url, QObject *parent = 0);

    QString localFilePath() const;
    QUrl url() const;

signals:
    void finished();
    void error();

private slots:
    void onFinished();
    void onError(QNetworkReply::NetworkError);

private:
    QUrl m_url;
    QString m_localPath;

    static QString urlToLocalPath(const QUrl &url);
};

#endif // FILESTORAGE_H
