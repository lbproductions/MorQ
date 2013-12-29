#ifndef EPISODESLISTMODEL_H
#define EPISODESLISTMODEL_H

#include "objectlistmodel.h"

#include "model/episode.h"

class Season;

class EpisodesListModel : public QpAbstractObjectListModel<Episode>
{
    Q_OBJECT
public:
    explicit EpisodesListModel(QObject *parent = 0);

    enum Role{
        TitleRole = Qt::UserRole + 1,
        VideoPathRole,
        DownloadLinkRole
    };
    
    void setSeason(QSharedPointer<Season> season);

    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // EPISODESLISTMODEL_H
