#include "downloadsitemmodel.h"

#include "controller/controller.h"

#include "model/download.h"
#include "model/downloadpackage.h"

#include <QPersistence.h>

#include <QFileIconProvider>
#include <QDebug>
#include <QTime>
#include <QTemporaryFile>

DownloadsItemModel::DownloadsItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
    connect(Qp::dataAccessObject<Download>(), &QpDaoBase::objectCreated,
            this, &DownloadsItemModel::insertDownload);
    connect(Qp::dataAccessObject<Download>(), &QpDaoBase::objectUpdated,
            this, &DownloadsItemModel::updateDownload);
    connect(Qp::dataAccessObject<Download>(), &QpDaoBase::objectRemoved,
            this, &DownloadsItemModel::removeDownload);

    connect(Qp::dataAccessObject<DownloadPackage>(), &QpDaoBase::objectCreated,
            this, &DownloadsItemModel::insertPackage);
    connect(Qp::dataAccessObject<DownloadPackage>(), &QpDaoBase::objectUpdated,
            this, &DownloadsItemModel::updatePackage);
    connect(Qp::dataAccessObject<DownloadPackage>(), &QpDaoBase::objectRemoved,
            this, &DownloadsItemModel::removePackage);

    foreach(QSharedPointer<DownloadPackage> object, Qp::readAll<DownloadPackage>()) {
        insertPackage(object);
    }

    setColumnCount(columnCount());
    invisibleRootItem()->setColumnCount(columnCount());
}

DownloadsItemModel::~DownloadsItemModel()
{
}

Qt::ItemFlags DownloadsItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractItemModel::flags(index);
    if(!index.parent().isValid()
            && index.column() == UserInputColumn
            && !data(index, Qt::DisplayRole).isNull()) {
        return f | Qt::ItemIsEditable;
    }

    return f;
}

QVariant DownloadsItemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    static double speed = 0.0;
    static double weightedSpeed = 0.0;
    static QFileIconProvider provider;
    static QByteArray captchaData;
    static QTime eta;

    if(!index.parent().isValid()) {
        QSharedPointer<DownloadPackage> package = packageByIndex(index);
        if(!package)
            return QVariant();

        if(role == Qt::UserRole + 1) {
            return QVariant::fromValue<QSharedPointer<DownloadPackage> >(package);
        }
        else if(role == Qt::DisplayRole) {
            switch(index.column()) {
            case NameColumn:
                return package->name();

            case ProgressColumn:
                return package->progress(); // painting percentages is done by item delegate

            case FileSizeColumn:
                return humanReadableSize(package->totalFileSize());

            case DownloadedColumn:
                return humanReadableSize(package->bytesDownloaded());

            case SpeedColumn:
                speed = qAbs(double(package->speed()));
                if(qFuzzyIsNull(speed))
                    return QVariant();

                return QString("%1/s").arg(humanReadableSize(speed));

            case SpeedWeightedColumn:
                weightedSpeed = qAbs(double(package->speedWeighted()));
                if(qFuzzyIsNull(weightedSpeed))
                    return QVariant();

                return QString("%1/s").arg(humanReadableSize(weightedSpeed));

            case EtaColumn:
                eta = package->eta();
                if(eta.isNull())
                    return QVariant();
                if(eta == QTime(0,0,0))
                    return QVariant();

                return eta.toString("hh:mm:ss");

            case MessageColumn:
                return package->message();

            case UserInputColumn:
                captchaData = package->captcha();
                if(captchaData.isEmpty())
                    return QVariant();
                return QPixmap::fromImage(QImage::fromData(captchaData));

            case UrlColumn:
                return package->sourceUrl();

            case RedirectedUrlColumn:
                return QVariant();

            default:
                break;
            }
        }
        else if(role == Qt::DecorationRole) {

            static QFileIconProvider provider;
            static QIcon FOLDERICON = provider.icon(QFileIconProvider::Folder);

            switch(index.column()) {
            case NameColumn:
                return FOLDERICON;

            default:
                break;
            }
        }
    }
    else if(!index.parent().parent().isValid()) {
        QSharedPointer<Download> dl = downloadByIndex(index);

        if(role == Qt::UserRole + 1) {
            return QVariant::fromValue<QSharedPointer<Download> >(dl);
        }
        else if(role == Qt::DisplayRole) {
            switch(index.column()) {
            case NameColumn:
                return dl->fileName();

            case ProgressColumn:
                return dl->downloadProgress(); // painting percentages is done by item delegate

            case FileSizeColumn:
                return humanReadableSize(dl->fileSize());

            case DownloadedColumn:
                return humanReadableSize(dl->bytesDownloaded());

            case SpeedColumn:
                speed = qAbs(double(dl->speed()));
                if(qFuzzyIsNull(speed))
                    return QVariant();

                return QString("%1/s").arg(humanReadableSize(speed));

            case SpeedWeightedColumn:
                weightedSpeed = qAbs(double(dl->speedWeighted()));
                if(qFuzzyIsNull(weightedSpeed))
                    return QVariant();

                return QString("%1/s").arg(humanReadableSize(weightedSpeed));

            case EtaColumn:
                eta = dl->eta();
                if(eta.isNull())
                    return QVariant();
                if(eta == QTime(0,0,0))
                    return QVariant();

                return eta.toString("hh:mm:ss");

            case MessageColumn:
                return dl->message();

            case UrlColumn:
                return dl->url();

            case RedirectedUrlColumn:
                return dl->redirectedUrl();

            default:
                break;
            }
        }
        else if(role == Qt::DecorationRole) {

            QIcon icon;
            if(m_icons.contains(dl->fileName())) {
                icon = m_icons.value(dl->fileName());
            }
            else {
                QTemporaryFile file(QString("XXXXXX").append(dl->fileName())); // XXXXXX so that QTemporaryFile does not append these
                // which would overwrite the original extension
                file.open(); // Open, because QFileIconProvider only works for existing files... This little fucker
                QFileInfo info(file);
                icon = provider.icon(info);
                m_icons.insert(dl->fileName(), icon);
            }

            switch(index.column()) {
            case NameColumn:
                return icon;
            default:
                break;
            }
        }
    }

    return QVariant();
}

QVariant DownloadsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role == Qt::DisplayRole) {
        switch(section) {
        case NameColumn:
            return QVariant("Name");
        case FileSizeColumn:
            return QVariant("Size");
        case DownloadedColumn:
            return QVariant("Downloaded");
        case ProgressColumn:
            return QVariant("Progress");
        case SpeedColumn:
            return QVariant("Speed");
        case SpeedWeightedColumn:
            return QVariant("Av.Speed");
        case EtaColumn:
            return QVariant("ETA");
        case MessageColumn:
            return QVariant("Message");
        case UserInputColumn:
            return QVariant("");
        case UrlColumn:
            return QVariant("Url");
        case RedirectedUrlColumn:
            return QVariant("Redirected");
        default:
            break;
        }
    }

    return QVariant();
}

int DownloadsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 11;
}

QSharedPointer<Download> DownloadsItemModel::downloadByIndex(const QModelIndex &index) const
{
    return downloadByItem(invisibleRootItem()->child(index.parent().row())->child(index.row()));
}

QSharedPointer<DownloadPackage> DownloadsItemModel::packageByIndex(const QModelIndex &index) const
{
    return packageByItem(invisibleRootItem()->child(index.row()));
}

void DownloadsItemModel::insertPackage(QSharedPointer<QObject> object)
{
    QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(object);
    int count = rowCount();
    m_packageRows.insert(package, count);

    QStandardItem *item = new QStandardItem(package->name());
    item->setData(QVariant::fromValue<QSharedPointer<DownloadPackage> >(package));
    item->setColumnCount(columnCount());
    invisibleRootItem()->appendRow(item);

    foreach(QSharedPointer<Download> dl, package->downloads()) {
        QStandardItem *child = new QStandardItem(dl->fileName());
        child->setData(QVariant::fromValue<QSharedPointer<Download> >(dl));
        item->appendRow(child);
    }
}

void DownloadsItemModel::updatePackage(QSharedPointer<QObject> object)
{
    QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(object);

    if(!m_packageRows.contains(package))
        return;

    int i = m_packageRows[package];
    emit dataChanged(index(i, 0),
                     index(i, columnCount()));
}

void DownloadsItemModel::removePackage(QSharedPointer<QObject> object)
{
    QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(object);

    if(!m_packageRows.contains(package))
        return;

    int row = m_packageRows[package];

    invisibleRootItem()->removeRow(row);

    int i = 0;
    foreach(QSharedPointer<DownloadPackage> p, Qp::readAll<DownloadPackage>()) {
        m_packageRows.remove(p);
        m_packageRows.insert(p, i);
        ++i;
    }
}

void DownloadsItemModel::insertDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    if(!download || !download->package())
        return;

    QStandardItem *packageItem = invisibleRootItem()->child(m_packageRows[download->package()]);

    QStandardItem *child = new QStandardItem(download->fileName());
    child->setColumnCount(columnCount());
    child->setData(QVariant::fromValue<QSharedPointer<Download> >(download));
    m_downloadItems.insert(download, child);
    packageItem->appendRow(child);
}

void DownloadsItemModel::updateDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    if(!download->package())
        return;

    if(!m_downloadItems.contains(download)) {
        insertDownload(download);
    }
    else {
        int parentRow = m_packageRows[download->package()];
        QModelIndex parent = index(parentRow, 0);

        int row = download->order();
        emit dataChanged(index(row, 0, parent),
                         index(row, columnCount(), parent));
    }
}

void DownloadsItemModel::removeDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    QStandardItem *packageItem = invisibleRootItem()->child(m_packageRows[download->package()]);

    packageItem->removeRow(download->order());
    m_downloadItems.remove(download);
}

QString DownloadsItemModel::humanReadableSize(qint64 bytes) const
{
    static const int THRESH = 1100;
    if(bytes < 0)
        return QString();

    if(bytes == 0)
        return QString("0.0 MB");

    double result = bytes;
    int i = 0;
    while(result > THRESH
          && i < 4) {
        result /= 1000.0;
        ++i;
    }

    QString unit("B");

    switch(i) {
    case 0:
    default:
        break;
    case 1:
        unit.prepend("K");
        break;
    case 2:
        unit.prepend("M");
        break;
    case 3:
        unit.prepend("G");
        break;
    case 4:
        unit.prepend("T");
        break;
    case 5:
        unit.prepend("P");
        break;
    case 6:
        unit.prepend("E"); // Exabyte YEAAAHHHH! That physics class finally tought me something useful
        break;

    // Let us for now assume, that now one's gonna load faster than 1000 Exabytes per second ^^
    }

    return QString("%1 %2").arg(QString::number(result,'f',2)).arg(unit);
}

QSharedPointer<Download> DownloadsItemModel::downloadByItem(QStandardItem *item) const
{
    return item->data().value<QSharedPointer<Download> >();
}

QSharedPointer<DownloadPackage> DownloadsItemModel::packageByItem(QStandardItem *item) const
{
    return item->data().value<QSharedPointer<DownloadPackage> >();
}
