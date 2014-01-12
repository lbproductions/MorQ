#include "seasonslistitemdelegate.h"

#include "model/seasonslistmodel.h"
#include "model/series.h"

static const QPoint STATUS_ICON_OFFSET(8,9);
static const QPoint TITLE_OFFSET(28,8);
static const QPoint FLAG_OFFSET(10,3); // relative to title ending
static const int FLAG_MARGIN = 2; // Gap between flags

SeasonsListItemDelegate::SeasonsListItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void SeasonsListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QRectF boundingRect = drawTitleText(painter, option,
                                        index.data().toString(),
                                        TITLE_OFFSET);

    QSharedPointer<Season> season = index.data(SeasonsListModel::RawDataRole).value<QSharedPointer<Season> >();

    drawPixmap(painter, option, season->statusPixmap(), STATUS_ICON_OFFSET);

    QPoint flagOffset = boundingRect.topRight().toPoint() - option.rect.topLeft() + FLAG_OFFSET;
    foreach (QLocale::Language language, season->languages()) {
        QPixmap pm = Series::languageFlag(language);
        drawPixmap(painter, option,
                   pm,
                   flagOffset);
        flagOffset.setX(flagOffset.x() + pm.width() + FLAG_MARGIN);
    }

    drawText(painter, option,
             season->statusMessage(),
             TITLE_OFFSET,
             Qt::AlignRight);
}

QSize SeasonsListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 14);
    return size;
}
