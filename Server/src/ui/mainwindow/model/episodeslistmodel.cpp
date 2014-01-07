#include "episodeslistmodel.h"

#include "model/season.h"

EpisodesListModel::EpisodesListModel(QObject *parent) :
    QpObjectListModel<Episode>(parent)
{
}

void EpisodesListModel::setSeason(QSharedPointer<Season> season)
{
    if (!season)
        setObjects(QList<QSharedPointer<Episode> >());
    else
        setObjects(season->episodes());
}

int EpisodesListModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant EpisodesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QSharedPointer<Episode> episode = objectByIndex(index);

    QString links = "";
    if (episode->downloadLinks().size() > 0) {
        links = "Downloads available";
    }

    switch (role) {
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
    case RawDataRole:
        return QVariant::fromValue<QSharedPointer<Episode> >(episode);
    }

    return QVariant();
}

EpisodesSortFilterProxyModel::EpisodesSortFilterProxyModel(EpisodesListModel *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Episode>(sourceModel, parent)
{
}

bool EpisodesSortFilterProxyModel::lessThan(QSharedPointer<Episode> left, QSharedPointer<Episode> right) const
{
    if (sortRole() == Number)
        return left->number() < right->number();

    return left < right;
}

QStringList EpisodesSortFilterProxyModel::sortRoles() const
{
    static QStringList roles = QStringList()
            << tr("Number");
    return roles;
}
