#ifndef DOWNLOADSITEMMODEL_H
#define DOWNLOADSITEMMODEL_H

#include <QStandardItemModel>

namespace QDataSuite {
class AbstractDataAccessObject;
}

class Download;
class DownloadPackage;

class DownloadsItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum Columns {
        NameColumn,
        ProgressColumn,
        DownloadedColumn,
        FileSizeColumn,
        SpeedColumn,
        SpeedWeightedColumn,
        EtaColumn,
        MessageColumn,
        UserInputColumn,
        UrlColumn,
        RedirectedUrlColumn
    };

    explicit DownloadsItemModel(QObject *parent = 0);
    ~DownloadsItemModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QSharedPointer<Download> downloadByIndex(const QModelIndex &index) const;
    QSharedPointer<DownloadPackage> packageByIndex(const QModelIndex &index) const;

private slots:
    void insertPackage(QSharedPointer<QObject> object);
    void updatePackage(QSharedPointer<QObject> object);
    void removePackage(QSharedPointer<QObject> object);

    void insertDownload(QSharedPointer<QObject> object);
    void updateDownload(QSharedPointer<QObject> object);
    void removeDownload(QSharedPointer<QObject> object);

private:
    QString humanReadableSize(qint64 bytes) const;
    QHash<QSharedPointer<DownloadPackage> , int> m_packageRows;
    QHash<QSharedPointer<Download>, QStandardItem *> m_downloadItems;

    QSharedPointer<Download> downloadByItem(QStandardItem *item) const;
    QSharedPointer<DownloadPackage> packageByItem(QStandardItem *item) const;

    mutable QHash<QString, QIcon> m_icons;
};

#endif // DOWNLOADSITEMMODEL_H
