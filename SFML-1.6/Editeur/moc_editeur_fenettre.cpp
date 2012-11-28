/****************************************************************************
** Meta object code from reading C++ file 'editeur_fenettre.hpp'
**
** Created: Wed May 11 19:15:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "editeur_fenettre.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editeur_fenettre.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EDITEUR_FENETTRE[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      34,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   17,   17,   17, 0x0a,
      66,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EDITEUR_FENETTRE[] = {
    "EDITEUR_FENETTRE\0\0LvlChanged(int)\0"
    "TypeChanged(int)\0changeLvl(int)\0"
    "changeType(int)\0"
};

const QMetaObject EDITEUR_FENETTRE::staticMetaObject = {
    { &QSFMLCanvas::staticMetaObject, qt_meta_stringdata_EDITEUR_FENETTRE,
      qt_meta_data_EDITEUR_FENETTRE, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EDITEUR_FENETTRE::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EDITEUR_FENETTRE::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EDITEUR_FENETTRE::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EDITEUR_FENETTRE))
        return static_cast<void*>(const_cast< EDITEUR_FENETTRE*>(this));
    return QSFMLCanvas::qt_metacast(_clname);
}

int EDITEUR_FENETTRE::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSFMLCanvas::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: LvlChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: TypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: changeLvl((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: changeType((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EDITEUR_FENETTRE::LvlChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EDITEUR_FENETTRE::TypeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
