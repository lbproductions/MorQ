#ifndef CHOOSEDOWNLOADLINKSDIALOG_H
#define CHOOSEDOWNLOADLINKSDIALOG_H

#include <QDialog>

namespace Ui {
class ChooseDownloadLinksDialog;
}

class Episode;

class ChooseDownloadLinksDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChooseDownloadLinksDialog(QWidget *parent = 0);
    ~ChooseDownloadLinksDialog();
    
    void setEpisodes(QList<Episode *> episodes);

private slots:
    void addMirrorsBasedOnSelectedFormat();

private:
    Ui::ChooseDownloadLinksDialog *ui;

    QList<Episode *> m_episodes;
};

#endif // CHOOSEDOWNLOADLINKSDIALOG_H
