#ifndef SEASONSLISTMODEL_H
#define SEASONSLISTMODEL_H

#include <QSortFilterProxyModel>

#include "objectlistmodel.h"
#include "abstractobjectlistmodel.h"

#include "model/season.h"

class Series;

class SeasonsListModel : public QpAbstractObjectListModel<Season>
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

class SeasonSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SeasonSortFilterProxyModel(QObject *parent = 0);

    SeasonsListModel *sourceModel() const;

    QSharedPointer<Season> objectByIndex(const QModelIndex &index) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // SEASONSLISTMODEL_H
