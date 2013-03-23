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

    QString files = "";
    if(episode->videoFiles().size() > 0) {
        files = episode->videoFiles().first();
    }

    switch(role) {
    case Qt::DisplayRole:
        return QString("E%1 - %2")
                .arg(episode->number(),2,10,QChar('0'))
                .arg(episode->title());
    case VideoPathRole:
        return QString("%1")
                .arg(files);
    case TitleRole:
        return QString("%1")
                .arg(episode->overview());
    }

    return QVariant();
}

QList<Episode *> EpisodesListModel::objects() const
{
    if(!m_season)
        return QList<Episode *>();

    return m_season->episodes();
}
