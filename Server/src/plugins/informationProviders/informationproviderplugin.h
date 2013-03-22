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
    QList<Series *> seriesSearchResults() const;

    virtual void copySeries(Series *source, Series *target) const = 0;
    virtual void copyEpisode(Episode *source, Episode *target) const = 0;

    virtual void scrapeSeries(Series *series) const = 0;
    virtual void scrapeSeriesIncludingEpisodes(Series *series) const = 0;

    QList<Episode *> newEpisodes() const;

    virtual void scrapeEpisode(Episode *episode) const = 0;

signals:
    void finished();

protected:
    void addResult(Series *series);
    void addResults(const QList<Series *> &series);

    void addNewEpisode(Episode *episode);

private:
    QList<Series *> m_results;
    QList<Episode *>  m_newEpisodes;
};

#endif // INFORMATIONPROVIDERPLUGIN_H
