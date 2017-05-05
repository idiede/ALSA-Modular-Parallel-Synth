/****************************************************************************
** Meta object code from reading C++ file 'midiwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "midiwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'midiwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiControllerModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_MidiControllerModel[] = {
    "MidiControllerModel\0"
};

void MidiControllerModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MidiControllerModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MidiControllerModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_MidiControllerModel,
      qt_meta_data_MidiControllerModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiControllerModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiControllerModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiControllerModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiControllerModel))
        return static_cast<void*>(const_cast< MidiControllerModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int MidiControllerModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ModuleModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_ModuleModel[] = {
    "ModuleModel\0"
};

void ModuleModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ModuleModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuleModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_ModuleModel,
      qt_meta_data_ModuleModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuleModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuleModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuleModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuleModel))
        return static_cast<void*>(const_cast< ModuleModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int ModuleModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_MidiWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      30,   11,   11,   11, 0x0a,
      45,   11,   11,   11, 0x0a,
      59,   11,   11,   11, 0x0a,
      87,   11,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     141,   11,   11,   11, 0x0a,
     162,   11,   11,   11, 0x0a,
     199,  179,   11,   11, 0x0a,
     248,  179,   11,   11, 0x0a,
     301,  298,   11,   11, 0x0a,
     318,   11,   11,   11, 0x0a,
     330,   11,   11,   11, 0x0a,
     342,   11,   11,   11, 0x0a,
     367,  361,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MidiWidget[] = {
    "MidiWidget\0\0clearAllClicked()\0"
    "clearClicked()\0bindClicked()\0"
    "addToParameterViewClicked()\0"
    "noteControllerCheckToggle(int)\0"
    "configCheckToggle(int)\0midiCheckToggle(int)\0"
    "toggleMidiSign()\0selected,deselected\0"
    "guiControlChanged(QItemSelection,QItemSelection)\0"
    "midiControlChanged(QItemSelection,QItemSelection)\0"
    "on\0setLogMode(bool)\0setNewMin()\0"
    "setNewMax()\0setInitialMinMax()\0index\0"
    "updateMidiChannel(int)\0"
};

void MidiWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MidiWidget *_t = static_cast<MidiWidget *>(_o);
        switch (_id) {
        case 0: _t->clearAllClicked(); break;
        case 1: _t->clearClicked(); break;
        case 2: _t->bindClicked(); break;
        case 3: _t->addToParameterViewClicked(); break;
        case 4: _t->noteControllerCheckToggle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->configCheckToggle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->midiCheckToggle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->toggleMidiSign(); break;
        case 8: _t->guiControlChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 9: _t->midiControlChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1])),(*reinterpret_cast< const QItemSelection(*)>(_a[2]))); break;
        case 10: _t->setLogMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->setNewMin(); break;
        case 12: _t->setNewMax(); break;
        case 13: _t->setInitialMinMax(); break;
        case 14: _t->updateMidiChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MidiWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MidiWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MidiWidget,
      qt_meta_data_MidiWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiWidget))
        return static_cast<void*>(const_cast< MidiWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MidiWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
