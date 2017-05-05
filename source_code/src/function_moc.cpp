/****************************************************************************
** Meta object code from reading C++ file 'function.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "function.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'function.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Function[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   30,    9,    9, 0x0a,
      63,   56,    9,    9, 0x0a,
      78,   30,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Function[] = {
    "Function\0\0,\0mousePos(int,int)\0index\0"
    "updateFunction(int)\0p_zoom\0setZoom(float)\0"
    "highlightFunction(int)\0"
};

void Function::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Function *_t = static_cast<Function *>(_o);
        switch (_id) {
        case 0: _t->mousePos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->updateFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setZoom((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->highlightFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Function::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Function::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_Function,
      qt_meta_data_Function, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Function::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Function::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Function::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Function))
        return static_cast<void*>(const_cast< Function*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int Function::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Function::mousePos(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
