#ifndef RESCANCOLLECTIONDIALOG_H
#define RESCANCOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class RescanCollectionDialog;
}

class FileScraper;
class Series;
class InformationProviderPlugin;
template<class T>
class QPersistenceSimpleDataAccessObject;
class SeriesListModel;
class QItemSelection;

class RescanCollectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RescanCollectionDialog(QWidget *parent = 0);
    ~RescanCollectionDialog();
    
    void scan();

private slots:
    void checkForNewSeries();
    void confirmNextNewSeries();

    void search();
    void displaySearchResults();
    void showSelectedSeries();

    void saveTvdbResult();

    void accept();

private:
    FileScraper *m_scraper;
    Ui::RescanCollectionDialog *ui;

    Series *m_currentSeries;
    QList<Series *> m_newSeries;
    InformationProviderPlugin *m_provider;

    QPushButton *m_pushButtonRemoveSeries;
    QPushButton *m_pushButtonNextSeries;
    QPersistenceSimpleDataAccessObject<Series> *m_seriesDao;
    SeriesListModel *m_seriesListModel;
};

#endif // RESCANCOLLECTIONDIALOG_H
