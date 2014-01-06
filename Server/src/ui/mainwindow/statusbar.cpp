#include "statusbar.h"

#include "ui/widgets/label.h"
#include <QMovie>

StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent),
    m_messageLabel(new Label(this)),
    m_workingIndicator(new QLabel(this)),
    m_workingGif(new QMovie(":/statusBar/loader"))
{
    m_workingIndicator->setMovie(m_workingGif);

    m_messageLabel->setForegroundColor(QColor(68,68,68));
    m_messageLabel->setShadorColor(QColor(217,217,220));

    QWidget *spacer = new QWidget(this);
    spacer->setFixedWidth(3);
    addWidget(spacer);

    addWidget(m_workingIndicator);
    addWidget(m_messageLabel);
}

StatusBar::~StatusBar()
{
    delete m_workingGif;
}

void StatusBar::setMessage(const QString &message)
{
    m_messageLabel->setVisible(!message.isEmpty());
    m_messageLabel->setText(message);
}

void StatusBar::setWorking(bool working)
{
    if(working) {
        m_workingGif->start();
    }
    else {
        m_workingGif->stop();
    }
    m_workingIndicator->setVisible(working);
}
