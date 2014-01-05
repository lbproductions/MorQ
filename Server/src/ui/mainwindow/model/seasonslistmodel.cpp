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


SeasonSortFilterProxyModel::SeasonSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

SeasonsListModel *SeasonSortFilterProxyModel::sourceModel() const
{
    return static_cast<SeasonsListModel *>(QSortFilterProxyModel::sourceModel());
}

QSharedPointer<Season> SeasonSortFilterProxyModel::objectByIndex(const QModelIndex &index) const
{
    QModelIndex i = mapToSource(index);
    return sourceModel()->objectByIndex(i);
}

bool SeasonSortFilterProxyModel::filterAcceptsRow(int /*source_row*/, const QModelIndex &/*source_parent*/) const
{
    return true;
}

bool SeasonSortFilterProxyModel::filterAcceptsColumn(int /*source_column*/, const QModelIndex &/*source_parent*/) const
{
    return true;
}

bool SeasonSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QSharedPointer<Season> s1 = sourceModel()->objectByIndex(left);
    QSharedPointer<Season> s2 = sourceModel()->objectByIndex(right);

    return s1->number() > s2->number();
}
