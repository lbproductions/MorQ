#ifndef EPISODESLISTMODEL_H
#define EPISODESLISTMODEL_H

#include "../../lib/QPersistence/src/objectlistmodel.h"
#include "../../lib/QPersistence/src/sortfilterproxyobjectmodel.h"

#include "model/episode.h"

class Season;

class EpisodesListModel : public QpObjectListModel<Episode>
{
    Q_OBJECT
public:
    explicit EpisodesListModel(QObject *parent = 0);

    enum Role{
        TitleRole = Qt::UserRole + 1,
        VideoPathRole,
        DownloadLinkRole,
        RawDataRole
    };
    
    void setSeason(QSharedPointer<Season> season);

    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

class EpisodesSortFilterProxyModel : public QpSortFilterProxyObjectModel<Episode>
{
    Q_OBJECT
public:
    enum SortRole {
        Number
    };

    explicit EpisodesSortFilterProxyModel(EpisodesListModel *sourceModel, QObject *parent = 0);

    QStringList sortRoles() const Q_DECL_OVERRIDE;

protected:
    bool lessThan(QSharedPointer<Episode> left, QSharedPointer<Episode> right) const Q_DECL_OVERRIDE;
};

#endif // EPISODESLISTMODEL_H
