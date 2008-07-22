/****************************************************************************
** Meta object code from reading C++ file 'CappiGen.h'
**
** Created: Tue Jul 22 08:50:44 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CappiGen.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CappiGen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CappiGen[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      45,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CappiGen[] = {
    "CappiGen\0\0p,radarId,prodType,timetag,hskpMap\0"
    "productSlot(std::vector<double>,int,int,qlonglong,StrMapDouble)\0"
};

const QMetaObject CappiGen::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CappiGen,
      qt_meta_data_CappiGen, 0 }
};

const QMetaObject *CappiGen::metaObject() const
{
    return &staticMetaObject;
}

void *CappiGen::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CappiGen))
	return static_cast<void*>(const_cast<CappiGen*>(this));
    return QObject::qt_metacast(_clname);
}

int CappiGen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: productSlot((*reinterpret_cast< std::vector<double>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< qlonglong(*)>(_a[4])),(*reinterpret_cast< StrMapDouble(*)>(_a[5]))); break;
        }
        _id -= 1;
    }
    return _id;
}
