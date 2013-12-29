#include "informationproviderplugin.h"

#include "model/series.h"

#include <QDebug>

InformationProviderPlugin::InformationProviderPlugin(QObject *parent) :
    QObject(parent)
{
}

InformationProviderPlugin::~InformationProviderPlugin()
{
}

QList<QSharedPointer<Series> > InformationProviderPlugin::seriesSearchResults() const
{
    return m_results;
}

void InformationProviderPlugin::addResult(QSharedPointer<Series> series)
{
    series->setParent(this);
    m_results.append(series);
}

void InformationProviderPlugin::addResults(const QList<QSharedPointer<Series> > &series)
{
    foreach(QSharedPointer<Series> s, series) {
        s->setParent(this);
    }

    m_results.append(series);
}

void InformationProviderPlugin::addNewEpisode(QSharedPointer<Episode> episode)
{
    m_newEpisodes.append(episode);
}
