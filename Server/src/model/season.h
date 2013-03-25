#ifndef SEASON_H
#define SEASON_H

#include <QObject>

#include <QPersistence.h>

#include <QUrl>
#include <QLocale>

class Series;
class Episode;

class Season : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString serienJunkiesTitle READ serienJunkiesTitle WRITE setSerienJunkiesTitle)
    Q_PROPERTY(QUrl serienJunkiesUrl READ serienJunkiesUrl WRITE setSerienJunkiesUrl)
    Q_PROPERTY(Series* series READ series WRITE setSeries)
    Q_PROPERTY(QLocale::Language primaryLanguage READ primaryLanguage WRITE setPrimaryLanguage)
    Q_PROPERTY(QList<Episode*> episodes READ episodes WRITE setEpisodes)

    Q_CLASSINFO(QPERSISTENCE_PRIMARYKEY, "id")
    Q_CLASSINFO("QPERSISTENCE_PROPERTYMETADATA:id",
                "autoincremented=true;")
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

    Series *series() const;

    QList<Episode *> episodes() const;
    Episode *episode(int number) const;
    void addEpisode(Episode *episode);
    void removeEpisode(Episode *episode);

    QString tvdbLanguage() const;

private:
    friend class Series;
    void setId(int id);
    void setSeries(Series *series);
    void setEpisodes(const QList<Episode *> &episodes);

    int m_id;
    int m_number;
    QString m_serienJunkiesTitle;
    QString m_title;
    QUrl m_serienJunkiesUrl;
    Series *m_series;
    QMap<int, Episode *> m_episodes;
    QLocale::Language m_primaryLanguage;
};

#endif // SEASON_H
