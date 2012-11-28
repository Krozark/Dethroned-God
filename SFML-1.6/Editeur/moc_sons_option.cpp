/****************************************************************************
** Meta object code from reading C++ file 'sons_option.hpp'
**
** Created: Wed May 11 19:15:14 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sons_option.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sons_option.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SONS_OPTION[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      25,   12,   12,   12, 0x0a,
      44,   12,   12,   12, 0x0a,
      70,   12,   12,   12, 0x0a,
      96,   12,   12,   12, 0x0a,
     117,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SONS_OPTION[] = {
    "SONS_OPTION\0\0Play_stop()\0ChangeSon(QString)\0"
    "ChangeAttenuation(double)\0"
    "ChangeMinDistance(double)\0"
    "ChangeVolume(double)\0ChangeTypeSon()\0"
};

const QMetaObject SONS_OPTION::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SONS_OPTION,
      qt_meta_data_SONS_OPTION, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SONS_OPTION::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SONS_OPTION::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SONS_OPTION::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SONS_OPTION))
        return static_cast<void*>(const_cast< SONS_OPTION*>(this));
    return QFrame::qt_metacast(_clname);
}

int SONS_OPTION::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Play_stop(); break;
        case 1: ChangeSon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: ChangeAttenuation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: ChangeMinDistance((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: ChangeVolume((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: ChangeTypeSon(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
