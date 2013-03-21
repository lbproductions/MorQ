#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include "objectlistmodel.h"

#include "model/series.h"

class SeriesListModel : public ObjectListModel<Series>
{
    Q_OBJECT
public:
    enum Role {
        EpisodeCountRole = Qt::UserRole + 1,
        SeasonCountRole
    };

    explicit SeriesListModel(QPersistenceAbstractDataAccessObject *dao, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QList<Series *> objects() const Q_DECL_OVERRIDE;

    void setDao();

private:
    QPersistenceAbstractDataAccessObject *m_dao;
};

#endif // SERIESLISTMODEL_H
