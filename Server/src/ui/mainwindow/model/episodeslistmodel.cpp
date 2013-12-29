#include "episodeslistmodel.h"

#include "model/season.h"

EpisodesListModel::EpisodesListModel(QObject *parent) :
    QpAbstractObjectListModel<Episode>(parent)
{
}

void EpisodesListModel::setSeason(QSharedPointer<Season> season)
{
    setObjects(season->episodes());
}

int EpisodesListModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant EpisodesListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    QSharedPointer<Episode> episode = objectByIndex(index);

    QString links = "";
    if(episode->downloadLinks().size() > 0) {
        links = "Downloads available";
    }

    switch(role) {
    case Qt::DisplayRole:
        return QString("E%1 - %2")
                .arg(episode->number(),2,10,QChar('0'))
                .arg(episode->title());
    case VideoPathRole:
        return QString("%1")
                .arg(episode->videoFile());
    case TitleRole:
        return QString("%1")
                .arg(episode->overview());
    case DownloadLinkRole:
        return QString("%1")
                .arg(links);
    }

    return QVariant();
}
