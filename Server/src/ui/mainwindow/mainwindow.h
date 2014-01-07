#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMovie>

namespace Ui {
class MainWindow;
}

class QLabel;
class PreferencesWindow;
class DownloadsItemModel;
class SeriesListModel;
class SeasonsListModel;
class EpisodesListModel;
class SeasonSortFilterProxyModel;
class SeriesSortFilterProxyModel;
class HeaderView;
class EpisodesSortFilterProxyModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static MainWindow *instance();
    ~MainWindow();

private slots:
    void on_actionDownloads_triggered();
    void on_actionTV_Shows_triggered();

    void on_actionPreferences_triggered();
    void on_actionDownload_Preferences_triggered();
    void on_actionPremiumizeMe_Preferences_triggered();

    void on_actionStart_triggered();
    void on_actionDeleteDownload_triggered();
    void on_actionResetDownload_triggered();
    void on_actionStopDownloads_triggered();
    void on_actionExtract_triggered();

    void enableActionsAccordingToDownloadSelection();
    void enableActionsAccordingToDownloadStatus();

    void on_actionAdd_show_triggered();

    void showSeasonsForSelectedSeries();
    void showEpisodesForSelectedSeason();
    void enableActionsAccordingToSeriesSelection();

    void on_actionAddDownload_triggered();

    void on_actionRescan_collection_triggered();

    void on_actionShow_in_Finder_triggered();

private:
    static MainWindow *s_instance;
    explicit MainWindow(QWidget *parent = 0);

    Ui::MainWindow *ui;

    PreferencesWindow *m_preferencesWindow;
    DownloadsItemModel *m_downloadsModel;
    SeriesSortFilterProxyModel *m_seriesProxyModel;
    SeasonsListModel *m_seasonsModel;
    SeasonSortFilterProxyModel *m_seasonsProxyModel;
    EpisodesListModel *m_episodesModel;
    EpisodesSortFilterProxyModel *m_episodesProxyModel;
    HeaderView *m_seriesHeaderView;
    HeaderView *m_seasonsHeaderView;
    HeaderView *m_episodesHeaderView;
};

Q_DECLARE_METATYPE(Qt::SortOrder)

#endif // MAINWINDOW_H
