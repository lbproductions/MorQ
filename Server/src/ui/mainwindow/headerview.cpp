#include "headerview.h"

#include "misc/tools.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>

static const QPoint TEXT_OFFSET(9,4);
static const QPoint ARROW_OFFSET(3,2); // relative to text top right
static const QPoint BORDER_LEFT_OFFSET(-7,-2); // relative to text top left
static const QPoint BORDER_RIGHT_OFFSET(8,-2); // relative to text top right

HeaderView::HeaderView(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent),
    m_hover(false),
    m_menuVisible(false)
{
}

QSize HeaderView::sizeHint() const
{
    QSize size = QHeaderView::sizeHint();
    return QSize(size.width(), 19);
}

void HeaderView::leaveEvent(QEvent *)
{
    bool hover = m_menuVisible;
    if (hover == m_hover)
        return;

    m_hover = hover;
    resizeSection(0, sectionSize(0) + 1);
    resizeSection(0, sectionSize(0) - 1);
}

void HeaderView::mouseMoveEvent(QMouseEvent *e)
{
    bool hover = m_menuVisible || m_textRect.adjusted(-4,-2,9,1).contains(e->pos());
    if (hover == m_hover)
        return;

    m_hover = hover;
    resizeSection(0, sectionSize(0) + 1);
    resizeSection(0, sectionSize(0) - 1);
}

void HeaderView::mousePressEvent(QMouseEvent *e)
{
    m_hover = m_textRect.adjusted(-4,-2,9,1).contains(e->pos());
    if (!m_hover)
        return;

    m_menuVisible = true;
    resizeSection(0, sectionSize(0) + 1);
    resizeSection(0, sectionSize(0) - 1);

    QMenu *menu = new QMenu(this);

    int roleCount = sortModel()->sortRoleCount();
    for (int role = 0; role < roleCount; ++role) {
        QString data = sortModel()->sortRoleTitle(role);
        QAction *action = menu->addAction(data);

        connect(action, &QAction::triggered, [=]{
            sortModel()->setSortRole(role);
            setSortIndicator(0, sortIndicatorOrder());
        });

        if (sortModel()->sortRole() == role) {
            action->setCheckable(true);
            action->setChecked(true);
        }
    }

    menu->addSeparator();

    QAction *actionAsc = menu->addAction(tr("Ascending"));
    QAction *actionDesc = menu->addAction(tr("Descending"));
    actionAsc->setCheckable(true);
    actionDesc->setCheckable(true);

    connect(actionAsc, &QAction::triggered, [=]{
        setSortIndicator(0, Qt::AscendingOrder);
    });
    connect(actionDesc, &QAction::triggered, [=]{
        setSortIndicator(0, Qt::DescendingOrder);
    });

    if (sortIndicatorOrder() == Qt::AscendingOrder)
        actionAsc->setChecked(true);
    else
        actionDesc->setChecked(true);

    menu->exec(mapToGlobal(m_textRect.bottomLeft()) + QPoint(-2,6));

    menu->deleteLater();

    m_menuVisible = false;
    m_hover = false;

    resizeSection(0, sectionSize(0) + 1);
    resizeSection(0, sectionSize(0) - 1);
}

void HeaderView::paintEvent(QPaintEvent *e)
{
    QRect rect = e->rect();
    QPainter painter(viewport());

    // Draw background
    painter.setBrush(QBrush(QColor(243,243,243)));
    painter.setPen(QPen(Qt::transparent));
    painter.drawRect(rect);
    painter.setPen(QPen(QColor(230,230,230)));
    painter.drawLine(rect.bottomLeft(), rect.bottomRight());

    QString text = tr("Sort by %1")
            .arg(sortModel()->sortRoleTitle(sortModel()->sortRole()));

    QFont f = painter.font();
    f.setFamily("Helvetica");
    f.setPixelSize(11);
    f.setBold(true);
    painter.setFont(f);

    if (m_hover) {
        QPixmap arrow = Tools::cachedPixmap(":/headerView/arrow_hover");
        QPixmap borderLeft = Tools::cachedPixmap(":/headerView/button_border_left");
        QPixmap borderRight = Tools::cachedPixmap(":/headerView/button_border_right");
        QPixmap background = Tools::cachedPixmap(":/headerView/button_background");

        if (m_menuVisible) {
            arrow  = Tools::cachedPixmap(":/headerView/arrow_pressed");
            borderLeft = Tools::cachedPixmap(":/headerView/button_border_left_pressed");
            borderRight = Tools::cachedPixmap(":/headerView/button_border_right_pressed");
            background = Tools::cachedPixmap(":/headerView/button_background_pressed");
        }

        // Left button border
        painter.drawPixmap(m_textRect.topLeft().x() + BORDER_LEFT_OFFSET.x(),
                            m_textRect.topLeft().y() + BORDER_LEFT_OFFSET.y(),
                            borderLeft);
        // Right button border
        painter.drawPixmap(m_textRect.topRight().x() + BORDER_RIGHT_OFFSET.x(),
                            m_textRect.topRight().y() + BORDER_RIGHT_OFFSET.y(),
                            borderRight);

        // Background
        painter.drawTiledPixmap(m_textRect.adjusted(-1,-2,7,1), background);

        // Text shadow
        painter.setPen(QPen(QColor(112,112,112)));
        painter.drawText(rect.adjusted(TEXT_OFFSET.x(), TEXT_OFFSET.y() + 1, 0,0),
                          Qt::AlignLeft,
                          text);

        // Text
        painter.setPen(QPen(QColor(Qt::white)));
        painter.drawText(rect.adjusted(TEXT_OFFSET.x(), TEXT_OFFSET.y(), 0,0),
                          Qt::AlignLeft,
                          text,
                          &m_textRect);

        // Arrow
        painter.drawPixmap(m_textRect.topRight().x() + ARROW_OFFSET.x(),
                            m_textRect.topRight().y() + ARROW_OFFSET.y(),
                            arrow);
    }
    else {
        QPixmap arrow = Tools::cachedPixmap(":/headerView/arrow_normal");
        painter.setPen(QPen(QColor(145,145,145)));
        painter.drawText(rect.adjusted(TEXT_OFFSET.x(), TEXT_OFFSET.y(), 0,0),
                          Qt::AlignLeft,
                          text,
                          &m_textRect);
        painter.drawPixmap(m_textRect.topRight().x() + ARROW_OFFSET.x(),
                            m_textRect.topRight().y() + ARROW_OFFSET.y(),
                            arrow);
    }
}

QpSortFilterProxyObjectModelBase *HeaderView::sortModel() const
{
    return m_sortModel;
}

void HeaderView::setSortModel(QpSortFilterProxyObjectModelBase *model)
{
    m_sortModel = model;
}
