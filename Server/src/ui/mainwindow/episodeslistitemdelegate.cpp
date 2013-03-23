#include "episodeslistitemdelegate.h"

#include "model/episodeslistmodel.h"

#include <QPainter>

static const QPoint TITLE_OFFSET(8,8);
static const int HEIGHT = 128;

EpisodesListItemDelegate::EpisodesListItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void EpisodesListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QColor original = COLOR_TITLE_NORMAL;
    if(index.data(EpisodesListModel::VideoPathRole).toString() == ""){
        COLOR_TITLE_NORMAL = QColor(Qt::gray);
    }

    drawTitleText(painter, option,
                  index.data().toString(),
                  TITLE_OFFSET);

    drawText(painter, option,
             tr("%1")
             .arg(index.data(EpisodesListModel::TitleRole).toString()),
             QPoint(12,32),
             Qt::TextWordWrap
             );

    COLOR_TITLE_NORMAL = original;
}

QSize EpisodesListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(HEIGHT);
    return size;
}
