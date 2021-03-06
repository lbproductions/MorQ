#include "serieslistmodel.h"

#include "controller/controller.h"
#include "model/episode.h"

#include <QPixmap>

SeriesListModel::SeriesListModel(QObject *parent) :
    QpObjectListModel<Series>(parent),
    m_checkable(false)
{
}

int SeriesListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags SeriesListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QpObjectListModel<Series>::flags(index);

    if (isCheckable())
        f = f | Qt::ItemIsTristate;

    return f;
}

QVariant SeriesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QSharedPointer<Series> series = objectByIndex(index);
    switch (role) {
    case Qt::DisplayRole:
        return series->title();
    case Qt::CheckStateRole:
        return series->checkState();
    case EpisodeCountRole:
        return series->episodes().size();
    case SeasonCountRole:
        return series->seasons().size();
    case Qt::DecorationRole:
        return Series::languageFlag(series->languages().first());
    case RawDataRole:
        return QVariant::fromValue<QSharedPointer<Series> >(series);
    }

    return QVariant();
}

bool SeriesListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        QSharedPointer<Series> series = objectByIndex(index);
        Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());

        if (checkState == Qt::Checked) {
            if (m_lastCheckedIndex.isValid()) {
                QSharedPointer<Series> lastCheckedSeries = objectByIndex(m_lastCheckedIndex);

                if (lastCheckedSeries && lastCheckedSeries != series && lastCheckedSeries->checkState() == Qt::Checked) {
                    lastCheckedSeries->setCheckState(Qt::PartiallyChecked);
                    emit dataChanged(m_lastCheckedIndex, m_lastCheckedIndex);
                }
            }

            m_lastCheckedIndex = index;
        }
        else if (checkState == Qt::PartiallyChecked && !m_lastCheckedIndex.isValid()) {
            m_lastCheckedIndex = index;
            checkState = Qt::Checked;
        }
        else if (checkState == Qt::Unchecked) {
            m_lastCheckedIndex = QModelIndex();
        }

        series->setCheckState(checkState);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool SeriesListModel::isCheckable() const
{
    return m_checkable;
}

void SeriesListModel::setCheckable(bool checkable)
{
    m_checkable = checkable;
}

QList<QSharedPointer<Series> > SeriesListModel::seriesByCheckState(Qt::CheckState state) const
{
    QList<QSharedPointer<Series> > result;
    foreach (QSharedPointer<Series> series, objects()) {
        if (series->checkState() == state) {
            result.append(series);
        }
    }
    return result;
}


QSharedPointer<Series> SeriesListModel::checkedSeries() const
{
    if (!m_lastCheckedIndex.isValid())
        return QSharedPointer<Series>();

    return objectByIndex(m_lastCheckedIndex);
}


SeriesSortFilterProxyModel::SeriesSortFilterProxyModel(SeriesListModel *sourceModel, QObject *parent) :
    QpSortFilterProxyObjectModel<Series>(sourceModel, parent)
{
}

QStringList SeriesSortFilterProxyModel::sortRoles() const
{
    static QStringList roles = QStringList()
            << tr("Title")
            << tr("Date")
            << tr("Episode count")
            << tr("Latest episode");
    return roles;
}

bool SeriesSortFilterProxyModel::lessThan(QSharedPointer<Series> left, QSharedPointer<Series> right) const
{
    if (sortRole() == Title)
        return left->title() < right->title();
    if (sortRole() == Date)
        return left->firstAired() < right->firstAired();
    if (sortRole() == EpisodeCount)
        return left->episodes().size() < right->episodes().size();
    if (sortRole() == LatestEpisode) {
        QSharedPointer<Episode> e1 = left->latestEpisode();
        if(!e1)
            return true;

        QSharedPointer<Episode> e2 = right->latestEpisode();
        if(!e2)
            return false;

        return e1->firstAired() < e2->firstAired();
    }

    return left < right;
}
