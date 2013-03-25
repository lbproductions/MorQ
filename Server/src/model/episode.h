#ifndef EPISODE_H
#define EPISODE_H

#include <QObject>

#include <QPersistence.h>
#include <QLocale>

class VideoDownloadLink;
class Season;

class Episode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString serienJunkiesTitle READ serienJunkiesTitle WRITE setSerienJunkiesTitle)
    Q_PROPERTY(Season* season READ season WRITE setSeason)
    Q_PROPERTY(QString videoFile READ videoFile WRITE setVideoFile)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QList<VideoDownloadLink*> downloadLinks READ downloadLinks WRITE setDownloadLinks)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)

    Q_CLASSINFO(QPERSISTENCE_PRIMARYKEY, "id")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:id",
                "autoincremented=true;")

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

    Season *season() const;

    QList<VideoDownloadLink *> downloadLinks() const;
    QList<VideoDownloadLink *> downloadLinks(const QString &formatDescription, const QString &mirror) const;
    void addDownloadLink(VideoDownloadLink *link);
    void removeDownloadLink(VideoDownloadLink *link);

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

private:
    friend class Season;
    void setId(int id);
    void setSeason(Season *season);
    void setDownloadLinks(const QList<VideoDownloadLink *> &links);

    int m_id;
    int m_number;
    int m_seasonNumber;
    QList<VideoDownloadLink *> m_downloadLinks;
    QString m_serienJunkiesTitle;
    Season *m_season;
    QString m_videoFile;
    QString m_title;
    QString m_overview;
    QLocale::Language m_primaryLanguage;
};

#endif // EPISODE_H
