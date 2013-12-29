#ifndef SEASON_H
#define SEASON_H

#include <QObject>

#include <QPersistence.h>
#include <QPersistenceRelations.h>

#include <QUrl>
#include <QLocale>

class Series;
class Episode;

class Season : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString serienJunkiesTitle READ serienJunkiesTitle WRITE setSerienJunkiesTitle)
    Q_PROPERTY(QUrl serienJunkiesUrl READ serienJunkiesUrl WRITE setSerienJunkiesUrl)
    Q_PROPERTY(QSharedPointer<Series>  series READ series WRITE setSeries)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QList<QSharedPointer<Episode> > episodes READ episodes WRITE setEpisodes)
    Q_PROPERTY(QStringList folders READ folders WRITE setFolders)

    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:series",
                "reverserelation=seasons;")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:episodes",
                "reverserelation=season;")

public:
    explicit Season(QObject *parent = 0);
    ~Season();

    int id() const;

    int number() const;
    void setNumber(int number);

    QString title() const;
    void setTitle(const QString title);

    QString serienJunkiesTitle() const;
    void setSerienJunkiesTitle(const QString &title);

    QUrl serienJunkiesUrl() const;
    void setSerienJunkiesUrl(const QUrl &serienJunkiesUrl);

    QLocale::Language primaryLanguage() const;
    void setPrimaryLanguage(QLocale::Language language);

    QPixmap primaryLanguageFlag() const;

    QSet<QLocale::Language> languages() const;

    QSharedPointer<Series> series() const;

    QList<QSharedPointer<Episode> > episodes() const;
    QSharedPointer<Episode> episode(int number) const;
    void addEpisode(QSharedPointer<Episode> episode);
    void removeEpisode(QSharedPointer<Episode> episode);

    QString tvdbLanguage() const;

    QStringList folders() const;
    void setFolders(const QStringList &folders);
    void addFolder(const QString &folder);

private:
    friend class Series;
    void setSeries(QSharedPointer<Series> series);
    void setEpisodes(const QList<QSharedPointer<Episode> > &episodes);

    int m_number;
    QString m_serienJunkiesTitle;
    QString m_title;
    QUrl m_serienJunkiesUrl;
    QpWeakRelation<Series> m_series;
    QpStrongRelation<Episode> m_episodes;
    mutable QMap<int, QSharedPointer<Episode> > m_episodesByNumber;
    QLocale::Language m_primaryLanguage;
    QStringList m_folders;
};

#endif // SEASON_H
