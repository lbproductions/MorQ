#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>
#include "../../lib/QPersistence/src/sortfilterproxyobjectmodel.h"

class HeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HeaderView(QWidget *parent = 0);

    QSize sizeHint() const Q_DECL_OVERRIDE;

    void leaveEvent(QEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

    QpSortFilterProxyObjectModelBase *sortModel() const;
    void setSortModel(QpSortFilterProxyObjectModelBase *sortModel);

protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

private:
    mutable QRect m_textRect;
    bool m_hover;
    bool m_menuVisible;
    QpSortFilterProxyObjectModelBase *m_sortModel;
};

#endif // HEADERVIEW_H
