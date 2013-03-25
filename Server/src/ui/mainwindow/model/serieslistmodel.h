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

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QList<Series *> objects() const Q_DECL_OVERRIDE;

    bool isCheckable() const;
    void setCheckable(bool isCheckable);

    QList<Series *> partiallyCheckedSeries() const;
    Series *checkedSeries() const;

private:
    QPersistenceAbstractDataAccessObject *m_dao;
    bool m_checkable;
    QModelIndex m_lastCheckedIndex;
};

#endif // SERIESLISTMODEL_H
