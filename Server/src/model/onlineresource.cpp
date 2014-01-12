#include "onlineresource.h"

#include "episode.h"
#include "season.h"

#include <QDebug>

OnlineResource::OnlineResource(QObject *parent) :
    QObject(parent),
    m_episode("episode", this),
    m_season("season", this)
{
}

OnlineResource::~OnlineResource()
{
}

QString OnlineResource::name() const
{
    return m_name;
}

void OnlineResource::setName(const QString &name)
{
    m_name  = name;
}

QString OnlineResource::formatDescription() const
{
    return m_formatDescription;
}

void OnlineResource::setFormatDescription(const QString &description)
{
    m_formatDescription  = description;
}

QUrl OnlineResource::url() const
{
    return m_url;
}

void OnlineResource::setUrl(const QUrl &url)
{
    m_url  = url;
}

QString OnlineResource::mirror() const
{
    return m_mirror;
}

QSharedPointer<Episode> OnlineResource::episode() const
{
    return m_episode.resolve();
}

void OnlineResource::setEpisode(QSharedPointer<Episode> episode)
{
    m_episode.relate(episode);
}

QSharedPointer<Season> OnlineResource::season() const
{
    return m_season.resolve();
}

QList<QLocale::Language> OnlineResource::languages() const
{
    return Qp::castList<QLocale::Language>(languages());
}

void OnlineResource::setLanguages(const QList<QLocale::Language> &languages)
{
    _setLanguages(Qp::castList<int>(languages));
}

void OnlineResource::setSeason(QSharedPointer<Season> season)
{
    m_season.relate(season);
}
QList<int> OnlineResource::_languages() const
{
    return m_languages;
}

void OnlineResource::_setLanguages(const QList<int> &languages)
{
    m_languages = languages;
}


void OnlineResource::setMirror(const QString &mirror)
{
    m_mirror = mirror;
}

QString OnlineResource::extractFolder() const
{
    return m_extractFolder;
}

void OnlineResource::setExtractFolder(const QString &extractFolder)
{
    m_extractFolder = extractFolder;
}
