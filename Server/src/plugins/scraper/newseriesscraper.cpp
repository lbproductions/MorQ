#include "newseriesscraper.h"

#include <model/series.h>
#include <model/episode.h>

#include <controller/controller.h>

NewSeriesScraper::NewSeriesScraper(QString title, QObject *parent):
    Scraper(parent),
    m_title(title)
{
}

void NewSeriesScraper::scan()
{
    emit finished();
}

QList<Series *> NewSeriesScraper::newSeries()
{
    Series *series = Controller::seriesDao()->byTitle(m_title);

    if(!series) {
        series = Qp::create<Series>();
        series->setTitle(m_title);
        Qp::insert(series);
    }

    return QList<Series *>() << series;
}

QList<Episode *> NewSeriesScraper::newEpisodes()
{
    return QList<Episode *>();
}
