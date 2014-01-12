#include "serieslistitemdelegate.h"

#include "model/serieslistmodel.h"

#include <QPainter>

static const int HEIGHT = 64;
static const QPoint TITLE_OFFSET(12,12);
static const QPoint STATUS_ICON_OFFSET(12,33);
static const QPoint STATUSMESSAGE_OFFSET(32,32);
static const QPoint SEASON_EPISODE_COUNT_OFFSET(0,12);
static const QPoint FLAG_OFFSET(12,3); // relative to title ending
static const int FLAT_MARGIN = 2; // Gap between flags

SeriesListItemDelegate::SeriesListItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void SeriesListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Paint gradient
    NoisyGradientItemDelegate::paint(painter, option, index);

    drawTitleText(painter, option,
                  index.data().toString(),
                  TITLE_OFFSET);

    QSharedPointer<Series> series = index.data(SeriesListModel::RawDataRole).value<QSharedPointer<Series> >();

    drawPixmap(painter, option,
               series->statusPixmap(),
               STATUS_ICON_OFFSET);

    drawText(painter, option,
             series->statusMessage(),
             STATUSMESSAGE_OFFSET);
    drawText(painter, option,
             tr("%1 seasons, %2 episodes")
             .arg(index.data(SeriesListModel::SeasonCountRole).toInt())
             .arg(index.data(SeriesListModel::EpisodeCountRole).toInt()),
             SEASON_EPISODE_COUNT_OFFSET,
             Qt::AlignRight);

}

QSize SeriesListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(HEIGHT);
    return size;
}
