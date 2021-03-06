#include "episode.h"

#include "series.h"
#include "season.h"
#include "onlineresource.h"
#include "misc/tools.h"
#include "downloadpackage.h"

#include <QDebug>
#include <QPixmap>

Episode::Episode(QObject *parent) :
    QObject(parent),
    m_number(-1),
    m_tvdbId(-1),
    m_tvdbSeasonId(-1),
    m_seasonNumber(-1),
    m_status(Episode::UnkownStatus),
    m_downloadLinks("downloadLinks", this),
    m_season("season", this),
    m_primaryLanguage(QLocale::AnyLanguage),
    m_downloadPackage("downloadPackage", this)
{
}

Episode::~Episode()
{
}

QString Episode::displayString() const
{
    if(number() < 0)
        return title();

    return QString("E%1 - %2")
            .arg(number(),2,10,QChar('0'))
            .arg(title());
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

QSharedPointer<Season> Episode::season() const
{
    return m_season.resolve();
}

void Episode::setSeason(QSharedPointer<Season> season)
{
    if (season)
        m_seasonNumber = season->number();

    m_season.relate(season);
}

QList<QSharedPointer<OnlineResource> > Episode::downloadLinks() const
{
    return m_downloadLinks.resolveList();
}

QList<QSharedPointer<OnlineResource> > Episode::downloadLinks(const QString &formatDescription, const QString &mirror) const
{
    QList<QSharedPointer<OnlineResource> > list;
    foreach (QSharedPointer<OnlineResource> link, downloadLinks()) {
        if (link->formatDescription() == formatDescription && link->mirror() == mirror) {
            list.append(link);
        }
    }
    return list;
}

void Episode::addDownloadLink(QSharedPointer<OnlineResource> link)
{
    link->setEpisode(Qp::sharedFrom(this));
    m_downloadLinks.relate(link);
}

void Episode::removeDownloadLink(QSharedPointer<OnlineResource> link)
{
    link->setEpisode(QSharedPointer<Episode>());
    m_downloadLinks.unrelate(link);
}

void Episode::setDownloadLinks(const QList<QSharedPointer<OnlineResource> > &links)
{
    m_downloadLinks.clear();
    m_downloadLinks.relate(links);
}

QDate Episode::firstAired() const
{
    return m_firstAired;
}

void Episode::setFirstAired(const QDate &firstAired)
{
    m_firstAired = firstAired;
}

QSharedPointer<DownloadPackage> Episode::downloadPackage() const
{
    return m_downloadPackage.resolve();
}

void Episode::setDownloadPackage(QSharedPointer<DownloadPackage> arg)
{
    arg->setEpisode(Qp::sharedFrom(this));
    m_downloadPackage.relate(arg);
}

int Episode::tvdbSeasonId() const
{
    return m_tvdbSeasonId;
}

void Episode::setTvdbSeasonId(int arg)
{
    if(season())
        season()->setTvdbId(arg);

    m_tvdbSeasonId = arg;
}

int Episode::tvdbId() const
{
    return m_tvdbId;
}

void Episode::setTvdbId(int arg)
{
    m_tvdbId = arg;
}

QUrl Episode::tvdbUrl() const
{
    if(tvdbId() <= 0)
        return QUrl();

    return QUrl(QString("http://thetvdb.com/?tab=episode&seriesid=%1&seasonid=%2&id=%3&lid=7")
                .arg(season()->series()->tvdbId())
                .arg(tvdbSeasonId())
                .arg(tvdbId()));
}

Episode::Status Episode::status() const
{
    return m_status;
}

void Episode::setStatus(Episode::Status status)
{
    m_status = status;
}

QPixmap Episode::statusPixmap() const
{
    switch (status()) {
    case DownloadAvailable:
        return Tools::cachedPixmap(":/icons/download_available");
    case Missing:
        return Tools::cachedPixmap(":/icons/missing");
    case Ok:
        return Tools::cachedPixmap(":/icons/okay");
    case Downloading:
        return Tools::cachedPixmap(":/icons/downloading");
    case Upcoming:
        return Tools::cachedPixmap(":/icons/upcoming");
    case Extracting:
    default:
    case UnkownStatus:
        break;
    }

    // TODO: Add missing episode status icons

    return Tools::cachedPixmap(":/icons/questionmark");
}

QString Episode::statusMessage() const
{
    switch (status()) {
    case DownloadAvailable:
        return tr("Download available");
    case Missing:
        return tr("Episode missing");
    case Ok:
        return tr("Okay");
    case Downloading:
        return tr("Download running...");
    case Extracting:
        return tr("Extracting...");
    case Upcoming:
        return Tools::airsInString(firstAired());

    default:
    case UnkownStatus:
        break;
    }
    return tr("Unknown status");
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
    if (m_primaryLanguage == QLocale::AnyLanguage)
        return "en";

    QString lang = QLocale(m_primaryLanguage).name();
    return lang.left(lang.lastIndexOf('_'));
}

QPixmap Episode::primaryLanguageFlag() const
{
    return Series::languageFlag(m_primaryLanguage);
}
