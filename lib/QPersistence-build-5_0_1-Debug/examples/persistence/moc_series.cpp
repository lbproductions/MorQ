/****************************************************************************
** Meta object code from reading C++ file 'series.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../QPersistence/QPersistence/examples/seriesModel/series.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'series.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Series_t {
    QByteArrayData data[16];
    char stringdata[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Series_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Series_t qt_meta_stringdata_Series = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 23),
QT_MOC_LITERAL(2, 31, 6),
QT_MOC_LITERAL(3, 38, 37),
QT_MOC_LITERAL(4, 76, 23),
QT_MOC_LITERAL(5, 100, 5),
QT_MOC_LITERAL(6, 106, 12),
QT_MOC_LITERAL(7, 119, 6),
QT_MOC_LITERAL(8, 126, 8),
QT_MOC_LITERAL(9, 135, 10),
QT_MOC_LITERAL(10, 146, 6),
QT_MOC_LITERAL(11, 153, 6),
QT_MOC_LITERAL(12, 160, 10),
QT_MOC_LITERAL(13, 171, 10),
QT_MOC_LITERAL(14, 182, 7),
QT_MOC_LITERAL(15, 190, 14)
    },
    "Series\0QPERSISTENCE_PRIMARYKEY\0tvdbId\0"
    "QPERSISTENCE_PROPERTYMETADATA:seasons\0"
    "reverserelation=series;\0title\0"
    "absolutePath\0imdbId\0overview\0firstAired\0"
    "genres\0actors\0bannerUrls\0posterUrls\0"
    "seasons\0QList<Season*>\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Series[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       0,    0, // methods
      11,   18, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // properties: name, type, flags
       2, QMetaType::Int, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::QString, 0x00095103,
       9, QMetaType::QDate, 0x00095103,
      10, QMetaType::QStringList, 0x00095103,
      11, QMetaType::QStringList, 0x00095103,
      12, QMetaType::QStringList, 0x00095103,
      13, QMetaType::QStringList, 0x00095103,
      14, 0x80000000 | 15, 0x0009510b,

       0        // eod
};

void Series::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Series::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Series.data,
      qt_meta_data_Series,  qt_static_metacall, 0, 0}
};


const QMetaObject *Series::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Series::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Series.stringdata))
        return static_cast<void*>(const_cast< Series*>(this));
    return QObject::qt_metacast(_clname);
}

int Series::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = tvdbId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = title(); break;
        case 2: *reinterpret_cast< QString*>(_v) = absolutePath(); break;
        case 3: *reinterpret_cast< QString*>(_v) = imdbId(); break;
        case 4: *reinterpret_cast< QString*>(_v) = overview(); break;
        case 5: *reinterpret_cast< QDate*>(_v) = firstAired(); break;
        case 6: *reinterpret_cast< QStringList*>(_v) = genres(); break;
        case 7: *reinterpret_cast< QStringList*>(_v) = actors(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = bannerUrls(); break;
        case 9: *reinterpret_cast< QStringList*>(_v) = posterUrls(); break;
        case 10: *reinterpret_cast< QList<Season*>*>(_v) = seasons(); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTvdbId(*reinterpret_cast< int*>(_v)); break;
        case 1: setTitle(*reinterpret_cast< QString*>(_v)); break;
        case 2: setAbsolutePath(*reinterpret_cast< QString*>(_v)); break;
        case 3: setImdbId(*reinterpret_cast< QString*>(_v)); break;
        case 4: setOverview(*reinterpret_cast< QString*>(_v)); break;
        case 5: setFirstAired(*reinterpret_cast< QDate*>(_v)); break;
        case 6: setGenres(*reinterpret_cast< QStringList*>(_v)); break;
        case 7: setActors(*reinterpret_cast< QStringList*>(_v)); break;
        case 8: setBannerUrls(*reinterpret_cast< QStringList*>(_v)); break;
        case 9: setPosterUrls(*reinterpret_cast< QStringList*>(_v)); break;
        case 10: setSeasons(*reinterpret_cast< QList<Season*>*>(_v)); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
