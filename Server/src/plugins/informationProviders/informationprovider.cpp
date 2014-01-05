#include "informationprovider.h"

#include "model/series.h"

#include <QDebug>

InformationProvider::InformationProvider(const QString &name, QObject *parent) :
    Plugin(name, parent)
{
}

InformationProvider::~InformationProvider()
{
}


InformationProviderTask::InformationProviderTask(InformationProvider *parent) :
    PluginTask(parent)
{
}


QList<QSharedPointer<Series> > InformationProviderTask::resultSeries() const
{
    return m_resultSeries;
}

void InformationProviderTask::addResultSeries(QSharedPointer<Series> series)
{
    m_resultSeries.append(series);
}

QSharedPointer<Series> InformationProviderTask::series() const
{
    return m_series;
}

InformationProvider *InformationProviderTask::provider() const
{
    return static_cast<InformationProvider *>(parent());
}

void InformationProviderTask::setSeries(const QSharedPointer<Series> &series)
{
    m_series = series;
}
