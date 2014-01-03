#ifndef THETVDBINFORMATIONPROVIDER_H
#define THETVDBINFORMATIONPROVIDER_H

#include "informationprovider.h"

#include <QtGlobal>

class QXmlStreamReader;

class TheTvdbInformationProvider : public InformationProvider
{
    Q_OBJECT
public:
    static const QString Name;

    InformationProviderTask *searchSeries(const QString &title, QSharedPointer<Series> series = QSharedPointer<Series>()) Q_DECL_OVERRIDE;
    InformationProviderTask *scrapeSeries(QSharedPointer<Series> series) Q_DECL_OVERRIDE;
    InformationProviderTask *scrapeEpisode(QSharedPointer<Episode> episode) Q_DECL_OVERRIDE;
    void saveSeriesResult(QSharedPointer<Series> source, QSharedPointer<Series> target) const Q_DECL_OVERRIDE;
    void saveEpisodeResult(QSharedPointer<Episode> source, QSharedPointer<Episode> target) const;

private:
    friend class Plugins;
    explicit TheTvdbInformationProvider(QObject *parent = 0);
};


class TheTvdbInformationProviderTask : public InformationProviderTask
{
    Q_OBJECT
private slots:
    void parseSearchSeriesReply();
    void parseScrapeEpisodeReply();
    void parseScrapeSeriesReply();

    void handleNetworkError();

private:
    friend class TheTvdbInformationProvider;
    TheTvdbInformationProviderTask(TheTvdbInformationProvider *parent);

    void searchSeries(const QString &title);
    void scrapeSeries(QSharedPointer<Series> series);
    void scrapeEpisode(QSharedPointer<Episode> episode);

    static void parseSeries(QXmlStreamReader &xml, QSharedPointer<Series> series);
    static void parseEpisode(QXmlStreamReader &xml, QSharedPointer<Episode> episode);

    void decreaseActiveRequestCountAndMaybeEmitFinished();

    int m_activeRequestsCount;

    QSet<int> m_tvdbIds;
};






#endif // THETVDBINFORMATIONPROVIDER_H
