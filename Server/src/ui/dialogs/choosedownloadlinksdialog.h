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
    
    void setEpisodes(QList<QSharedPointer<Episode> > episodes);

private slots:
    void addMirrorsBasedOnSelectedFormat();

    void finish();

private:
    Ui::ChooseDownloadLinksDialog *ui;

    QList<QSharedPointer<Episode> > m_episodes;
};

#endif // CHOOSEDOWNLOADLINKSDIALOG_H
