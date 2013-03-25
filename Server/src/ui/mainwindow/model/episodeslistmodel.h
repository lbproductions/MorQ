#ifndef EPISODESLISTMODEL_H
#define EPISODESLISTMODEL_H

#include "objectlistmodel.h"

#include "model/episode.h"

class Season;

class EpisodesListModel : public ObjectListModel<Episode>
{
    Q_OBJECT
public:
    explicit EpisodesListModel(QObject *parent = 0);

    enum Role{
        TitleRole = Qt::UserRole + 1,
        VideoPathRole,
        DownloadLinkRole
    };
    
    void setSeason(Season *season);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QList<Episode *> objects() const Q_DECL_OVERRIDE;

private:
    Season *m_season;
};

#endif // EPISODESLISTMODEL_H
