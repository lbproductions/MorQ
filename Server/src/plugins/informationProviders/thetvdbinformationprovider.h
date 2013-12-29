#ifndef THETVDBINFORMATIONPROVIDER_H
#define THETVDBINFORMATIONPROVIDER_H

#include "informationproviderplugin.h"

#include <QtGlobal>

class QXmlStreamReader;

class TheTvdbInformationProvider : public InformationProviderPlugin
{
    Q_OBJECT
public:
    explicit TheTvdbInformationProvider(QObject *parent = 0);

    void searchSeries(const QString &title) const Q_DECL_OVERRIDE;
    void copySeries(QSharedPointer<Series> source, QSharedPointer<Series> target) const Q_DECL_OVERRIDE;
    void copyEpisode(QSharedPointer<Episode> source, QSharedPointer<Episode> target) const Q_DECL_OVERRIDE;
    void scrapeSeries(QSharedPointer<Series> series) const Q_DECL_OVERRIDE;
    void scrapeSeriesIncludingEpisodes(QSharedPointer<Series> series) const Q_DECL_OVERRIDE;
    void scrapeEpisode(QSharedPointer<Episode> episode) const Q_DECL_OVERRIDE;

private slots:
    void parseSearchSeriesReply();
    void parseScrapeEpisodeReply();
    void parseScrapeSeriesReply();

private:
    static void parseSeries(QXmlStreamReader &xml, QSharedPointer<Series> series);
    static void parseEpisode(QXmlStreamReader &xml, QSharedPointer<Episode> episode);

    mutable int m_activeRequestsCount;

    void decreaseActiveRequestCountAndMaybeEmitFinished();
};

#endif // THETVDBINFORMATIONPROVIDER_H
