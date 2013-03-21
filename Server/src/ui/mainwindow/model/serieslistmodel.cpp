#include "serieslistmodel.h"

#include "controller/controller.h"

SeriesListModel::SeriesListModel(QPersistenceAbstractDataAccessObject *dao, QObject *parent) :
    ObjectListModel<Series>(parent),
    m_dao(dao)
{
    connect(m_dao, &SeriesDAO::objectInserted,
            this, &SeriesListModel::objectInserted);

    connect(m_dao, &SeriesDAO::objectUpdated,
            this, &SeriesListModel::objectUpdated);

    connect(m_dao, &SeriesDAO::objectRemoved,
            this, &SeriesListModel::objectRemoved);
}

QVariant SeriesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Series *series = objectByIndex(index);
    switch(role) {
    case Qt::DisplayRole:
        return series->title();
    case EpisodeCountRole:
        return series->episodes().size();
    case SeasonCountRole:
        return series->seasons().size();
    }

    return QVariant();
}

QList<Series *> SeriesListModel::objects() const
{
    return QPersistence::castList<Series>(m_dao->readAllObjects());
}
