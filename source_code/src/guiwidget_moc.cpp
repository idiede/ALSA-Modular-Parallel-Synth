/****************************************************************************
** Meta object code from reading C++ file 'guiwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "guiwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'guiwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GuiWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      23,   10,   10,   10, 0x0a,
      35,   10,   10,   10, 0x0a,
      47,   10,   10,   10, 0x0a,
      65,   10,   10,   10, 0x0a,
      80,   10,   10,   10, 0x0a,
      91,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GuiWidget[] = {
    "GuiWidget\0\0presetDec()\0presetInc()\0"
    "addPreset()\0overwritePreset()\0"
    "clearPresets()\0clearGui()\0refreshGui()\0"
};

void GuiWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GuiWidget *_t = static_cast<GuiWidget *>(_o);
        switch (_id) {
        case 0: _t->presetDec(); break;
        case 1: _t->presetInc(); break;
        case 2: _t->addPreset(); break;
        case 3: _t->overwritePreset(); break;
        case 4: _t->clearPresets(); break;
        case 5: _t->clearGui(); break;
        case 6: _t->refreshGui(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData GuiWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GuiWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GuiWidget,
      qt_meta_data_GuiWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GuiWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GuiWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GuiWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GuiWidget))
        return static_cast<void*>(const_cast< GuiWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int GuiWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
