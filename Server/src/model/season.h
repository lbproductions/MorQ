#ifndef SEASON_H
#define SEASON_H

#include <QObject>

#include <QPersistence.h>

#include <QUrl>
#include <QLocale>

class Series;
class Episode;
class OnlineResource;

class Season : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(int tvdbId READ tvdbId WRITE setTvdbId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QSharedPointer<Series>  series READ series WRITE setSeries)
    Q_PROPERTY(QList<QSharedPointer<Episode> > episodes READ episodes WRITE setEpisodes)
    Q_PROPERTY(QList<QSharedPointer<OnlineResource> > serienjunkiesUrls READ serienjunkiesUrls WRITE setSerienjunkiesUrls)
    Q_PROPERTY(QStringList folders READ folders WRITE setFolders)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:series",
                "reverserelation=seasons;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:episodes",
                "reverserelation=season;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:serienjunkiesUrls",
                "reverserelation=season;")

public:
    enum Status {
        UnkownStatus,
        DownloadsAvailable,
        MissingEpisodes,
        Complete
    };

    explicit Season(QObject *parent = 0);
    ~Season();

    int number() const;
    void setNumber(int number);

    Season::Status status() const;
    QPixmap statusPixmap() const;
    QString statusMessage() const;

    QString title() const;
    void setTitle(const QString title);

    QSet<QLocale::Language> languages() const;

    QSharedPointer<Series> series() const;

    QList<QSharedPointer<Episode> > episodes() const;
    QList<QSharedPointer<Episode> > specialEpisodes() const;
    QSharedPointer<Episode> episode(int number) const;
    QSharedPointer<Episode> episode(const QString &title) const;
    void addEpisode(QSharedPointer<Episode> episode);
    void removeEpisode(QSharedPointer<Episode> episode);

    QStringList folders() const;
    void setFolders(const QStringList &folders);
    void addFolder(const QString &folder);

    void addSerienjunkiesUrl(QSharedPointer<OnlineResource> serienjunkiesUrl);
    QList<QSharedPointer<OnlineResource> > serienjunkiesUrls() const;

    int tvdbId() const;
    void setTvdbId(int arg);
    QUrl tvdbUrl() const;

private:
    friend class Series;
    void setSeries(QSharedPointer<Series> series);
    void setEpisodes(const QList<QSharedPointer<Episode> > &episodes);
    void setSerienjunkiesUrls(const QList<QSharedPointer<OnlineResource> > &serienjunkiesUrls);

    int m_number;
    QString m_title;
    QpWeakRelation<Series> m_series;
    QpStrongRelation<Episode> m_episodes;
    QpStrongRelation<OnlineResource> m_serienjunkiesUrls;
    mutable QMultiMap<int, QSharedPointer<Episode> > m_episodesByNumber;
    QStringList m_folders;
    int m_tvdbId;
};

Q_DECLARE_METATYPE(Season::Status)

#endif // SEASON_H
