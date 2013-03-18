#include "serieslistmodel.h"

#include "controller/controller.h"

SeriesListModel::SeriesListModel(QObject *parent) :
    ObjectListModel<Series>(parent)
{
    connect(Controller::seriesDao(), &SeriesDAO::objectInserted,
            this, &SeriesListModel::objectInserted);

    connect(Controller::seriesDao(), &SeriesDAO::objectUpdated,
            this, &SeriesListModel::objectUpdated);

    connect(Controller::seriesDao(), &SeriesDAO::objectRemoved,
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
    return QPersistence::readAll<Series>();
}
