#ifndef SEASONSLISTMODEL_H
#define SEASONSLISTMODEL_H

#include <QSortFilterProxyModel>

#include "../../lib/QPersistence/src/objectlistmodel.h"
#include "../../lib/QPersistence/src/sortfilterproxyobjectmodel.h"

#include "model/season.h"

class Series;

class SeasonsListModel : public QpObjectListModel<Season>
{
    Q_OBJECT
public:
    enum Role {
        RawDataRole = Qt::UserRole + 1
    };

    explicit SeasonsListModel(QObject *parent = 0);

    void setSeries(QSharedPointer<Series> series);

    int columnCount(const QModelIndex &) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    QSharedPointer<Series> m_series;
};

class SeasonSortFilterProxyModel : public QpSortFilterProxyObjectModel<Season>
{
    Q_OBJECT
public:
    enum SortRole {
        Number,
        EpisodeCount
    };

    explicit SeasonSortFilterProxyModel(SeasonsListModel *sourceModel, QObject *parent = 0);

    QStringList sortRoles() const Q_DECL_OVERRIDE;

protected:
    bool lessThan(QSharedPointer<Season> left, QSharedPointer<Season> right) const Q_DECL_OVERRIDE;
};

#endif // SEASONSLISTMODEL_H
