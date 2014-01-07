#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>

class HeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HeaderView(QWidget *parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;

    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

    QStringList sortOptions() const;
    void setSortOptions(const QStringList &sortOptions);

    QString currentSortOption() const;
    void setCurrentSortOption(const QString &sortOption);

    Qt::SortOrder currentSortOrder() const;
    void setCurrentSortOrder(Qt::SortOrder sortOrder);

signals:
    void currentSortOptionChanged(QString);
    void currentSortOrderChanged(Qt::SortOrder);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const Q_DECL_OVERRIDE;

private:
    mutable QRect m_textRect;
    bool m_hover;
    bool m_menuVisible;

    QString m_currentSortOption;
    QStringList m_sortOptions;
    Qt::SortOrder m_currentSortOrder;
};

#endif // HEADERVIEW_H
