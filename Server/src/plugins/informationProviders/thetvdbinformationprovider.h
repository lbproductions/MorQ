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
    void copySeries(Series *source, Series *target) const Q_DECL_OVERRIDE;
    void copyEpisode(Episode *source, Episode *target) const Q_DECL_OVERRIDE;
    void scrapeSeries(Series *series) const Q_DECL_OVERRIDE;
    void scrapeSeriesIncludingEpisodes(Series *series) const Q_DECL_OVERRIDE;
    void scrapeEpisode(Episode *episode) const Q_DECL_OVERRIDE;

private slots:
    void parseSearchSeriesReply();
    void parseScrapeEpisodeReply();
    void parseScrapeSeriesReply();

private:
    static void parseSeries(QXmlStreamReader &xml, Series *series);
    static void parseEpisode(QXmlStreamReader &xml, Episode *episode);
};

#endif // THETVDBINFORMATIONPROVIDER_H
