#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = 0);

    void paintEvent(QPaintEvent *e);

    void setShadorColor(const QColor &shadorColor);
    void setShadowOffset(int dx, int dy);

    void setForegroundColor(const QColor &foregroundColor);

private:
    QPalette m_shadowPalette;
    QMargins m_shadowMargins;
};

#endif // LABEL_H
