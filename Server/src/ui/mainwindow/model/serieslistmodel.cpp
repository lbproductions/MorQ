#include "serieslistmodel.h"

#include "controller/controller.h"

SeriesListModel::SeriesListModel(QPersistenceAbstractDataAccessObject *dao, QObject *parent) :
    ObjectListModel<Series>(parent),
    m_dao(dao),
    m_checkable(false)
{
    connect(m_dao, &SeriesDAO::objectInserted,
            this, &SeriesListModel::objectInserted);

    connect(m_dao, &SeriesDAO::objectUpdated,
            this, &SeriesListModel::objectUpdated);

    connect(m_dao, &SeriesDAO::objectRemoved,
            this, &SeriesListModel::objectRemoved);
}

Qt::ItemFlags SeriesListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = ObjectListModel<Series>::flags(index);

    if(isCheckable())
        f = f | Qt::ItemIsTristate;

    return f;
}

QVariant SeriesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Series *series = objectByIndex(index);
    switch(role) {
    case Qt::DisplayRole:
        return series->title();
    case Qt::CheckStateRole:
        return series->checkState();
    case EpisodeCountRole:
        return series->episodes().size();
    case SeasonCountRole:
        return series->seasons().size();
    }

    return QVariant();
}

bool SeriesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::CheckStateRole) {
        Series *series = objectByIndex(index);
        Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());

        if(checkState == Qt::Checked) {
            if(m_lastCheckedIndex.isValid()) {
                Series *lastCheckedSeries = objectByIndex(m_lastCheckedIndex);

                if(lastCheckedSeries && lastCheckedSeries != series && lastCheckedSeries->checkState() == Qt::Checked) {
                    lastCheckedSeries->setCheckState(Qt::PartiallyChecked);
                    emit dataChanged(m_lastCheckedIndex, m_lastCheckedIndex);
                }
            }

            m_lastCheckedIndex = index;
        }

        series->setCheckState(checkState);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QList<Series *> SeriesListModel::objects() const
{
    return QPersistence::castList<Series>(m_dao->readAllObjects());
}

bool SeriesListModel::isCheckable() const
{
    return m_checkable;
}

void SeriesListModel::setCheckable(bool checkable)
{
    m_checkable = checkable;
}
