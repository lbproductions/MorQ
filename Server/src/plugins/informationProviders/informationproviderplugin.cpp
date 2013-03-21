#include "informationproviderplugin.h"

InformationProviderPlugin::InformationProviderPlugin(QObject *parent) :
    QObject(parent)
{
}

QList<Series *> InformationProviderPlugin::results() const
{
    return m_results;
}

void InformationProviderPlugin::addResult(Series *series)
{
    m_results.append(series);
}

void InformationProviderPlugin::addResults(const QList<Series *> &series)
{
    m_results.append(series);
}
