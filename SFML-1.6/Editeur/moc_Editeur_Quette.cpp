/****************************************************************************
** Meta object code from reading C++ file 'Editeur_Quette.h'
**
** Created: Wed May 11 19:15:07 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Editeur_Quette.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Editeur_Quette.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Editeur_Quette[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      44,   15,   15,   15, 0x0a,
      70,   15,   15,   15, 0x0a,
      95,   15,   15,   15, 0x0a,
     121,   15,   15,   15, 0x0a,
     145,   15,   15,   15, 0x0a,
     166,   15,   15,   15, 0x0a,
     188,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Editeur_Quette[] = {
    "Editeur_Quette\0\0Changer_dialogue_objet(int)\0"
    "Changer_quette_objet(int)\0"
    "Changer_skill_objet(int)\0"
    "Changer_dialogue_pnj(int)\0"
    "Changer_quette_pnj(int)\0Changer_lvl_pnj(int)\0"
    "Change_type_Edit(int)\0reload()\0"
};

const QMetaObject Editeur_Quette::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Editeur_Quette,
      qt_meta_data_Editeur_Quette, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Editeur_Quette::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Editeur_Quette::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Editeur_Quette::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Editeur_Quette))
        return static_cast<void*>(const_cast< Editeur_Quette*>(this));
    return QFrame::qt_metacast(_clname);
}

int Editeur_Quette::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Changer_dialogue_objet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: Changer_quette_objet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: Changer_skill_objet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: Changer_dialogue_pnj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: Changer_quette_pnj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: Changer_lvl_pnj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: Change_type_Edit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: reload(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
