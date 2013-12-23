#ifndef SERIESDATAACCESSOBJECT_H
#define SERIESDATAACCESSOBJECT_H

#include <QPersistenceCachedDataAccessObject.h>

#include "model/series.h"

class SeriesDataAccessObject : public QpCachedDataAccessObject<Series>
{
    Q_OBJECT
public:
    explicit SeriesDataAccessObject(QObject *parent = 0);

    Series *byTitle(const QString &title);
};

#endif // SERIESDATAACCESSOBJECT_H
