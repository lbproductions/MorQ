#include "seriessearchresultitemdelegate.h"

#include <QPixmap>
#include <QDebug>

static const QPoint TITLE_OFFSET(52,8);
static const QPoint CHECKBOX_OFFSET(8,8);
static const QPoint FLAG_OFFSET(32,11);

SeriesSearchResultItemDelegate::SeriesSearchResultItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
    setCheckBoxOffset(CHECKBOX_OFFSET);
}

void SeriesSearchResultItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    drawCheckBox(painter, option, index);
    drawPixmap(painter, option, index.data(Qt::DecorationRole).value<QPixmap>(), FLAG_OFFSET);

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
