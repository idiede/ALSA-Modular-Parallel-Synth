/****************************************************************************
** Meta object code from reading C++ file 'midisliderbase.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "midisliderbase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midisliderbase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiSliderBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MidiSliderBase[] = {
    "MidiSliderBase\0\0value\0valueChanged(int)\0"
};

void MidiSliderBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MidiSliderBase *_t = static_cast<MidiSliderBase *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MidiSliderBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MidiSliderBase::staticMetaObject = {
    { &MidiGUIcomponent::staticMetaObject, qt_meta_stringdata_MidiSliderBase,
      qt_meta_data_MidiSliderBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiSliderBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiSliderBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiSliderBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiSliderBase))
        return static_cast<void*>(const_cast< MidiSliderBase*>(this));
    return MidiGUIcomponent::qt_metacast(_clname);
}

int MidiSliderBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MidiGUIcomponent::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
