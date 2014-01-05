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

QList<QSharedPointer<Series> > NewSeriesScraper::newSeries()
{
    QSharedPointer<Series> series = Series::forTitle(m_title);

    if(!series) {
        series = Qp::create<Series>();
        series->setTitle(m_title);
        Qp::update(series);
    }

    return QList<QSharedPointer<Series> >() << series;
}

QList<QSharedPointer<Episode> > NewSeriesScraper::newEpisodes()
{
    return QList<QSharedPointer<Episode> >();
}
