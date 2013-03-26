#include "season.h"

#include "episode.h"
#include "series.h"

#include <QDebug>
#include <QPixmap>

Season::Season(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_number(0),
    m_series(nullptr),
    m_primaryLanguage(QLocale::AnyLanguage)
{
}

Season::~Season()
{
    qDebug() << "~Season(" << m_id << ")=" << this;

    if(m_series) {
        m_series->removeSeason(this);
    }

    foreach(Episode *episode, m_episodes.values()) {
        episode->setSeason(nullptr);
    }
}

int Season::id() const
{
    return m_id;
}

void Season::setId(int id)
{
    m_id = id;
}

int Season::number() const
{
    return m_number;
}

void Season::setNumber(int number)
{
    m_number = number;
}

QString Season::title() const
{
    if(!m_title.isEmpty())
        return m_title;

    if(m_number <= 0)
        return QString("Specials");

    static QHash<QLocale::Language, QString> translations;
    translations[QLocale::AnyLanguage] = QString("S%1");
    translations[QLocale::German] = QString("Staffel %1");
    translations[QLocale::English] = QString("Season %1");

    if(translations.contains(m_primaryLanguage))
        return translations.value(m_primaryLanguage).arg(m_number);

    return translations.value(QLocale::AnyLanguage)
            .arg(m_number).append(QString(" (%1)")
                                  .arg(QLocale::languageToString(m_primaryLanguage)));
}

void Season::setTitle(const QString title)
{
    m_title = title;
}

QString Season::serienJunkiesTitle() const
{
    return m_serienJunkiesTitle;
}

void Season::setSerienJunkiesTitle(const QString &title)
{
    m_serienJunkiesTitle = title;
}

QUrl Season::serienJunkiesUrl() const
{
    return m_serienJunkiesUrl;
}

void Season::setSerienJunkiesUrl(const QUrl &serienJunkiesUrl)
{
    m_serienJunkiesUrl = serienJunkiesUrl;
}

QLocale::Language Season::primaryLanguage() const
{
    return m_primaryLanguage;
}

void Season::setPrimaryLanguage(QLocale::Language language)
{
    m_primaryLanguage = language;
}

QPixmap Season::primaryLanguageFlag() const
{
    return Series::languageFlag(m_primaryLanguage);
}

Series *Season::series() const
{
    return m_series;
}

void Season::setSeries(Series *series)
{
    m_series = series;
}

QList<Episode *> Season::episodes() const
{
    return m_episodes.values();
}

Episode *Season::episode(int number) const
{
    return m_episodes.value(number);
}

void Season::addEpisode(Episode *episode)
{
    Q_ASSERT(!m_episodes.contains(episode->number()));

    episode->setSeason(this);
    m_episodes.insert(episode->number(), episode);
}

void Season::removeEpisode(Episode *episode)
{
    Q_ASSERT(m_episodes.contains(episode->number()));

    episode->setSeason(nullptr);
    m_episodes.remove(episode->number());
}

void Season::setEpisodes(const QList<Episode *> &episodes)
{
    m_episodes.clear();

    foreach(Episode *episode, episodes) {
        addEpisode(episode);
    }
}

QSet<QLocale::Language> Season::languages() const
{
    QSet<QLocale::Language> result;
    result.insert(m_primaryLanguage);
    foreach(Episode *episode, m_episodes) {
        foreach(QLocale::Language lang, episode->languages()) {
            result.insert(lang);
        }
    }
    return result;
}

QString Season::tvdbLanguage() const
{
    if(m_primaryLanguage == QLocale::AnyLanguage)
        return "en";

    QString lang = QLocale(m_primaryLanguage).name();
    return lang.left(lang.lastIndexOf('_'));
}

QStringList Season::folders() const
{
    return m_folders;
}

void Season::setFolders(const QStringList &folders)
{
    m_folders = folders;
}

void Season::addFolder(const QString &folder)
{
    m_folders.append(folder);
}
