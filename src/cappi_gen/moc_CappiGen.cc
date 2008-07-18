/****************************************************************************
** Meta object code from reading C++ file 'CappiGen.h'
**
** Created: Fri Jul 18 17:12:00 2008
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
     160,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CappiGen[] = {
    "CappiGen\0\0"
    "p,radarId,elDegrees,prodType,gateSizeMeters,dwellWidth,airspdCorr,timetag,longitude,latitude,elevation,heading,roll,pitch,groundSpeedEW,groundSpeedNS\0"
    "productSlot(std::vector<double>,int,float,int,float,double,double,qlonglong,double,double,double,double,double,double,double,double)\0"
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
        case 0: productSlot((*reinterpret_cast< std::vector<double>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< qlonglong(*)>(_a[8])),(*reinterpret_cast< double(*)>(_a[9])),(*reinterpret_cast< double(*)>(_a[10])),(*reinterpret_cast< double(*)>(_a[11])),(*reinterpret_cast< double(*)>(_a[12])),(*reinterpret_cast< double(*)>(_a[13])),(*reinterpret_cast< double(*)>(_a[14])),(*reinterpret_cast< double(*)>(_a[15])),(*reinterpret_cast< double(*)>(_a[16]))); break;
        }
        _id -= 1;
    }
    return _id;
}
