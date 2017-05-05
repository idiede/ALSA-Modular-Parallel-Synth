/****************************************************************************
** Meta object code from reading C++ file 'prefwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "prefwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'prefwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PrefWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      51,   11,   11,   11, 0x0a,
      67,   11,   11,   11, 0x0a,
      82,   11,   11,   11, 0x0a,
      96,   11,   11,   11, 0x0a,
     121,   11,   11,   11, 0x0a,
     152,   11,   11,   11, 0x0a,
     179,   11,   11,   11, 0x0a,
     204,   11,   11,   11, 0x0a,
     224,   11,   11,   11, 0x0a,
     246,   11,   11,   11, 0x0a,
     265,   11,   11,   11, 0x0a,
     278,   11,   11,   11, 0x0a,
     291,   11,   11,   11, 0x0a,
     311,   11,   11,   11, 0x0a,
     334,   11,   11,   11, 0x0a,
     351,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PrefWidget[] = {
    "PrefWidget\0\0prefChanged()\0submitPref()\0"
    "applyPref()\0refreshColors()\0recallColors()\0"
    "storeColors()\0colorBackgroundClicked()\0"
    "colorModuleBackgroundClicked()\0"
    "colorModuleBorderClicked()\0"
    "colorModuleFontClicked()\0colorCableClicked()\0"
    "defaultcolorClicked()\0colorJackClicked()\0"
    "browseLoad()\0browseSave()\0updateMidiMode(int)\0"
    "updateEditingMode(int)\0loadPathUpdate()\0"
    "savePathUpdate()\0"
};

void PrefWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PrefWidget *_t = static_cast<PrefWidget *>(_o);
        switch (_id) {
        case 0: _t->prefChanged(); break;
        case 1: _t->submitPref(); break;
        case 2: _t->applyPref(); break;
        case 3: _t->refreshColors(); break;
        case 4: _t->recallColors(); break;
        case 5: _t->storeColors(); break;
        case 6: _t->colorBackgroundClicked(); break;
        case 7: _t->colorModuleBackgroundClicked(); break;
        case 8: _t->colorModuleBorderClicked(); break;
        case 9: _t->colorModuleFontClicked(); break;
        case 10: _t->colorCableClicked(); break;
        case 11: _t->defaultcolorClicked(); break;
        case 12: _t->colorJackClicked(); break;
        case 13: _t->browseLoad(); break;
        case 14: _t->browseSave(); break;
        case 15: _t->updateMidiMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->updateEditingMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->loadPathUpdate(); break;
        case 18: _t->savePathUpdate(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PrefWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PrefWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PrefWidget,
      qt_meta_data_PrefWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PrefWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PrefWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PrefWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PrefWidget))
        return static_cast<void*>(const_cast< PrefWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PrefWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void PrefWidget::prefChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
