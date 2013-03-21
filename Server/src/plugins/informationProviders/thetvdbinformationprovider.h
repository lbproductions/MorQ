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
    void scrapeSeries(Series *series) const Q_DECL_OVERRIDE;
    void scrapeEpisode(Episode *episode) Q_DECL_OVERRIDE;

private slots:
    void parseGetSeriesReply();
    void parseEpisodeReply();

private:
    void parseSeries(QXmlStreamReader &xml);
    void parseEpisode(QXmlStreamReader &xml);

    Episode *m_currentEpisode;
};

#endif // THETVDBINFORMATIONPROVIDER_H
