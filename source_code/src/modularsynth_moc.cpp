/****************************************************************************
** Meta object code from reading C++ file 'modularsynth.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "modularsynth.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modularsynth.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModularSynth[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      29,   13,   13,   13, 0x0a,
      53,   13,   13,   13, 0x0a,
      76,   13,   13,   13, 0x0a,
     100,   97,   13,   13, 0x0a,
     116,   13,   13,   13, 0x0a,
     129,   13,   13,   13, 0x0a,
     141,   13,   13,   13, 0x0a,
     156,   13,   13,   13, 0x0a,
     169,   13,   13,   13, 0x0a,
     180,   13,   13,   13, 0x0a,
     191,   13,   13,   13, 0x0a,
     205,   13,   13,   13, 0x0a,
     216,   13,   13,   13, 0x0a,
     231,   13,   13,   13, 0x0a,
     246,   13,   13,   13, 0x0a,
     257,   13,   13,   13, 0x0a,
     268,   13,   13,   13, 0x0a,
     281,   13,   13,   13, 0x0a,
     296,   13,   13,   13, 0x0a,
     308,   13,   13,   13, 0x0a,
     319,   13,   13,   13, 0x0a,
     332,   13,   13,   13, 0x0a,
     345,   13,   13,   13, 0x0a,
     358,   13,   13,   13, 0x0a,
     372,   13,   13,   13, 0x0a,
     385,   13,   13,   13, 0x0a,
     399,  395,   13,   13, 0x0a,
     422,   13,   13,   13, 0x0a,
     442,   13,   13,   13, 0x0a,
     457,   13,   13,   13, 0x0a,
     477,  474,   13,   13, 0x0a,
     503,   13,   13,   13, 0x0a,
     522,   13,   13,   13, 0x0a,
     541,   13,   13,   13, 0x0a,
     554,   13,   13,   13, 0x0a,
     567,   13,   13,   13, 0x0a,
     582,   13,   13,   13, 0x0a,
     608,   13,   13,   13, 0x0a,
     638,   13,   13,   13, 0x0a,
     654,   13,   13,   13, 0x0a,
     678,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ModularSynth[] = {
    "ModularSynth\0\0displayAbout()\0"
    "displayMidiController()\0displayParameterView()\0"
    "displayPreferences()\0fd\0midiAction(int)\0"
    "startSynth()\0stopSynth()\0new_textEdit()\0"
    "newM_seq_8()\0newM_env()\0newM_mcv()\0"
    "newM_advmcv()\0newM_vco()\0newM_vca_lin()\0"
    "newM_vca_exp()\0newM_vcf()\0newM_lfo()\0"
    "newM_noise()\0newM_ringmod()\0newM_fsin()\0"
    "newM_inv()\0newM_mix_2()\0newM_mix_4()\0"
    "newM_mix_8()\0newM_pcmout()\0newM_pcmin()\0"
    "newM_sh()\0pos\0resizeTextEdit(QPoint)\0"
    "portSelected(Port*)\0deleteModule()\0"
    "deleteTextEdit()\0te\0deleteTextEdit(TextEdit*)\0"
    "load(QTextStream&)\0save(QTextStream&)\0"
    "loadColors()\0saveColors()\0allVoicesOff()\0"
    "loadPreferences(QString&)\0"
    "savePreferences(QTextStream&)\0"
    "refreshColors()\0redrawPortConnections()\0"
    "updateColors()\0"
};

void ModularSynth::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModularSynth *_t = static_cast<ModularSynth *>(_o);
        switch (_id) {
        case 0: _t->displayAbout(); break;
        case 1: _t->displayMidiController(); break;
        case 2: _t->displayParameterView(); break;
        case 3: _t->displayPreferences(); break;
        case 4: _t->midiAction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->startSynth(); break;
        case 6: _t->stopSynth(); break;
        case 7: _t->new_textEdit(); break;
        case 8: _t->newM_seq_8(); break;
        case 9: _t->newM_env(); break;
        case 10: _t->newM_mcv(); break;
        case 11: _t->newM_advmcv(); break;
        case 12: _t->newM_vco(); break;
        case 13: _t->newM_vca_lin(); break;
        case 14: _t->newM_vca_exp(); break;
        case 15: _t->newM_vcf(); break;
        case 16: _t->newM_lfo(); break;
        case 17: _t->newM_noise(); break;
        case 18: _t->newM_ringmod(); break;
        case 19: _t->newM_fsin(); break;
        case 20: _t->newM_inv(); break;
        case 21: _t->newM_mix_2(); break;
        case 22: _t->newM_mix_4(); break;
        case 23: _t->newM_mix_8(); break;
        case 24: _t->newM_pcmout(); break;
        case 25: _t->newM_pcmin(); break;
        case 26: _t->newM_sh(); break;
        case 27: _t->resizeTextEdit((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 28: _t->portSelected((*reinterpret_cast< Port*(*)>(_a[1]))); break;
        case 29: _t->deleteModule(); break;
        case 30: _t->deleteTextEdit(); break;
        case 31: _t->deleteTextEdit((*reinterpret_cast< TextEdit*(*)>(_a[1]))); break;
        case 32: _t->load((*reinterpret_cast< QTextStream(*)>(_a[1]))); break;
        case 33: _t->save((*reinterpret_cast< QTextStream(*)>(_a[1]))); break;
        case 34: _t->loadColors(); break;
        case 35: _t->saveColors(); break;
        case 36: _t->allVoicesOff(); break;
        case 37: _t->loadPreferences((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 38: _t->savePreferences((*reinterpret_cast< QTextStream(*)>(_a[1]))); break;
        case 39: _t->refreshColors(); break;
        case 40: _t->redrawPortConnections(); break;
        case 41: _t->updateColors(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModularSynth::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModularSynth::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ModularSynth,
      qt_meta_data_ModularSynth, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModularSynth::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModularSynth::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModularSynth::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModularSynth))
        return static_cast<void*>(const_cast< ModularSynth*>(this));
    return QWidget::qt_metacast(_clname);
}

int ModularSynth::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 42)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 42;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
