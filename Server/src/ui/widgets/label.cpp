#include "label.h"

#include <QStyleOption>

Label::Label(QWidget *parent) :
    QLabel(parent),
    m_shadowPalette(palette())
{
    setShadowOffset(0,1);
}

void Label::paintEvent(QPaintEvent *e)
{
    QMargins originalMargins = contentsMargins();
    QPalette originalPalette = palette();

    // Paint shadow first
    setPalette(m_shadowPalette);
    setContentsMargins(m_shadowMargins);
    QLabel::paintEvent(e);

    // Then paint normal QLabel
    setPalette(originalPalette);
    setContentsMargins(originalMargins);
    QLabel::paintEvent(e);
}

void Label::setShadorColor(const QColor &shadorColor)
{
    m_shadowPalette.setColor(QPalette::Foreground, shadorColor);
}

void Label::setShadowOffset(int dx, int dy)
{
    m_shadowMargins = contentsMargins();
    m_shadowMargins.setTop(m_shadowMargins.top() + dy);
    m_shadowMargins.setLeft(m_shadowMargins.bottom() + dx);
}

void Label::setForegroundColor(const QColor &foregroundColor)
{
    QPalette originalPalette = palette();
    originalPalette.setColor(QPalette::Foreground, foregroundColor);
    setPalette(originalPalette);
}



