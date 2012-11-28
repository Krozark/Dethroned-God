/****************************************************************************
** Meta object code from reading C++ file 'editeur_MAP.hpp'
**
** Created: Wed May 11 19:15:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "editeur_MAP.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editeur_MAP.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EDITEUR_MAP[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x0a,
      32,   12,   12,   12, 0x0a,
      53,   12,   12,   12, 0x2a,
      70,   12,   12,   12, 0x0a,
      78,   12,   12,   12, 0x0a,
      88,   12,   12,   12, 0x0a,
     102,   12,   12,   12, 0x0a,
     119,  117,   12,   12, 0x0a,
     141,  117,   12,   12, 0x0a,
     158,  156,   12,   12, 0x0a,
     173,  171,   12,   12, 0x0a,
     185,   12,   12,   12, 0x0a,
     210,   12,   12,   12, 0x0a,
     233,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EDITEUR_MAP[] = {
    "EDITEUR_MAP\0\0m\0Set_map(QString)\0"
    "ecrire_fichier(bool)\0ecrire_fichier()\0"
    "reset()\0quitter()\0envoie_site()\0"
    "Set_map_load()\0c\0Set_type_edition(int)\0"
    "Set_Ligne(int)\0t\0SetType(int)\0l\0"
    "SetLvl(int)\0ChangeAutotEnabled(bool)\0"
    "BougerFenettreCliked()\0BougerFenettreRelase()\0"
};

const QMetaObject EDITEUR_MAP::staticMetaObject = {
    { &QSFMLCanvas::staticMetaObject, qt_meta_stringdata_EDITEUR_MAP,
      qt_meta_data_EDITEUR_MAP, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EDITEUR_MAP::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EDITEUR_MAP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EDITEUR_MAP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EDITEUR_MAP))
        return static_cast<void*>(const_cast< EDITEUR_MAP*>(this));
    return QSFMLCanvas::qt_metacast(_clname);
}

int EDITEUR_MAP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSFMLCanvas::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Set_map((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: ecrire_fichier((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: ecrire_fichier(); break;
        case 3: reset(); break;
        case 4: quitter(); break;
        case 5: envoie_site(); break;
        case 6: Set_map_load(); break;
        case 7: Set_type_edition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: Set_Ligne((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: SetType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: SetLvl((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: ChangeAutotEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: BougerFenettreCliked(); break;
        case 13: BougerFenettreRelase(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
