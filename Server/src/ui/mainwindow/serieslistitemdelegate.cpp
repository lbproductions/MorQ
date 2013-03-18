#include "serieslistitemdelegate.h"

#include "model/serieslistmodel.h"

#include <QPainter>

static const int HEIGHT = 64;

// Text
static const QPoint TITLE_OFFSET(12,12);
static const QPoint TITLE_OFFSET_SHADOW(0, -1); // relative to title offset
static const QPoint NEXT_EPISODE_OFFSET(12,32);
static const QPoint SEASON_EPISODE_COUNT_OFFSET(-12,12);
static const QColor COLOR_TITLE_NORMAL(75,77,80);
static const QColor COLOR_TITLE_NORMAL_SHADOW(222,222,222);
static const QColor COLOR_TITLE_SELECTED(231,240,249);
static const QColor COLOR_TITLE_SELECTED_SHADOW(45,130,201);
static const QColor COLOR_TEXT_NORMAL(148,149,151);
static const QColor COLOR_TEXT_SELECTED(231,240,249);
static const bool TITLE_BOLD = true;

// Background
static const QColor COLOR_LINE_TOP1_NORMAL(255,255,255,0);
static const QColor COLOR_LINE_TOP2_NORMAL(255,255,255,0);
static const QColor COLOR_LINE_BOTTOM_NORMAL(231,231,231);
static const QColor COLOR_GRADIENT_TOP_NORMAL(255,255,255,0);
static const QColor COLOR_GRADIENT_BOTTOM_NORMAL(255,255,255,0);

static const QColor COLOR_LINE_TOP1_SELECTED(79,154,221);
static const QColor COLOR_LINE_TOP2_SELECTED(138,185,231);
static const QColor COLOR_LINE_BOTTOM_SELECTED(34,101,157);
static const QColor COLOR_GRADIENT_TOP_SELECTED(111,171,226);
static const QColor COLOR_GRADIENT_BOTTOM_SELECTED(37,125,199);

SeriesListItemDelegate::SeriesListItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void SeriesListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static const QPixmap NOISY_TEXTURE(":/general/noisy-texture-200x200");
    QColor COLOR_LINE_TOP1 = COLOR_LINE_TOP1_NORMAL;
    QColor COLOR_LINE_TOP2 = COLOR_LINE_TOP2_NORMAL;
    QColor COLOR_LINE_BOTTOM = COLOR_LINE_BOTTOM_NORMAL;
    QColor COLOR_GRADIENT_TOP = COLOR_GRADIENT_TOP_NORMAL;
    QColor COLOR_GRADIENT_BOTTOM = COLOR_GRADIENT_BOTTOM_NORMAL;

    QColor COLOR_TITLE = COLOR_TITLE_NORMAL;
    QColor COLOR_TITLE_SHADOW = COLOR_TITLE_NORMAL_SHADOW;

    QColor COLOR_TEXT = COLOR_TEXT_NORMAL;

    if(option.state & QStyle::State_Selected) {
        COLOR_LINE_TOP1 = COLOR_LINE_TOP1_SELECTED;
        COLOR_LINE_TOP2 = COLOR_LINE_TOP2_SELECTED;
        COLOR_LINE_BOTTOM = COLOR_LINE_BOTTOM_SELECTED;
        COLOR_GRADIENT_TOP = COLOR_GRADIENT_TOP_SELECTED;
        COLOR_GRADIENT_BOTTOM = COLOR_GRADIENT_BOTTOM_SELECTED;

        COLOR_TITLE = COLOR_TITLE_SELECTED;
        COLOR_TITLE_SHADOW = COLOR_TITLE_SELECTED_SHADOW;

        COLOR_TEXT = COLOR_TEXT_SELECTED;
    }

    painter->save();

    // Top lines
    QRect topLineRect = option.rect;
    topLineRect.adjust(0,-1, 0, -1);
    painter->setPen(COLOR_LINE_TOP1);
    painter->drawLine(topLineRect.topLeft(), topLineRect.topRight());

    painter->setPen(COLOR_LINE_TOP2);
    painter->drawLine(option.rect.topLeft(), option.rect.topRight());

    // Bottom line
    painter->setPen(COLOR_LINE_BOTTOM);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    // Gradient
    QRect gradientRect = option.rect;
    gradientRect.adjust(0,1,0,-1);

    QLinearGradient gradient(option.rect.topLeft(), option.rect.bottomLeft());
    gradient.setColorAt(0, COLOR_GRADIENT_TOP);
    gradient.setColorAt(1, COLOR_GRADIENT_BOTTOM);

    QBrush gradientBrush(gradient);
    painter->setBrush(gradientBrush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(gradientRect);

    painter->restore();

    painter->drawTiledPixmap(option.rect, NOISY_TEXTURE);

    // Text
    painter->save();
    QFont font = painter->font();
    font.setBold(TITLE_BOLD);
    painter->setFont(font);

    // Shadow title
    painter->setPen(COLOR_TITLE_SHADOW);
    painter->drawText(option.rect.adjusted(TITLE_OFFSET.x() + TITLE_OFFSET_SHADOW.x(),
                                           TITLE_OFFSET.y() + TITLE_OFFSET_SHADOW.y(),0,0),
                      index.data().toString());

    // Foreground title
    QRect titleBoundingRect;
    painter->setPen(COLOR_TITLE);
    painter->drawText(option.rect.adjusted(TITLE_OFFSET.x(), TITLE_OFFSET.y(),0,0),
                      Qt::AlignLeft,
                      index.data().toString(),
                      &titleBoundingRect);

    // Next episode
    painter->setPen(COLOR_TEXT);
    font.setBold(false);
    painter->setFont(font);
    painter->drawText(option.rect.adjusted(NEXT_EPISODE_OFFSET.x(), NEXT_EPISODE_OFFSET.y(),0,0),
                      tr("Next episode in %1 days").arg("?"));

    // Season and episode count
    painter->drawText(option.rect.adjusted(SEASON_EPISODE_COUNT_OFFSET.x(), SEASON_EPISODE_COUNT_OFFSET.y(),
                                           SEASON_EPISODE_COUNT_OFFSET.x(), SEASON_EPISODE_COUNT_OFFSET.y()),
                      Qt::AlignRight,
                      tr("%1 seasons, %2 episodes")
                      .arg(index.data(SeriesListModel::SeasonCountRole).toInt())
                      .arg(index.data(SeriesListModel::EpisodeCountRole).toInt()));

    painter->restore();

}

QSize SeriesListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(HEIGHT);
    return size;
}
