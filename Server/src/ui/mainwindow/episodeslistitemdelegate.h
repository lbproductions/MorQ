#ifndef EPISODESLISTITEMDELEGATE_H
#define EPISODESLISTITEMDELEGATE_H

#include "noisygradientitemdelegate.h"

class EpisodesListItemDelegate : public NoisyGradientItemDelegate
{
    Q_OBJECT
public:
    explicit EpisodesListItemDelegate(QAbstractItemView *view, QObject *parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
};

#endif // EPISODESLISTITEMDELEGATE_H
