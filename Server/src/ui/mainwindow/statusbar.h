#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class Label;
class QLabel;

class StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent = 0);
    ~StatusBar();

    void setMessage(const QString &message);
    void setWorking(bool working);

private:
    Label *m_messageLabel;
    QLabel *m_workingIndicator;
    QMovie *m_workingGif;
};

#endif // STATUSBAR_H
