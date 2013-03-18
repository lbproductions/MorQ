#include "noisygradientitemdelegate.h"

#include <QPainter>
#include <QAbstractItemView>

// Text
const QPoint NoisyGradientItemDelegate::TITLE_OFFSET_SHADOW(0, -1); // relative to title offset
const QColor NoisyGradientItemDelegate::COLOR_TITLE_NORMAL(75,77,80);
const QColor NoisyGradientItemDelegate::COLOR_TITLE_NORMAL_SHADOW(222,222,222);
const QColor NoisyGradientItemDelegate::COLOR_TITLE_SELECTED(231,240,249);
const QColor NoisyGradientItemDelegate::COLOR_TITLE_SELECTED_SHADOW(45,130,201);
const QColor NoisyGradientItemDelegate::COLOR_TEXT_NORMAL(148,149,151);
const QColor NoisyGradientItemDelegate::COLOR_TEXT_SELECTED(231,240,249);
const bool NoisyGradientItemDelegate::TITLE_BOLD = true;

// Background
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP1_NORMAL(255,255,255,0);
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP2_NORMAL(255,255,255,0);
const QColor NoisyGradientItemDelegate::COLOR_LINE_BOTTOM_NORMAL(231,231,231);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_TOP_NORMAL(255,255,255,0);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_BOTTOM_NORMAL(255,255,255,0);

const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP1_SELECTED(79,154,221);
const QColor NoisyGradientItemDelegate::COLOR_LINE_TOP2_SELECTED(138,185,231);
const QColor NoisyGradientItemDelegate::COLOR_LINE_BOTTOM_SELECTED(34,101,157);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_TOP_SELECTED(111,171,226);
const QColor NoisyGradientItemDelegate::COLOR_GRADIENT_BOTTOM_SELECTED(37,125,199);

NoisyGradientItemDelegate::NoisyGradientItemDelegate(QAbstractItemView *view, QObject *parent) :
    QStyledItemDelegate(parent),
    m_view(view)
{
    connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &NoisyGradientItemDelegate::repaintItemsWhenSelectionChanges);
}

void NoisyGradientItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

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
}


void NoisyGradientItemDelegate::drawTitleText(QPainter *painter,
                                              const QStyleOptionViewItem &option,
                                              const QString &text,
                                              const QPoint &offset) const
{

    QColor COLOR_TITLE = COLOR_TITLE_NORMAL;
    QColor COLOR_TITLE_SHADOW = COLOR_TITLE_NORMAL_SHADOW;

    if(option.state & QStyle::State_Selected) {
        COLOR_TITLE = COLOR_TITLE_SELECTED;
        COLOR_TITLE_SHADOW = COLOR_TITLE_SELECTED_SHADOW;
    }

    painter->save();

    QFont font = painter->font();
    font.setBold(TITLE_BOLD);
    painter->setFont(font);

    // Shadow title
    painter->setPen(COLOR_TITLE_SHADOW);
    painter->drawText(option.rect.adjusted(offset.x() + TITLE_OFFSET_SHADOW.x(),
                                           offset.y() + TITLE_OFFSET_SHADOW.y(),
                                           offset.x() + TITLE_OFFSET_SHADOW.x(),
                                           offset.y() + TITLE_OFFSET_SHADOW.y()),
                      text);

    // Foreground title
    QRect titleBoundingRect;
    painter->setPen(COLOR_TITLE);
    painter->drawText(option.rect.adjusted(offset.x(), offset.y(),
                                           offset.x(), offset.y()),
                      Qt::AlignLeft,
                      text,
                      &titleBoundingRect);
    painter->restore();
}

void NoisyGradientItemDelegate::drawText(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QString &text,
                                         const QPoint &offset,
                                         Qt::Alignment alignment) const
{
    QColor COLOR_TEXT = COLOR_TEXT_NORMAL;

    if(option.state & QStyle::State_Selected) {
        COLOR_TEXT = COLOR_TEXT_SELECTED;
    }
    painter->save();

    painter->setPen(COLOR_TEXT);

    QFont font = painter->font();
    font.setBold(false);
    painter->setFont(font);
    painter->drawText(option.rect.adjusted(offset.x(), offset.y(),
                                           offset.x(), offset.y()),
                      alignment,
                      text);
    painter->restore();
}

void NoisyGradientItemDelegate::repaintItemsWhenSelectionChanges(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);

    foreach(QModelIndex index, deselected.indexes()) {
        if(index.row() == 0)
            continue;

        m_view->update(m_view->model()->index(index.row() - 1, 0));
    }
}
