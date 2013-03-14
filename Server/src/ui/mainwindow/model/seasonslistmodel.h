#ifndef SEASONSLISTMODEL_H
#define SEASONSLISTMODEL_H

#include "objectlistmodel.h"

#include "model/season.h"

class Series;

class SeasonsListModel : public ObjectListModel<Season>
{
    Q_OBJECT
public:
    explicit SeasonsListModel(QObject *parent = 0);

    void setSeries(Series *series);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QList<Season *> objects() const Q_DECL_OVERRIDE;

private:
    Series *m_series;
};

#endif // SEASONSLISTMODEL_H
