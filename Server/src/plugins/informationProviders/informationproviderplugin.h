#ifndef INFORMATIONPROVIDERPLUGIN_H
#define INFORMATIONPROVIDERPLUGIN_H

#include <QObject>

class Series;

class InformationProviderPlugin : public QObject
{
    Q_OBJECT
public:
    explicit InformationProviderPlugin(QObject *parent = 0);

    virtual void searchSeries(const QString &title) const = 0;
    QList<Series *> results() const;

    virtual void copySeries(Series *source, Series *target) const = 0;

    virtual void scrapeSeries(Series *series) const = 0;

signals:
    void finished();

protected:
    void addResult(Series *series);
    void addResults(const QList<Series *> &series);

private:
    QList<Series *> m_results;
};

#endif // INFORMATIONPROVIDERPLUGIN_H
