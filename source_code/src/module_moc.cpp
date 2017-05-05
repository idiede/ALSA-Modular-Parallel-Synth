/****************************************************************************
** Meta object code from reading C++ file 'module.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "module.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'module.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Module[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,
      28,    7,    7,    7, 0x05,
      43,    7,    7,    7, 0x05,
      63,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Module[] = {
    "Module\0\0pos\0dragged(QPoint)\0removeModule()\0"
    "portSelected(Port*)\0portDisconnected()\0"
    "removeThisModule()\0"
};

void Module::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Module *_t = static_cast<Module *>(_o);
        switch (_id) {
        case 0: _t->dragged((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 1: _t->removeModule(); break;
        case 2: _t->portSelected((*reinterpret_cast< Port*(*)>(_a[1]))); break;
        case 3: _t->portDisconnected(); break;
        case 4: _t->removeThisModule(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Module::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Module::staticMetaObject = {
    { &Box::staticMetaObject, qt_meta_stringdata_Module,
      qt_meta_data_Module, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Module::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Module::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Module::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Module))
        return static_cast<void*>(const_cast< Module*>(this));
    return Box::qt_metacast(_clname);
}

int Module::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Box::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Module::dragged(QPoint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Module::removeModule()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Module::portSelected(Port * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Module::portDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
