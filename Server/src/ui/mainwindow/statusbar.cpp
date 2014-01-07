#include "statusbar.h"

#include "ui/widgets/label.h"
#include <QMovie>

StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent),
    m_messageLabelLeft(new Label(this)),
    m_messageLabelRight(new Label(this)),
    m_workingIndicator(new QLabel(this)),
    m_workingGif (new QMovie(":/statusBar/loader"))
{
    m_workingIndicator->setMovie(m_workingGif);

    m_messageLabelLeft->setForegroundColor(QColor(68,68,68));
    m_messageLabelLeft->setShadorColor(QColor(217,217,220));

    m_messageLabelRight->setForegroundColor(QColor(68,68,68));
    m_messageLabelRight->setShadorColor(QColor(217,217,220));

    QWidget *spacer = new QWidget(this);
    spacer->setFixedWidth(3);
    addWidget(spacer);

    addWidget(m_workingIndicator);
    addWidget(m_messageLabelLeft);
    addPermanentWidget(m_messageLabelRight);
}

StatusBar::~StatusBar()
{
    delete m_workingGif;
}

void StatusBar::setLeftMessage(const QString &message)
{
    m_messageLabelLeft->setVisible(!message.isEmpty());
    m_messageLabelLeft->setText(message);
}

void StatusBar::setRightMessage(const QString &message)
{
    m_messageLabelRight->setVisible(!message.isEmpty());
    m_messageLabelRight->setText(message);
}

void StatusBar::setWorking(bool working)
{
    if (working) {
        m_workingGif->start();
    }
    else {
        m_workingGif->stop();
    }
    m_workingIndicator->setVisible(working);
}
