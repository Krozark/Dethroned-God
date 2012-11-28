/****************************************************************************
** Meta object code from reading C++ file 'ClasseRemake.hpp'
**
** Created: Tue Mar 29 15:24:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ClasseRemake.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClasseRemake.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QLineEditIndex[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QLineEditIndex[] = {
    "QLineEditIndex\0\0index\0EmitIndex(int)\0"
    "slotTextChange()\0"
};

const QMetaObject QLineEditIndex::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_QLineEditIndex,
      qt_meta_data_QLineEditIndex, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QLineEditIndex::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QLineEditIndex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QLineEditIndex::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QLineEditIndex))
        return static_cast<void*>(const_cast< QLineEditIndex*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int QLineEditIndex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: EmitIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slotTextChange(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QLineEditIndex::EmitIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QTextEditIndex[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTextEditIndex[] = {
    "QTextEditIndex\0\0index\0EmitIndex(int)\0"
    "slotTextChange()\0"
};

const QMetaObject QTextEditIndex::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_QTextEditIndex,
      qt_meta_data_QTextEditIndex, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTextEditIndex::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTextEditIndex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTextEditIndex::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTextEditIndex))
        return static_cast<void*>(const_cast< QTextEditIndex*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int QTextEditIndex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: EmitIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slotTextChange(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QTextEditIndex::EmitIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QSpinBoxIndex[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QSpinBoxIndex[] = {
    "QSpinBoxIndex\0\0index\0EmitIndex(int)\0"
    "slotValueChange()\0"
};

const QMetaObject QSpinBoxIndex::staticMetaObject = {
    { &QSpinBox::staticMetaObject, qt_meta_stringdata_QSpinBoxIndex,
      qt_meta_data_QSpinBoxIndex, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSpinBoxIndex::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSpinBoxIndex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSpinBoxIndex::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSpinBoxIndex))
        return static_cast<void*>(const_cast< QSpinBoxIndex*>(this));
    return QSpinBox::qt_metacast(_clname);
}

int QSpinBoxIndex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSpinBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: EmitIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slotValueChange(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QSpinBoxIndex::EmitIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_QComboBoxIndex[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QComboBoxIndex[] = {
    "QComboBoxIndex\0\0index\0EmitIndex(int)\0"
    "slotValueChange()\0"
};

const QMetaObject QComboBoxIndex::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_QComboBoxIndex,
      qt_meta_data_QComboBoxIndex, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QComboBoxIndex::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QComboBoxIndex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QComboBoxIndex::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QComboBoxIndex))
        return static_cast<void*>(const_cast< QComboBoxIndex*>(this));
    return QComboBox::qt_metacast(_clname);
}

int QComboBoxIndex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: EmitIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slotValueChange(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QComboBoxIndex::EmitIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
