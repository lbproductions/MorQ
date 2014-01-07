#ifndef EPISODESLISTMODEL_H
#define EPISODESLISTMODEL_H

#include "../../lib/QPersistence/src/objectlistmodel.h"

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

#endif // EPISODESLISTMODEL_H
