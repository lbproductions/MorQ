#ifndef INFORMATIONPROVIDERPLUGIN_H
#define INFORMATIONPROVIDERPLUGIN_H

#include <QObject>

class Series;
class Episode;

class InformationProviderPlugin : public QObject
{
    Q_OBJECT
public:
    explicit InformationProviderPlugin(QObject *parent = 0);
    virtual ~InformationProviderPlugin();

    virtual void searchSeries(const QString &title) const = 0;
    QList<QSharedPointer<Series> > seriesSearchResults() const;

    virtual void copySeries(QSharedPointer<Series> source, QSharedPointer<Series> target) const = 0;
    virtual void copyEpisode(QSharedPointer<Episode> source, QSharedPointer<Episode> target) const = 0;

    virtual void scrapeSeries(QSharedPointer<Series> series) const = 0;
    virtual void scrapeSeriesIncludingEpisodes(QSharedPointer<Series> series) const = 0;

    QList<QSharedPointer<Episode> > newEpisodes() const;

    virtual void scrapeEpisode(QSharedPointer<Episode> episode) const = 0;

signals:
    void finished();

protected:
    void addResult(QSharedPointer<Series> series);
    void addResults(const QList<QSharedPointer<Series> > &series);

    void addNewEpisode(QSharedPointer<Episode> episode);

private:
    QList<QSharedPointer<Series> > m_results;
    QList<QSharedPointer<Episode> >  m_newEpisodes;
};

#endif // INFORMATIONPROVIDERPLUGIN_H
