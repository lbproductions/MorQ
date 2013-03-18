#ifndef SEASONSLISTITEMDELEGATE_H
#define SEASONSLISTITEMDELEGATE_H

#include "noisygradientitemdelegate.h"

class SeasonsListItemDelegate : public NoisyGradientItemDelegate
{
    Q_OBJECT
public:
    explicit SeasonsListItemDelegate(QAbstractItemView *view, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SEASONSLISTITEMDELEGATE_H
