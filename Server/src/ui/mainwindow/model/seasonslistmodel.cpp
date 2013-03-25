#include "seasonslistmodel.h"

#include "model/series.h"

SeasonsListModel::SeasonsListModel(QObject *parent) :
    ObjectListModel<Season>(parent),
    m_series(nullptr)
{
}

void SeasonsListModel::setSeries(Series *series)
{
    beginResetModel();
    m_series = series;
    endResetModel();
}


QVariant SeasonsListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Season *season = objectByIndex(index);
    switch(role) {
    case Qt::DisplayRole:
        return season->title();
    }

    return QVariant();
}


QList<Season *> SeasonsListModel::objects() const
{
    if(!m_series)
        return QList<Season *>();

    return m_series->seasons();
}
