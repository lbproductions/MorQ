#ifndef QPERSISTENCE_DATAACCESSOBJECT_H
#define QPERSISTENCE_DATAACCESSOBJECT_H

#include <QtCore/QObject>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QSharedPointer>
#include <QtSql/QSqlDatabase>

#include "conversion.h"
#include "metaobject.h"

class QSqlQuery;
class QpError;
class QpSqlDataAccessObjectHelper;

class QpDaoBaseData;
class QpDaoBase : public QObject
{
    Q_OBJECT
public:
    static QpDaoBase *forClass(const QMetaObject &metaObject);
    static QList<QpDaoBase *> dataAccessObjects();

    ~QpDaoBase();

    QpSqlDataAccessObjectHelper *sqlDataAccessObjectHelper() const;
    QpMetaObject qpMetaObject() const;

    int count() const;
    QList<int> allKeys(int skip = -1, int count = -1) const;
    QList<QSharedPointer<QObject> > readAllObjects(int skip = -1, int count = -1) const;
    QSharedPointer<QObject> readObject(int id) const;
    QSharedPointer<QObject> createObject();
    bool updateObject(QSharedPointer<QObject> object);
    bool removeObject(QSharedPointer<QObject> object);

    QpError lastError() const;

Q_SIGNALS:
    void objectCreated(QSharedPointer<QObject>);
    void objectUpdated(QSharedPointer<QObject>);
    void objectRemoved(QSharedPointer<QObject>);

protected:
    explicit QpDaoBase(const QMetaObject &metaObject,
                       QObject *parent = 0);

    virtual QObject *createInstance() const = 0;

private:
    QSharedDataPointer<QpDaoBaseData> data;

    void setLastError(const QpError &error) const;
    void resetLastError() const;

    Q_DISABLE_COPY(QpDaoBase)
};

namespace Qp {
template<class T>
void registerClass();
template<class T, class Source>
QList<QSharedPointer<T> > castList(const QList<QSharedPointer<Source> >&);
}

template<class T>
class QpDao : public QpDaoBase
{
public:
    QSharedPointer<T> read(int id) { return qSharedPointerCast<T>(readObject(id)); }
    QList<QSharedPointer<T> > readAllObjects(int skip = -1, int count = -1) const
    {
        return Qp::castList<T>(QpDaoBase::readAllObjects(skip, count));
    }

protected:
    QpDao(QObject *parent) :
        QpDaoBase(T::staticMetaObject, parent)
    {}

    QObject *createInstance() const Q_DECL_OVERRIDE { return new T; }

private:
    template<class O> friend void Qp::registerClass();
    Q_DISABLE_COPY(QpDao)
};

uint qHash(const QVariant &var);

#endif // QPERSISTENCE_DATAACCESSOBJECT_H
