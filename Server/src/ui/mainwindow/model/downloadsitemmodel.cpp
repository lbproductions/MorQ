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
    QAbstractItemModel(parent)
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

        if(role == Qt::DisplayRole) {
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

        if(role == Qt::DisplayRole) {
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

int DownloadsItemModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return Qp::count<DownloadPackage>();

    QSharedPointer<DownloadPackage> package = packageByIndex(parent);
    if(!package)
        return 0;

    return package->downloads().size();
}

int DownloadsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 11;
}

bool DownloadsItemModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return true;

    QSharedPointer<DownloadPackage> package = packageByIndex(parent);
    if(!package)
        return false;

    return !package->downloads().isEmpty();
}

QModelIndex DownloadsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    if(parent.isValid()) {
        QSharedPointer<DownloadPackage> package = packageByIndex(parent);
        return createIndex(row, column, Qp::primaryKey(package->downloads().at(row)));
    }

    QSharedPointer<DownloadPackage> package = Qp::readAll<DownloadPackage>().at(row);
    return createIndex(row, column, Qp::primaryKey(package));
}

QModelIndex DownloadsItemModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    QSharedPointer<DownloadPackage> package = packageByIndex(child);
    if(package)
        return QModelIndex();

    QSharedPointer<Download> download = downloadByIndex(child);
    if(!download)
        return QModelIndex();

    package = download->package();
    if(!package || !m_packageRows.contains(package))
        return QModelIndex();

    return createIndex(m_packageRows.value(package), 0, Qp::primaryKey(package));
}

QSharedPointer<Download> DownloadsItemModel::downloadByIndex(const QModelIndex &index) const
{
    return Qp::read<Download>(index.internalId());
}

QSharedPointer<DownloadPackage> DownloadsItemModel::packageByIndex(const QModelIndex &index) const
{
    return Qp::read<DownloadPackage>(index.internalId());
}

void DownloadsItemModel::insertPackage(QSharedPointer<QObject> object)
{
    QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(object);
    int count = Qp::count<DownloadPackage>();
    beginInsertRows(QModelIndex(), count, count);

    _insertPackage(package);

    endInsertRows();
}

void DownloadsItemModel::_insertPackage(QSharedPointer<DownloadPackage> package)
{
    m_packageRows.insert(package, Qp::count<DownloadPackage>());

    foreach(QSharedPointer<Download> dl, package->downloads()) {
        _insertDownload(dl);
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

    foreach(QSharedPointer<Download> dl, package->downloads()) {
        _removeDownload(dl);
        _insertDownload(dl);
    }
}

void DownloadsItemModel::removePackage(QSharedPointer<QObject> object)
{
    QSharedPointer<DownloadPackage> package = qSharedPointerCast<DownloadPackage>(object);

    if(!m_packageRows.contains(package))
        return;

    int index = m_packageRows[package];

    beginRemoveRows(QModelIndex(), index, index);

    int i = 0;
    foreach(QSharedPointer<DownloadPackage> p, Qp::readAll<DownloadPackage>()) {
        m_packageRows.remove(p);
        m_packageRows.insert(p, i);
        ++i;
    }

    foreach(QSharedPointer<Download> dl, package->downloads()) {
        _removeDownload(dl);
    }
    endRemoveRows();
}

void DownloadsItemModel::insertDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    if(!download || !download->package())
        return;

    QModelIndex parent = indexForPackage(download->package());
    int index = download->order();

    beginInsertRows(parent, index, index);

    _insertDownload(download);

    endInsertRows();
}

void DownloadsItemModel::_insertDownload(QSharedPointer<Download> download)
{
    m_downloadRows.insert(download, download->order());
}

void DownloadsItemModel::updateDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    QModelIndex parent = indexForPackage(download->package());

    if(!m_downloadRows.contains(download)) {
        int index = download->order();
        beginInsertRows(parent, index, index);
        _insertDownload(download);
        endInsertRows();
    }
    else {
        int i = m_downloadRows[download];

        emit dataChanged(index(i, 0, parent),
                         index(i, columnCount(), parent));
    }
}

void DownloadsItemModel::removeDownload(QSharedPointer<QObject> object)
{
    QSharedPointer<Download> download = qSharedPointerCast<Download>(object);
    QModelIndex parent = indexForPackage(download->package());
    int i = m_downloadRows[download];

    beginRemoveRows(parent, i, i);

    _removeDownload(download);

    endRemoveRows();
}

void DownloadsItemModel::_removeDownload(QSharedPointer<Download> download)
{
    int i = 0;
    foreach(QSharedPointer<Download> dl, download->package()->downloads()) {
        m_downloadRows.remove(dl);
        if(dl != download) {
            m_downloadRows.insert(dl, i);
            ++i;
        }
    }
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

QModelIndex DownloadsItemModel::indexForPackage(QSharedPointer<DownloadPackage> package) const
{
    if(!package)
        return QModelIndex();

    return createIndex(m_packageRows.value(package), 0, Qp::primaryKey(package));
}
