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
    QColor original = COLOR_TITLE_NORMAL;
    QColor originalShadow = COLOR_TITLE_NORMAL_SHADOW;
    QColor originalGradientTop = COLOR_GRADIENT_TOP_NORMAL;
    QColor originalGradientBottom = COLOR_GRADIENT_BOTTOM_NORMAL;

    if(index.data(EpisodesListModel::VideoPathRole).toString() == ""){
        COLOR_TITLE_NORMAL = QColor(Qt::gray);
        if(index.data(EpisodesListModel::DownloadLinkRole).toString() != ""){
            COLOR_GRADIENT_TOP_NORMAL = QColor(230,230,230);
            COLOR_GRADIENT_BOTTOM_NORMAL = QColor(200,200,200);
        }
    }

    NoisyGradientItemDelegate::paint(painter, option, index);

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
    COLOR_TITLE_NORMAL_SHADOW = originalShadow;
    COLOR_GRADIENT_BOTTOM_NORMAL = originalGradientBottom;
    COLOR_GRADIENT_TOP_NORMAL = originalGradientTop;
}

QSize EpisodesListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = NoisyGradientItemDelegate::sizeHint(option, index);
    size.setHeight(HEIGHT);
    return size;
}
