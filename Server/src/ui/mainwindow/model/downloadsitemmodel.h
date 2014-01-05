#ifndef DOWNLOADSITEMMODEL_H
#define DOWNLOADSITEMMODEL_H

#include <QAbstractItemModel>

namespace QDataSuite {
class AbstractDataAccessObject;
}

class Download;
class DownloadPackage;

class DownloadsItemModel : public QAbstractItemModel
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
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    bool hasChildren(const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

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
    QHash<QSharedPointer<DownloadPackage> , int> m_packageRows;
    QMap<QSharedPointer<Download> , int> m_downloadRows;

    void _insertPackage(QSharedPointer<DownloadPackage> package);
    void _insertDownload(QSharedPointer<Download> download);
    void _removeDownload(QSharedPointer<Download> download);

    QString humanReadableSize(qint64 bytes) const;

    mutable QHash<QString, QIcon> m_icons;

    QModelIndex indexForPackage(QSharedPointer<DownloadPackage> package) const;
};

#endif // DOWNLOADSITEMMODEL_H
