#ifndef VIDEODOWNLOADLINK_H
#define VIDEODOWNLOADLINK_H

#include <QObject>

#include <QPersistence.h>

#include <QUrl>

class Episode;
class Season;

class OnlineResource : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString formatDescription READ formatDescription WRITE setFormatDescription)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QSharedPointer<Episode>  episode READ episode WRITE setEpisode)
    Q_PROPERTY(QSharedPointer<Season>  season READ season WRITE setSeason)
    Q_PROPERTY(QString extractFolder READ extractFolder WRITE setExtractFolder)
    Q_PROPERTY(QList<int> _languages READ _languages WRITE _setLanguages)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:episode",
                "reverserelation=downloadLinks;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:season",
                "reverserelation=serienjunkiesUrls;")


public:
    explicit OnlineResource(QObject *parent = 0);
    ~OnlineResource();

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
    QSharedPointer<Season> season() const;

    QList<QLocale::Language> languages() const;
    void setLanguages(const QList<QLocale::Language> &languages);

private:
    friend class Episode;
    friend class Season;
    void setId(int id);
    void setEpisode(QSharedPointer<Episode> episode);
    void setSeason(QSharedPointer<Season> season);

    QList<int> _languages() const;
    void _setLanguages(const QList<int> &_languages);

    int m_id;
    QString m_name;
    QString m_formatDescription;
    QUrl m_url;
    QString m_mirror;
    QString m_extractFolder;
    QpWeakRelation<Episode> m_episode;
    QpWeakRelation<Season> m_season;
    QList<int> m_languages;
};

#endif // VIDEODOWNLOADLINK_H
