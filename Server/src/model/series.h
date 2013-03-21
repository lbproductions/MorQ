#ifndef SERIES_H
#define SERIES_H

#include <QObject>

#include <QPersistence.h>

#include <QUrl>
#include <QDate>
#include <QStringList>

class Season;
class Episode;

class Series : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QUrl serienJunkiesUrl READ serienJunkiesUrl WRITE setSerienJunkiesUrl)
    Q_PROPERTY(QList<Season*> seasons READ seasons WRITE setSeasons)

    Q_PROPERTY(int tvdbId READ tvdbId WRITE setTvdbId)
    Q_PROPERTY(QString imdbId READ imdbId WRITE setImdbId)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QDate firstAired READ firstAired WRITE setFirstAired)
    Q_PROPERTY(QStringList genres READ genres WRITE setGenres)
    Q_PROPERTY(QStringList actors READ actors WRITE setActors)
    Q_PROPERTY(QStringList bannerUrls READ bannerUrls WRITE setBannerUrls)
    Q_PROPERTY(QStringList posterUrls READ posterUrls WRITE setPosterUrls)

    Q_CLASSINFO(QPERSISTENCE_PRIMARYKEY, "id")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:id",
                "autoincremented=true;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:seasons",
                "reverserelation=series;")

public:
    explicit Series(QObject *parent = 0);
    ~Series();
    
    // General
    int id() const;

    QList<Season *> seasons() const;
    Season *season(int number) const;
    void removeSeason(Season *season);
    void addSeason(Season *season);

    QList<Episode *> episodes() const;

    // SerienJunkies
    QUrl serienJunkiesUrl() const;
    void setSerienJunkiesUrl(const QUrl &serienJunkiesUrl);

    // TheTVDB
    int tvdbId() const;
    void setTvdbId(int id);
    QUrl tvdbUrl() const;

    QString title() const;
    void setTitle(const QString &title);

    QString imdbId() const;
    void setImdbId(const QString &id);
    QUrl imdbUrl() const;

    QString overview() const;
    void setOverview(const QString &overview);

    QDate firstAired() const;
    void setFirstAired(const QDate &date);

    QStringList genres() const;
    void setGenres(const QStringList &genres);

    QStringList actors() const;
    void setActors(const QStringList &actors);

    QStringList bannerUrls() const;
    void setBannerUrls(const QStringList &urls);
    void addBannerUrl(const QString &banner);

    QStringList posterUrls() const;
    void setPosterUrls(const QStringList &urls);

private:
    void setId(int id);
    void setSeasons(const QList<Season *> &seasons);

    int m_id;
    int m_tvdbId;
    QString m_title;
    QUrl m_serienJunkiesUrl;
    QMap<int, Season *> m_seasons;
    QString m_imdbId;
    QString m_overview;
    QDate m_firstAired;
    QStringList m_genres;
    QStringList m_actors;
    QStringList m_bannerUrls;
    QStringList m_posterUrls;
};

#endif // SERIES_H
