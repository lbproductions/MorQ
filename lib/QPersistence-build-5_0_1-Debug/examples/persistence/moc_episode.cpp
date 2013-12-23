/****************************************************************************
** Meta object code from reading C++ file 'episode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../QPersistence/QPersistence/examples/seriesModel/episode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'episode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Episode_t {
    QByteArrayData data[12];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Episode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Episode_t qt_meta_stringdata_Episode = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 6),
QT_MOC_LITERAL(2, 15, 5),
QT_MOC_LITERAL(3, 21, 16),
QT_MOC_LITERAL(4, 38, 6),
QT_MOC_LITERAL(5, 45, 6),
QT_MOC_LITERAL(6, 52, 6),
QT_MOC_LITERAL(7, 59, 8),
QT_MOC_LITERAL(8, 68, 10),
QT_MOC_LITERAL(9, 79, 10),
QT_MOC_LITERAL(10, 90, 7),
QT_MOC_LITERAL(11, 98, 8)
    },
    "Episode\0length\0title\0absoluteFileName\0"
    "number\0tvdbId\0imdbId\0overview\0firstAired\0"
    "guestStars\0writers\0director\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Episode[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      11,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QTime, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QString, 0x00095103,
       4, QMetaType::Int, 0x00095103,
       5, QMetaType::Int, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::QDate, 0x00095103,
       9, QMetaType::QStringList, 0x00095103,
      10, QMetaType::QStringList, 0x00095103,
      11, QMetaType::QString, 0x00095103,

       0        // eod
};

void Episode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Episode::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Episode.data,
      qt_meta_data_Episode,  qt_static_metacall, 0, 0}
};


const QMetaObject *Episode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Episode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Episode.stringdata))
        return static_cast<void*>(const_cast< Episode*>(this));
    return QObject::qt_metacast(_clname);
}

int Episode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QTime*>(_v) = length(); break;
        case 1: *reinterpret_cast< QString*>(_v) = title(); break;
        case 2: *reinterpret_cast< QString*>(_v) = absoluteFileName(); break;
        case 3: *reinterpret_cast< int*>(_v) = number(); break;
        case 4: *reinterpret_cast< int*>(_v) = tvdbId(); break;
        case 5: *reinterpret_cast< QString*>(_v) = imdbId(); break;
        case 6: *reinterpret_cast< QString*>(_v) = overview(); break;
        case 7: *reinterpret_cast< QDate*>(_v) = firstAired(); break;
        case 8: *reinterpret_cast< QStringList*>(_v) = guestStars(); break;
        case 9: *reinterpret_cast< QStringList*>(_v) = writers(); break;
        case 10: *reinterpret_cast< QString*>(_v) = director(); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLength(*reinterpret_cast< QTime*>(_v)); break;
        case 1: setTitle(*reinterpret_cast< QString*>(_v)); break;
        case 2: setAbsoluteFileName(*reinterpret_cast< QString*>(_v)); break;
        case 3: setNumber(*reinterpret_cast< int*>(_v)); break;
        case 4: setTvdbId(*reinterpret_cast< int*>(_v)); break;
        case 5: setImdbId(*reinterpret_cast< QString*>(_v)); break;
        case 6: setOverview(*reinterpret_cast< QString*>(_v)); break;
        case 7: setFirstAired(*reinterpret_cast< QDate*>(_v)); break;
        case 8: setGuestStars(*reinterpret_cast< QStringList*>(_v)); break;
        case 9: setWriters(*reinterpret_cast< QStringList*>(_v)); break;
        case 10: setDirector(*reinterpret_cast< QString*>(_v)); break;
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
