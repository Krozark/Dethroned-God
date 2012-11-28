/****************************************************************************
** Meta object code from reading C++ file 'map_options.hpp'
**
** Created: Wed May 11 19:15:12 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "map_options.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'map_options.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MAP_OPTION[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   11,   11,   11, 0x0a,
      50,   11,   11,   11, 0x0a,
      73,   11,   11,   11, 0x0a,
      94,   11,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     137,   11,   11,   11, 0x0a,
     160,   11,   11,   11, 0x0a,
     181,   11,   11,   11, 0x0a,
     208,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MAP_OPTION[] = {
    "MAP_OPTION\0\0FondChanged()\0"
    "change_gravitee(double)\0change_frottement(int)\0"
    "change_Fond(QString)\0change_musique(QString)\0"
    "change_chrono(int)\0change_NomMap(QString)\0"
    "change_Type(QString)\0change_Mythologie(QString)\0"
    "Change_AutoDialogue()\0"
};

const QMetaObject MAP_OPTION::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_MAP_OPTION,
      qt_meta_data_MAP_OPTION, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MAP_OPTION::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MAP_OPTION::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MAP_OPTION::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MAP_OPTION))
        return static_cast<void*>(const_cast< MAP_OPTION*>(this));
    return QFrame::qt_metacast(_clname);
}

int MAP_OPTION::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: FondChanged(); break;
        case 1: change_gravitee((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: change_frottement((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: change_Fond((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: change_musique((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: change_chrono((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: change_NomMap((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: change_Type((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: change_Mythologie((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: Change_AutoDialogue(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MAP_OPTION::FondChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
