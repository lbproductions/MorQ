//#ifndef NEWSERIESWIZARD_H
//#define NEWSERIESWIZARD_H

//#include <QWizard>

//#include <QTimer>

//#include "plugins/downloadProviders/downloadprovider.h"

//namespace Ui {
//class NewSeriesWizard;
//}

//class QCompleter;
//class DownloadProvider;

//class NewSeriesWizard : public QWizard
//{
//    Q_OBJECT
    
//public:
//    explicit NewSeriesWizard(QWidget *parent = 0);
//    ~NewSeriesWizard();

//    QString seriesTitle();

//private slots:
//    void on_comboBoxProvider_currentIndexChanged(int index);

//    void on_lineEditName_textEdited(const QString &text);

//    void complete(QList<DownloadProvider::SeriesData> series);
//    void timeout();

//    void finish();

//    void on_comboBoxLocation_currentIndexChanged(const QString &arg1);

//private:
//    Ui::NewSeriesWizard *ui;

//    DownloadProvider *m_currentDownloadProvider;
//    DownloadProvider::SeriesData m_currentSeries;
//    QCompleter *m_completer;
//    int m_currentSearchId;
//    QList<DownloadProvider::SeriesData> m_mostRecentResults;
//    QTimer m_timer;

//    void setSeries(DownloadProvider::SeriesData series);
//};

//#endif // NEWSERIESWIZARD_H
