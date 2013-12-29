#ifndef VIDEODOWNLOADLINK_H
#define VIDEODOWNLOADLINK_H

#include <QObject>

#include <QPersistence.h>
#include <QPersistenceRelations.h>

#include <QUrl>

class Episode;

class VideoDownloadLink : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString formatDescription READ formatDescription WRITE setFormatDescription)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QSharedPointer<Episode>  episode READ episode WRITE setEpisode)
    Q_PROPERTY(QString extractFolder READ extractFolder WRITE setExtractFolder)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:episode",
                "reverserelation=downloadLinks;")

public:
    explicit VideoDownloadLink(QObject *parent = 0);
    ~VideoDownloadLink();

    int id() const;

    QString name() const;
    void setName(const QString &name);

    QString formatDescription() const;
    void setFormatDescription(const QString &formatDescription);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString mirror() const;
    void setMirror(const QString &mirror);

    QString extractFolder() const;
    void setExtractFolder(const QString &extractFolder);

    QSharedPointer<Episode> episode() const;

private:
    friend class Episode;
    void setId(int id);
    void setEpisode(QSharedPointer<Episode> episode);

    int m_id;
    QString m_name;
    QString m_formatDescription;
    QUrl m_url;
    QString m_mirror;
    QString m_extractFolder;
    QpWeakRelation<Episode> m_episode;
};

#endif // VIDEODOWNLOADLINK_H
