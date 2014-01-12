#ifndef EPISODE_H
#define EPISODE_H

#include <QObject>

#include <QPersistence.h>
#include <QLocale>
#include <QDate>

class OnlineResource;
class Season;

class Episode : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(int tvdbId READ tvdbId WRITE setTvdbId)
    Q_PROPERTY(QSharedPointer<Season>  season READ season WRITE setSeason)
    Q_PROPERTY(QString videoFile READ videoFile WRITE setVideoFile)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QList<QSharedPointer<OnlineResource> > downloadLinks READ downloadLinks WRITE setDownloadLinks)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QDate firstAired READ firstAired WRITE setFirstAired)
    Q_PROPERTY(Status status READ status WRITE setStatus)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:season",
                "reverserelation=episodes;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:downloadLinks",
                    "reverserelation=episode;")

public:
    enum Status {
        UnkownStatus,
        DownloadAvailable,
        Missing,
        Ok,
        Downloading ,
        Extracting,
        Upcoming
    };

    explicit Episode(QObject *parent = 0);
    ~Episode();

    Status status() const;
    void setStatus(Episode::Status status);
    QPixmap statusPixmap() const;
    QString statusMessage() const;

    int number() const;
    void setNumber(int number);

    int seasonNumber() const;
    void setSeasonNumber(int number);

    QSharedPointer<Season> season() const;

    QList<QSharedPointer<OnlineResource> > downloadLinks() const;
    QList<QSharedPointer<OnlineResource> > downloadLinks(const QString &formatDescription, const QString &mirror) const;
    void addDownloadLink(QSharedPointer<OnlineResource> link);
    void removeDownloadLink(QSharedPointer<OnlineResource> link);

    QString videoFile() const;
    void setVideoFile(const QString &fileName);

    QSet<QLocale::Language> languages() const;
    QLocale::Language primaryLanguage() const;
    void setPrimaryLanguage(QLocale::Language language);
    QPixmap primaryLanguageFlag() const;

    // TheTVDB
    int tvdbId() const;
    void setTvdbId(int arg);

    int tvdbSeasonId() const;
    void setTvdbSeasonId(int arg);

    QUrl tvdbUrl() const;

    QString title() const;
    void setTitle(const QString &title);

    QString overview() const;
    void setOverview(const QString &overview);

    QString tvdbLanguage() const;

    QDate firstAired() const;
    void setFirstAired(const QDate &firstAired);

private:
    friend class Season;
    void setSeason(QSharedPointer<Season> season);
    void setDownloadLinks(const QList<QSharedPointer<OnlineResource> > &links);

    int m_number;
    int m_tvdbId;
    int m_tvdbSeasonId;
    int m_seasonNumber;
    Status m_status;
    QpStrongRelation<OnlineResource> m_downloadLinks;
    QString m_serienJunkiesTitle;
    QpWeakRelation<Season> m_season;
    QString m_videoFile;
    QString m_title;
    QString m_overview;
    QLocale::Language m_primaryLanguage;
    QDate m_firstAired;
};

#endif // EPISODE_H
