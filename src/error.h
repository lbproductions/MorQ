#ifndef QPERSISTENCE_ERROR_H
#define QPERSISTENCE_ERROR_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include <QtCore/QVariantMap>

class QSqlQuery;

class QPersistenceErrorPrivate;
class QPersistenceError
{
public:
    enum ErrorType {
        NoError = 0,
        SqlError,
        ParserError,
        SerializerError,
        ServerError,
        StorageError,
        UserError = 1024
    };

    QPersistenceError(const QString &text = QString(),
          ErrorType type = NoError,
          QVariantMap additionalInformation = QVariantMap());
    ~QPersistenceError();
    QPersistenceError(const QPersistenceError &other);
    QPersistenceError &operator = (const QPersistenceError &other);

    bool isValid() const;
    QString text() const;
    ErrorType type() const;

    QVariantMap additionalInformation() const;
    void addAdditionalInformation(const QString &key, const QVariant &value);

private:
    QSharedDataPointer<QPersistenceErrorPrivate> d;
};

QDebug operator<<(QDebug dbg, const QPersistenceError &error);

#endif // QPERSISTENCE_ERROR_H
