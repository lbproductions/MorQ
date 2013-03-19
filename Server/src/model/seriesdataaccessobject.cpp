#include "seriesdataaccessobject.h"

#include <QPersistencePersistentDataAccessObject.h>

SeriesDataAccessObject::SeriesDataAccessObject(QObject *parent) :
    QPersistenceCachedDataAccessObject<Series>(new QPersistencePersistentDataAccessObject<Series>(), parent)
{
}

Series *SeriesDataAccessObject::byTitle(const QString &title)
{
    foreach(Series *series, readAll()) {
        if(series->title() == title)
            return series;
    }

    return nullptr;
}
