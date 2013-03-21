#include "episodeslistmodel.h"

#include "model/season.h"

EpisodesListModel::EpisodesListModel(QObject *parent) :
    ObjectListModel<Episode>(parent),
    m_season(nullptr)
{
}

void EpisodesListModel::setSeason(Season *season)
{
    beginResetModel();
    m_season = season;
    endResetModel();
}

QVariant EpisodesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Episode *episode = objectByIndex(index);

    switch(role) {
    case Qt::DisplayRole:
        return QString("E%1 - %2")
                .arg(episode->number(),2,10,QChar('0'))
                .arg(episode->title());
    }

    return QVariant();
}

QList<Episode *> EpisodesListModel::objects() const
{
    if(!m_season)
        return QList<Episode *>();

    return m_season->episodes();
}
