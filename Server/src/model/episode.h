#ifndef EPISODE_H
#define EPISODE_H

#include <QObject>

#include <QPersistence.h>
#include <QStringList>

class VideoDownloadLink;
class Season;

class Episode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString serienJunkiesTitle READ serienJunkiesTitle WRITE setSerienJunkiesTitle)
    Q_PROPERTY(Season* season READ season WRITE setSeason)
    Q_PROPERTY(QList<VideoDownloadLink*> downloadLinks READ downloadLinks WRITE setDownloadLinks)
    Q_PROPERTY(QStringList videoFiles READ videoFiles WRITE setVideoFiles)
    Q_PROPERTY(QString title READ title WRITE setTitle)

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
    void addDownloadLink(VideoDownloadLink *link);
    void removeDownloadLink(VideoDownloadLink *link);

    QStringList videoFiles() const;
    void addVideoFile(const QString &fileName);

    // SerienJunkies
    QString serienJunkiesTitle() const;
    void setSerienJunkiesTitle(const QString &title);

    // TheTVDB
    QString title() const;
    void setTitle(const QString &title);

private:
    friend class Season;
    void setId(int id);
    void setSeason(Season *season);
    void setDownloadLinks(const QList<VideoDownloadLink *> &links);
    void setVideoFiles(const QStringList &files);

    int m_id;
    int m_number;
    int m_seasonNumber;
    QString m_serienJunkiesTitle;
    Season *m_season;
    QList<VideoDownloadLink *> m_downloadLinks;
    QStringList m_videoFiles;
    QString m_title;
};

#endif // EPISODE_H
