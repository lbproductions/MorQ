#ifndef SERIESLISTITEMDELEGATE_H
#define SERIESLISTITEMDELEGATE_H

#include "noisygradientitemdelegate.h"

class SeriesListItemDelegate : public NoisyGradientItemDelegate
{
    Q_OBJECT
public:
    explicit SeriesListItemDelegate(QAbstractItemView *view, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SERIESLISTITEMDELEGATE_H
