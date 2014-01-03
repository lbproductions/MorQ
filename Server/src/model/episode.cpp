#include "episode.h"

#include "series.h"
#include "season.h"
#include "videodownloadlink.h"

#include <QDebug>
#include <QPixmap>

Episode::Episode(QObject *parent) :
    QObject(parent),
    m_number(-1),
    m_seasonNumber(-1),
    m_downloadLinks("downloadLinks", this),
    m_season("season", this),
    m_primaryLanguage(QLocale::AnyLanguage)
{
}

Episode::~Episode()
{
}

int Episode::id() const
{
    return Qp::primaryKey(Qp::sharedFrom(this));
}

int Episode::number() const
{
    return m_number;
}

void Episode::setNumber(int number)
{
    m_number = number;
}

int Episode::seasonNumber() const
{
    return m_seasonNumber;
}

void Episode::setSeasonNumber(int number)
{
    m_seasonNumber = number;
}

QString Episode::serienJunkiesTitle() const
{
    return m_serienJunkiesTitle;
}

void Episode::setSerienJunkiesTitle(const QString &title)
{
    m_serienJunkiesTitle = title;
}

QSharedPointer<Season> Episode::season() const
{
    return m_season.resolve();
}

void Episode::setSeason(QSharedPointer<Season> season)
{
    if(season)
        m_seasonNumber = season->number();

    m_season.relate(season);
}

QList<QSharedPointer<VideoDownloadLink> > Episode::downloadLinks() const
{
    return QList<QSharedPointer<VideoDownloadLink> >();
    return m_downloadLinks.resolveList();
}

QList<QSharedPointer<VideoDownloadLink> > Episode::downloadLinks(const QString &formatDescription, const QString &mirror) const
{
    QList<QSharedPointer<VideoDownloadLink> > list;
    foreach(QSharedPointer<VideoDownloadLink> link, downloadLinks()) {
        if(link->formatDescription() == formatDescription && link->mirror() == mirror) {
            list.append(link);
        }
    }
    return list;
}

void Episode::addDownloadLink(QSharedPointer<VideoDownloadLink> link)
{
    link->setEpisode(Qp::sharedFrom(this));
    m_downloadLinks.relate(link);
}

void Episode::removeDownloadLink(QSharedPointer<VideoDownloadLink> link)
{
    link->setEpisode(QSharedPointer<Episode>());
    m_downloadLinks.unrelate(link);
}

void Episode::setDownloadLinks(const QList<QSharedPointer<VideoDownloadLink> > &links)
{
    m_downloadLinks.clear();
    m_downloadLinks.relate(links);
}

QString Episode::videoFile() const
{
    return m_videoFile;
}

void Episode::setVideoFile(const QString &fileName)
{
    m_videoFile = fileName;
}

QLocale::Language Episode::primaryLanguage() const
{
    return  m_primaryLanguage;
}

void Episode::setPrimaryLanguage(QLocale::Language language)
{
    m_primaryLanguage = language;
}

QString Episode::title() const
{
    return m_title;
}

void Episode::setTitle(const QString &title)
{
    m_title = title;
}

QString Episode::overview() const
{
    return m_overview;
}

void Episode::setOverview(const QString &overview)
{
    m_overview = overview;
}


QSet<QLocale::Language> Episode::languages() const
{
    QSet<QLocale::Language> result;
    result.insert(m_primaryLanguage);
    // TODO: Get languages from video file, or let them be set manually?
    return result;
}

QString Episode::tvdbLanguage() const
{
    if(m_primaryLanguage == QLocale::AnyLanguage)
        return "en";

    QString lang = QLocale(m_primaryLanguage).name();
    return lang.left(lang.lastIndexOf('_'));
}

QPixmap Episode::primaryLanguageFlag() const
{
    return Series::languageFlag(m_primaryLanguage);
}
