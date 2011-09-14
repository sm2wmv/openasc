/****************************************************************************
** Meta object code from reading C++ file 'mainwindowimpl.h'
**
** Created: Wed Sep 14 21:55:08 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindowimpl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowimpl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindowImpl[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      42,   15,   15,   15, 0x0a,
      67,   15,   15,   15, 0x0a,
     101,   95,   15,   15, 0x0a,
     128,   95,   15,   15, 0x0a,
     155,   95,   15,   15, 0x0a,
     182,   95,   15,   15, 0x0a,
     215,  209,   15,   15, 0x0a,
     245,   95,   15,   15, 0x0a,
     275,   95,   15,   15, 0x0a,
     305,   95,   15,   15, 0x0a,
     335,   95,   15,   15, 0x0a,
     365,   95,   15,   15, 0x0a,
     395,   95,   15,   15, 0x0a,
     425,   95,   15,   15, 0x0a,
     455,   95,   15,   15, 0x0a,
     485,   95,   15,   15, 0x0a,
     515,   95,   15,   15, 0x0a,
     546,   95,   15,   15, 0x0a,
     575,   15,   15,   15, 0x0a,
     600,   15,   15,   15, 0x0a,
     624,   15,   15,   15, 0x0a,
     654,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindowImpl[] = {
    "MainWindowImpl\0\0WindowRotatorsTriggered()\0"
    "actionConnectTriggered()\0"
    "actionDisconnectTriggered()\0state\0"
    "pushButtonTX1Clicked(bool)\0"
    "pushButtonTX2Clicked(bool)\0"
    "pushButtonTX3Clicked(bool)\0"
    "pushButtonTX4Clicked(bool)\0index\0"
    "comboBoxBandIndexChanged(int)\0"
    "pushButtonRXAnt1Clicked(bool)\0"
    "pushButtonRXAnt2Clicked(bool)\0"
    "pushButtonRXAnt3Clicked(bool)\0"
    "pushButtonRXAnt4Clicked(bool)\0"
    "pushButtonRXAnt5Clicked(bool)\0"
    "pushButtonRXAnt6Clicked(bool)\0"
    "pushButtonRXAnt7Clicked(bool)\0"
    "pushButtonRXAnt8Clicked(bool)\0"
    "pushButtonRXAnt9Clicked(bool)\0"
    "pushButtonRXAnt10Clicked(bool)\0"
    "pushButtonRXAntClicked(bool)\0"
    "timerPollRXQueueUpdate()\0"
    "timerPollStatusUpdate()\0"
    "actionSettingsEditTriggered()\0"
    "lineEditCommandPressed()\0"
};

const QMetaObject MainWindowImpl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindowImpl,
      qt_meta_data_MainWindowImpl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindowImpl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindowImpl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindowImpl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowImpl))
        return static_cast<void*>(const_cast< MainWindowImpl*>(this));
    if (!strcmp(_clname, "Ui::MainWindowImpl"))
        return static_cast< Ui::MainWindowImpl*>(const_cast< MainWindowImpl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindowImpl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: WindowRotatorsTriggered(); break;
        case 1: actionConnectTriggered(); break;
        case 2: actionDisconnectTriggered(); break;
        case 3: pushButtonTX1Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: pushButtonTX2Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: pushButtonTX3Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: pushButtonTX4Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: comboBoxBandIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: pushButtonRXAnt1Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: pushButtonRXAnt2Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: pushButtonRXAnt3Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: pushButtonRXAnt4Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: pushButtonRXAnt5Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: pushButtonRXAnt6Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: pushButtonRXAnt7Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: pushButtonRXAnt8Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: pushButtonRXAnt9Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: pushButtonRXAnt10Clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: pushButtonRXAntClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: timerPollRXQueueUpdate(); break;
        case 20: timerPollStatusUpdate(); break;
        case 21: actionSettingsEditTriggered(); break;
        case 22: lineEditCommandPressed(); break;
        default: ;
        }
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
