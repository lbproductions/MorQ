#ifndef SEASONSLISTMODEL_H
#define SEASONSLISTMODEL_H

#include "objectlistmodel.h"
#include "abstractobjectlistmodel.h"

#include "model/season.h"

class Series;

class SeasonsListModel : public QpAbstractObjectListModel<Season>
{
    Q_OBJECT
public:
    explicit SeasonsListModel(QObject *parent = 0);

    void setSeries(QSharedPointer<Series> series);

    int columnCount(const QModelIndex &) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Series> m_series;
};

#endif // SEASONSLISTMODEL_H
