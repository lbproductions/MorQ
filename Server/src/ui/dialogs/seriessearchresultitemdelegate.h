#ifndef SERIESSEARCHRESULTITEMDELEGATE_H
#define SERIESSEARCHRESULTITEMDELEGATE_H

#include "ui/mainwindow/noisygradientitemdelegate.h"

class SeriesSearchResultItemDelegate : public NoisyGradientItemDelegate
{
    Q_OBJECT
public:
    explicit SeriesSearchResultItemDelegate(QAbstractItemView *view, QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};

#endif // SERIESSEARCHRESULTITEMDELEGATE_H
