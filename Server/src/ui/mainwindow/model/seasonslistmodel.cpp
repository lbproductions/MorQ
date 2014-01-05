#include "seasonslistmodel.h"

#include "model/series.h"

SeasonsListModel::SeasonsListModel(QObject *parent) :
    QpAbstractObjectListModel<Season>(parent),
    m_series(nullptr)
{
}

int SeasonsListModel::columnCount(const QModelIndex &) const
{
    return 1;
}

void SeasonsListModel::setSeries(QSharedPointer<Series> series)
{
    if(!series)
        setObjects(QList<QSharedPointer<Season> >());
    else
        setObjects(series->seasons());
}


QVariant SeasonsListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Season> season = objectByIndex(index);
    switch(role) {
    case Qt::DisplayRole:
        return season->title();
    case RawDataRole:
        return QVariant::fromValue<QSharedPointer<Season> >(season);
    }

    return QVariant();
}
