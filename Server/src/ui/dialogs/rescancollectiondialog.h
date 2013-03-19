#ifndef RESCANCOLLECTIONDIALOG_H
#define RESCANCOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class RescanCollectionDialog;
}

class FileScraper;

class RescanCollectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RescanCollectionDialog(QWidget *parent = 0);
    ~RescanCollectionDialog();
    
    void scan();

private slots:
    void setCurrentFolder(const QString &folder);

private:
    FileScraper *m_scraper;
    Ui::RescanCollectionDialog *ui;
};

#endif // RESCANCOLLECTIONDIALOG_H
