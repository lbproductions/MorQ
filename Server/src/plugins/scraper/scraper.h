#ifndef SCRAPER_H
#define SCRAPER_H

#include <QObject>

class Series;
class Episode;

class Scraper : public QObject
{
    Q_OBJECT
public:
    explicit Scraper(QObject *parent = 0);

    virtual void scan() = 0;

    virtual QList<Series*> newSeries() = 0;

    virtual QList<Episode*> newEpisodes() = 0;
    
signals:
    void finished();
    
public slots:
    
};

#endif // SCRAPER_H
