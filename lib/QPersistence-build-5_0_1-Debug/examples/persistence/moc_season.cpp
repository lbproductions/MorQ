/****************************************************************************
** Meta object code from reading C++ file 'season.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../QPersistence/QPersistence/examples/seriesModel/season.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'season.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Season_t {
    QByteArrayData data[9];
    char stringdata[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Season_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Season_t qt_meta_stringdata_Season = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 23),
QT_MOC_LITERAL(2, 31, 6),
QT_MOC_LITERAL(3, 38, 36),
QT_MOC_LITERAL(4, 75, 24),
QT_MOC_LITERAL(5, 100, 6),
QT_MOC_LITERAL(6, 107, 12),
QT_MOC_LITERAL(7, 120, 6),
QT_MOC_LITERAL(8, 127, 7)
    },
    "Season\0QPERSISTENCE_PRIMARYKEY\0tvdbId\0"
    "QPERSISTENCE_PROPERTYMETADATA:series\0"
    "reverserelation=seasons;\0number\0"
    "absolutePath\0series\0Series*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Season[] = {

 // content:
       7,       // revision
       0,       // classname
       2,   14, // classinfo
       0,    0, // methods
       4,   18, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // properties: name, type, flags
       2, QMetaType::Int, 0x00095103,
       5, QMetaType::Int, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, 0x80000000 | 8, 0x0009510b,

       0        // eod
};

void Season::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Season::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Season.data,
      qt_meta_data_Season,  qt_static_metacall, 0, 0}
};


const QMetaObject *Season::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Season::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Season.stringdata))
        return static_cast<void*>(const_cast< Season*>(this));
    return QObject::qt_metacast(_clname);
}

int Season::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = tvdbId(); break;
        case 1: *reinterpret_cast< int*>(_v) = number(); break;
        case 2: *reinterpret_cast< QString*>(_v) = absolutePath(); break;
        case 3: *reinterpret_cast< Series**>(_v) = series(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTvdbId(*reinterpret_cast< int*>(_v)); break;
        case 1: setNumber(*reinterpret_cast< int*>(_v)); break;
        case 2: setAbsolutePath(*reinterpret_cast< QString*>(_v)); break;
        case 3: setSeries(*reinterpret_cast< Series**>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
