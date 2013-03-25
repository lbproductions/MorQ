#include "seriessearchresultitemdelegate.h"

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

static const QPoint TITLE_OFFSET(32,8);
static const QPoint CHECKBOX_OFFSET(8,8);

static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options)
{
    QStyleOptionButton check_box_style_option;
    QRect check_box_rect = QApplication::style()->subElementRect(
                QStyle::SE_CheckBoxIndicator,
                &check_box_style_option);
    QPoint check_box_point(view_item_style_options.rect.x() + CHECKBOX_OFFSET.x(),
                           view_item_style_options.rect.y() + CHECKBOX_OFFSET.y());
    return QRect(check_box_point, check_box_rect.size());
}

SeriesSearchResultItemDelegate::SeriesSearchResultItemDelegate(QAbstractItemView *view, QObject *parent) :
    NoisyGradientItemDelegate(view, parent)
{
}

void SeriesSearchResultItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    NoisyGradientItemDelegate::paint(painter, option, index);

    QStyleOptionButton buttonOption;
    buttonOption.rect = CheckBoxRect(option);
    buttonOption.state = QStyle::State_Enabled | QStyle::State_Active;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
    if(checkState == Qt::PartiallyChecked) {
        buttonOption.state |= QStyle::State_NoChange; // represents "partially"
    }
    else if(checkState == Qt::Checked) {
        buttonOption.state |= QStyle::State_On;
    }
    else if(checkState == Qt::Unchecked) {
        buttonOption.state |= QStyle::State_Off;
    }

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &buttonOption, painter, NULL);

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

bool SeriesSearchResultItemDelegate::editorEvent(QEvent *event,
                                                 QAbstractItemModel *model,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    if ((event->type() == QEvent::MouseButtonRelease)
            || (event->type() == QEvent::MouseButtonDblClick)) {
        QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() != Qt::LeftButton ||
                !CheckBoxRect(option).contains(mouse_event->pos())) {
            return false;
        }
        if (event->type() == QEvent::MouseButtonDblClick) {
            return true;
        }
    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select) {
            return false;
        }
    } else {
        return false;
    }

    Qt::CheckState checkState = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
    if(checkState == Qt::PartiallyChecked) {
        checkState = Qt::Unchecked;
    }
    else if(checkState == Qt::Checked) {
        checkState = Qt::PartiallyChecked;
    }
    else if(checkState == Qt::Unchecked) {
        checkState = Qt::Checked;
    }
    return model->setData(index, checkState, Qt::CheckStateRole);
}
