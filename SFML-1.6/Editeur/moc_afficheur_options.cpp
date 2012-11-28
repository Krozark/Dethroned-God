/****************************************************************************
** Meta object code from reading C++ file 'afficheur_options.hpp'
**
** Created: Wed May 11 19:14:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "afficheur_options.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'afficheur_options.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AFFICHEUR_OPTION[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   17,   17,   17, 0x0a,
      67,   17,   17,   17, 0x0a,
      83,   17,   17,   17, 0x0a,
      99,   17,   17,   17, 0x0a,
     115,   17,   17,   17, 0x0a,
     136,   17,   17,   17, 0x0a,
     153,   17,   17,   17, 0x0a,
     172,   17,   17,   17, 0x0a,
     191,   17,   17,   17, 0x0a,
     207,   17,   17,   17, 0x0a,
     227,   17,   17,   17, 0x0a,
     243,   17,   17,   17, 0x0a,
     259,   17,   17,   17, 0x0a,
     281,   17,   17,   17, 0x0a,
     300,   17,   17,   17, 0x0a,
     320,   17,   17,   17, 0x0a,
     341,   17,   17,   17, 0x0a,
     360,   17,   17,   17, 0x0a,
     382,   17,   17,   17, 0x0a,
     401,   17,   17,   17, 0x0a,
     416,   17,   17,   17, 0x0a,
     437,   17,   17,   17, 0x0a,
     455,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AFFICHEUR_OPTION[] = {
    "AFFICHEUR_OPTION\0\0Choix_DestinationPressed()\0"
    "change_direction(int)\0change_vue(int)\0"
    "change_min(int)\0change_max(int)\0"
    "change_agressif(int)\0change_boss(int)\0"
    "change_volant(int)\0change_degats(int)\0"
    "change_vie(int)\0change_vitesse(int)\0"
    "change_lvl(int)\0change_exp(int)\0"
    "change_resize(double)\0change_quette(int)\0"
    "change_element(int)\0change_type_obj(int)\0"
    "change_voleur(int)\0change_eparpieur(int)\0"
    "change_looter(int)\0change_or(int)\0"
    "Change_destination()\0change_dialogue()\0"
    "change_dialogue_skill(int)\0"
};

const QMetaObject AFFICHEUR_OPTION::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_AFFICHEUR_OPTION,
      qt_meta_data_AFFICHEUR_OPTION, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AFFICHEUR_OPTION::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AFFICHEUR_OPTION::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AFFICHEUR_OPTION::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AFFICHEUR_OPTION))
        return static_cast<void*>(const_cast< AFFICHEUR_OPTION*>(this));
    return QFrame::qt_metacast(_clname);
}

int AFFICHEUR_OPTION::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Choix_DestinationPressed(); break;
        case 1: change_direction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: change_vue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: change_min((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: change_max((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: change_agressif((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: change_boss((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: change_volant((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: change_degats((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: change_vie((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: change_vitesse((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: change_lvl((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: change_exp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: change_resize((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: change_quette((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: change_element((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: change_type_obj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: change_voleur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: change_eparpieur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: change_looter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: change_or((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: Change_destination(); break;
        case 22: change_dialogue(); break;
        case 23: change_dialogue_skill((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void AFFICHEUR_OPTION::Choix_DestinationPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
