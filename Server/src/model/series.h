#ifndef SERIES_H
#define SERIES_H

#include <QObject>

#include <QPersistence.h>
#include <QPersistenceRelations.h>

#include <QUrl>
#include <QDate>
#include <QStringList>

class Season;
class Episode;

class Series : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QUrl serienJunkiesUrl READ serienJunkiesUrl WRITE setSerienJunkiesUrl)
    Q_PROPERTY(QList<QSharedPointer<Season> > seasons READ seasons WRITE setSeasons)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QSet<int> additionalLanguages READ additionalLanguages WRITE setAdditionalLanguages)

    Q_PROPERTY(int tvdbId READ tvdbId WRITE setTvdbId)
    Q_PROPERTY(QString imdbId READ imdbId WRITE setImdbId)
    Q_PROPERTY(QString overview READ overview WRITE setOverview)
    Q_PROPERTY(QDate firstAired READ firstAired WRITE setFirstAired)
    Q_PROPERTY(QStringList genres READ genres WRITE setGenres)
    Q_PROPERTY(QStringList actors READ actors WRITE setActors)
    Q_PROPERTY(QStringList bannerUrls READ bannerUrls WRITE setBannerUrls)
    Q_PROPERTY(QStringList posterUrls READ posterUrls WRITE setPosterUrls)

    Q_PROPERTY(QStringList folders READ folders WRITE setFolders)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:seasons",
                "reverserelation=series;")

public:
    static QSharedPointer<Series> forTitle(const QString &title);

    explicit Series(QObject *parent = 0);
    ~Series();
    
    // General
    int id() const;

    QList<QSharedPointer<Season> > seasons(QLocale::Language language = QLocale::AnyLanguage) const;
    QSharedPointer<Season> season(int number, QLocale::Language language = QLocale::AnyLanguage) const;
    void removeSeason(QSharedPointer<Season> season);
    void addSeason(QSharedPointer<Season> season);

    QList<QSharedPointer<Episode> > episodes() const;

    QLocale::Language primaryLanguage() const;
    void setPrimaryLanguage(QLocale::Language language);

    QSet<QLocale::Language> languages() const;
    void addLanguage(QLocale::Language language);

    QPixmap primaryLanguageFlag() const;

    static QPixmap languageFlag(QLocale::Language language);
    static QString tvdbLanguage(QLocale::Language language);

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

    // Internally used
    Qt::CheckState checkState() const;
    void setCheckState(const Qt::CheckState &checkState);

    QStringList folders() const;
    void setFolders(const QStringList &folders);
    void addFolder(const QString &folder);

signals:
    void checkStateChanged(Qt::CheckState oldState, Qt::CheckState newState);

private:
    void setSeasons(const QList<QSharedPointer<Season> > &seasons);
    void setAdditionalLanguages(const QSet<int> &additionalLanguages);
    QSet<int> additionalLanguages() const;
    QMultiMap<int, QSharedPointer<Season> > seasonsByNumber() const;

    QLocale::Language m_primaryLanguage;
    int m_tvdbId;
    QString m_title;
    QUrl m_serienJunkiesUrl;
    QpStrongRelation<Season> m_seasons;
    mutable QMultiMap<int, QSharedPointer<Season> > m_seasonsByNumber;
    QString m_imdbId;
    QString m_overview;
    QDate m_firstAired;
    QStringList m_genres;
    QStringList m_actors;
    QStringList m_bannerUrls;
    QStringList m_posterUrls;
    Qt::CheckState m_checkState;
    QSet<int> m_additionalLanguages;
    QStringList m_folders;
};

Q_DECLARE_METATYPE(QSet<int>)

#endif // SERIES_H
