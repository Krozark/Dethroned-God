/****************************************************************************
** Meta object code from reading C++ file 'afficheur.hpp'
**
** Created: Wed May 11 19:14:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "afficheur.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'afficheur.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AFFICHEUR_GRILLE[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      28,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   17,   17,   17, 0x0a,
      48,   46,   17,   17, 0x0a,
      70,   17,   17,   17, 0x0a,
      82,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AFFICHEUR_GRILLE[] = {
    "AFFICHEUR_GRILLE\0\0Clicked()\0Relased()\0"
    "reset()\0c\0Set_type_edition(int)\0"
    "modifFond()\0ChangeDestTp()\0"
};

const QMetaObject AFFICHEUR_GRILLE::staticMetaObject = {
    { &QSFMLCanvas::staticMetaObject, qt_meta_stringdata_AFFICHEUR_GRILLE,
      qt_meta_data_AFFICHEUR_GRILLE, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AFFICHEUR_GRILLE::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AFFICHEUR_GRILLE::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AFFICHEUR_GRILLE::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AFFICHEUR_GRILLE))
        return static_cast<void*>(const_cast< AFFICHEUR_GRILLE*>(this));
    return QSFMLCanvas::qt_metacast(_clname);
}

int AFFICHEUR_GRILLE::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSFMLCanvas::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Clicked(); break;
        case 1: Relased(); break;
        case 2: reset(); break;
        case 3: Set_type_edition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: modifFond(); break;
        case 5: ChangeDestTp(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AFFICHEUR_GRILLE::Clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AFFICHEUR_GRILLE::Relased()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
