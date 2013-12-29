#ifndef EPISODE_H
#define EPISODE_H

#include <QObject>

#include <QPersistence.h>
#include <QPersistenceRelations.h>
#include <QLocale>

class VideoDownloadLink;
class Season;

class Episode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString serienJunkiesTitle READ serienJunkiesTitle WRITE setSerienJunkiesTitle)
    Q_PROPERTY(QSharedPointer<Season>  season READ season WRITE setSeason)
    Q_PROPERTY(QString videoFile READ videoFile WRITE setVideoFile)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QList<QSharedPointer<VideoDownloadLink> > downloadLinks READ downloadLinks WRITE setDownloadLinks)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:season",
                "reverserelation=episodes;")

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:downloadLinks",
                    "reverserelation=episode;")

public:
    explicit Episode(QObject *parent = 0);
    ~Episode();

    int id() const;

    int number() const;
    void setNumber(int number);

    int seasonNumber() const;
    void setSeasonNumber(int number);

    QSharedPointer<Season> season() const;

    QList<QSharedPointer<VideoDownloadLink> > downloadLinks() const;
    QList<QSharedPointer<VideoDownloadLink> > downloadLinks(const QString &formatDescription, const QString &mirror) const;
    void addDownloadLink(QSharedPointer<VideoDownloadLink> link);
    void removeDownloadLink(QSharedPointer<VideoDownloadLink> link);

    QString videoFile() const;
    void setVideoFile(const QString &fileName);

    QLocale::Language primaryLanguage() const;
    void setPrimaryLanguage(QLocale::Language language);

    QSet<QLocale::Language> languages() const;

    // SerienJunkies
    QString serienJunkiesTitle() const;
    void setSerienJunkiesTitle(const QString &title);

    // TheTVDB
    QString title() const;
    void setTitle(const QString &title);

    QString overview() const;
    void setOverview(const QString &overview);

    QString tvdbLanguage() const;

    QPixmap primaryLanguageFlag() const;

private:
    friend class Season;
    void setSeason(QSharedPointer<Season> season);
    void setDownloadLinks(const QList<QSharedPointer<VideoDownloadLink> > &links);

    int m_number;
    int m_seasonNumber;
    QpStrongRelation<VideoDownloadLink> m_downloadLinks;
    QString m_serienJunkiesTitle;
    QpWeakRelation<Season> m_season;
    QString m_videoFile;
    QString m_title;
    QString m_overview;
    QLocale::Language m_primaryLanguage;
};

#endif // EPISODE_H
