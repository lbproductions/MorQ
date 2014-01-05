#ifndef NEWSERIESSCRAPER_H
#define NEWSERIESSCRAPER_H

#include "scraper.h"

class Episode;

class NewSeriesScraper : public Scraper
{
public:
    NewSeriesScraper(QString title, QObject *parent = 0);

    void scan();

    QList<QSharedPointer<Series> > newSeries();

    QList<QSharedPointer<Episode> > newEpisodes();

private:
    QString m_title;
};

#endif // NEWSERIESSCRAPER_H
