#ifndef INFORMATIONPROVIDERPLUGIN_H
#define INFORMATIONPROVIDERPLUGIN_H

#include <QObject>

#include "model/series.h"

#include "../plugins.h"

class InformationProviderTask;

class InformationProvider : public Plugin
{
    Q_OBJECT
public:
    explicit InformationProvider(const QString &name, QObject *parent = 0);
    virtual ~InformationProvider();

    virtual InformationProviderTask *searchSeries(const QString &title, QSharedPointer<Series> series = QSharedPointer<Series>()) = 0;
    virtual InformationProviderTask *scrapeSeries(QSharedPointer<Series> series) = 0;

    virtual void saveSeriesResult(QSharedPointer<Series> source, QSharedPointer<Series> target) const = 0;
};

class InformationProviderTask : public PluginTask
{
    Q_OBJECT
public:
    QList<QSharedPointer<Series> > resultSeries() const;

    QSharedPointer<Series> series() const;

    InformationProvider *provider() const;

protected:
    explicit InformationProviderTask(InformationProvider *parent);

    void setSeries(const QSharedPointer<Series> &series);

    void addResultSeries(QSharedPointer<Series> series);
    QList<QSharedPointer<Series> > m_resultSeries;
    QSharedPointer<Series> m_series;
};

#endif // INFORMATIONPROVIDERPLUGIN_H
