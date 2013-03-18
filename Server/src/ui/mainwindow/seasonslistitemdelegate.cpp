#include "seasonslistitemdelegate.h"

static const QPoint TITLE_OFFSET(8,8);

SeasonsListItemDelegate::SeasonsListItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void SeasonsListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    drawTitleText(painter, option,
                  index.data().toString(),
                  TITLE_OFFSET);
}

QSize SeasonsListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(32);
    return size;
}
