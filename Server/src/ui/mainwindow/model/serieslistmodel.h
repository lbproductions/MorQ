#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include "objectlistmodel.h"

#include "model/series.h"

class SeriesListModel : public QpAbstractObjectListModel<Series>
{
    Q_OBJECT
public:
    enum Role {
        EpisodeCountRole = Qt::UserRole + 1,
        SeasonCountRole,
        RawDataRole
    };

    explicit SeriesListModel(QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

    bool isCheckable() const;
    void setCheckable(bool isCheckable);

    QList<QSharedPointer<Series> > seriesByCheckState(Qt::CheckState state) const;
    QSharedPointer<Series> checkedSeries() const;

private:
    bool m_checkable;
    QModelIndex m_lastCheckedIndex;
};

#endif // SERIESLISTMODEL_H
