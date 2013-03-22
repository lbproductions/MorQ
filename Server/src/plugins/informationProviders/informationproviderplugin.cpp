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

QList<Series *> InformationProviderPlugin::seriesSearchResults() const
{
    return m_results;
}

void InformationProviderPlugin::addResult(Series *series)
{
    series->setParent(this);
    m_results.append(series);
}

void InformationProviderPlugin::addResults(const QList<Series *> &series)
{
    foreach(Series *s, series) {
        s->setParent(this);
    }

    m_results.append(series);
}

void InformationProviderPlugin::addNewEpisode(Episode *episode)
{
    m_newEpisodes.append(episode);
}
