#ifndef NOISYGRADIENTITEMDELEGATE_H
#define NOISYGRADIENTITEMDELEGATE_H

#include <QStyledItemDelegate>

class QItemSelection;

class NoisyGradientItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    // Text
    static const QPoint TITLE_OFFSET_SHADOW;
    static QColor COLOR_TITLE_NORMAL;
    static QColor COLOR_TITLE_NORMAL_SHADOW;
    static const QColor COLOR_TITLE_SELECTED;
    static const QColor COLOR_TITLE_SELECTED_SHADOW;
    static const QColor COLOR_TEXT_NORMAL;
    static const QColor COLOR_TEXT_SELECTED;
    static const bool TITLE_BOLD;
    static const int TEXT_FOOT_DISTANCE;

    // Background
    static const QColor COLOR_LINE_TOP1_NORMAL;
    static const QColor COLOR_LINE_TOP2_NORMAL;
    static const QColor COLOR_LINE_BOTTOM_NORMAL;
    static QColor COLOR_GRADIENT_TOP_NORMAL;
    static QColor COLOR_GRADIENT_BOTTOM_NORMAL;

    static const QColor COLOR_LINE_TOP1_SELECTED;
    static const QColor COLOR_LINE_TOP2_SELECTED;
    static const QColor COLOR_LINE_BOTTOM_SELECTED;
    static const QColor COLOR_GRADIENT_TOP_SELECTED;
    static const QColor COLOR_GRADIENT_BOTTOM_SELECTED;

    explicit NoisyGradientItemDelegate(QAbstractItemView *view, QObject *parent = 0);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    QRectF drawTitleText(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QString &text,
                       const QPoint &offset) const;
    QRectF drawText(QPainter *painter,
                  const QStyleOptionViewItem &option,
                  const QString &text,
                  const QPoint &offset,
                  int alignment = Qt::AlignLeft | Qt::AlignTop) const;
    void drawCheckBox(QPainter *painter,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const;
    void drawPixmap(QPainter *painter,
                    const QStyleOptionViewItem &option,
                    const QPixmap &pixmap,
                    const QPoint &offset) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) Q_DECL_OVERRIDE;

protected:
    void setCheckBoxOffset(const QPoint &offset);

private slots:
    void repaintItemsWhenSelectionChanges(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QAbstractItemView *m_view;

    QPoint m_checkBoxOffset;
};

#endif // NOISYGRADIENTITEMDELEGATE_H
