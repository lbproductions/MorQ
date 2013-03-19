#ifndef PREFERENCESWINDOW_H
#define PREFERENCESWINDOW_H

#include <QMainWindow>

namespace Ui {
class PreferencesWindow;
}

class QItemSelection;

class PreferencesWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    enum Page {
        DownloadsPage,
        PremiumizeMePage
    };

    explicit PreferencesWindow(QWidget *parent = 0);
    ~PreferencesWindow();

    void setCurrentPage(Page page);

private slots:
    void on_lineEditUserName_editingFinished();

    void on_checkBoxEnableExtract_clicked();

    void on_actionPremuimizeMe_triggered();

    void on_actionDownloads_triggered();

    void on_lineEditPassword_editingFinished();

    void on_lineEditDownloadFolder_editingFinished();

    void on_lineEditExtractFolder_editingFinished();

    void on_pushButtonChooseDownloadFolder_clicked();

    void on_pushButtonChooseExtractFolder_clicked();

    void on_spinBoxMaxDownloads_editingFinished();

    void on_actionSeries_triggered();

    void on_pushButtonAddLocation_clicked();

    void on_pushButtonRemoveLocation_clicked();

    void enableRemoveLocationButtonUponSelection(const QItemSelection &selected, const QItemSelection &deselected);

private:
    Ui::PreferencesWindow *ui;

    void saveSeriesLocations();
};

#endif // PREFERENCESWINDOW_H
