#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>

class QpAbstractObjectListModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QpAbstractObjectListModelBase(QObject *parent = 0);

protected slots:
    virtual void objectInserted(QObject *object) = 0;
    virtual void objectUpdated(QObject *object) = 0;
    virtual void objectRemoved(QObject *object) = 0;
};

template<class T>
class ObjectListModel : public QpAbstractObjectListModelBase
{
public:
    explicit ObjectListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowOf(QObject *object) const;

    T *objectByIndex(const QModelIndex &index) const;
    virtual QList<T *> objects() const = 0;

protected:
    void objectInserted(QObject *object) Q_DECL_OVERRIDE;
    void objectUpdated(QObject *object) Q_DECL_OVERRIDE;
    void objectRemoved(QObject *object) Q_DECL_OVERRIDE;

private:
    QHash<T *, int> m_rows;
    void adjustExistingRows();
};

template<class T>
T *ObjectListModel<T>::objectByIndex(const QModelIndex &index) const
{
    if(index.row() >= objects().size())
        return nullptr;

    return objects().at(index.row());
}

template<class T>
ObjectListModel<T>::ObjectListModel(QObject *parent) :
    QpAbstractObjectListModelBase(parent)
{
}

template<class T>
int ObjectListModel<T>::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return objects().size();
}

template<class T>
int ObjectListModel<T>::rowOf(QObject *object) const
{
    if(m_rows.contains(static_cast<T*>(object)))
        return m_rows.value(static_cast<T*>(object));

    return objects().indexOf(static_cast<T*>(object));
}

template<class T>
void ObjectListModel<T>::objectInserted(QObject *object)
{
    int row = rowOf(object);
    if(row < 0)
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_rows.insert(static_cast<T*>(object), row);
    adjustExistingRows();
    endInsertRows();
}

template<class T>
void ObjectListModel<T>::objectUpdated(QObject *object)
{
    int row = rowOf(object);
    if(row < 0)
        return;

    emit dataChanged(index(row), index(row));
}

template<class T>
void ObjectListModel<T>::objectRemoved(QObject *object)
{
    int row = rowOf(object);
    if(row < 0)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_rows.remove(static_cast<T*>(object));
    adjustExistingRows();
    endRemoveRows();
}

template<class T>
void ObjectListModel<T>::adjustExistingRows()
{
    int i = 0;
    foreach(T *object, objects()) {
        if(m_rows.contains(object))
            m_rows.insert(object, i);
        ++i;
    }
}

#endif // OBJECTLISTMODEL_H
