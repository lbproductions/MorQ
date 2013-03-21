#include "seriessearchresultitemdelegate.h"

static const QPoint TITLE_OFFSET(8,8);

SeriesSearchResultItemDelegate::SeriesSearchResultItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void SeriesSearchResultItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    drawTitleText(painter, option,
                  index.data().toString(),
                  TITLE_OFFSET);
}

QSize SeriesSearchResultItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(32);
    return size;
}
