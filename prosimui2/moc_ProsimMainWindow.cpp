/****************************************************************************
** Meta object code from reading C++ file 'ProsimMainWindow.h'
**
** Created: Wed Sep 23 15:58:16 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ProsimMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProsimMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProsimMainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x08,
      57,   18,   17,   17, 0x08,
      93,   18,   17,   17, 0x08,
     128,   18,   17,   17, 0x08,
     162,   17,   17,   17, 0x08,
     193,   17,   17,   17, 0x08,
     229,   17,   17,   17, 0x08,
     263,   17,   17,   17, 0x08,
     295,   17,   17,   17, 0x08,
     332,   17,   17,   17, 0x08,
     367,   17,   17,   17, 0x08,
     390,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProsimMainWindow[] = {
    "ProsimMainWindow\0\0value\0"
    "on_algorithmModes_activated(int)\0"
    "on_quantumSpinBox_valueChanged(int)\0"
    "on_expirySpinBox_valueChanged(int)\0"
    "on_simulationModes_activated(int)\0"
    "on_inputFileDialogue_clicked()\0"
    "on_inputFileTextBox_returnPressed()\0"
    "on_inputFileTextBox_textChanged()\0"
    "on_outputFileDialogue_clicked()\0"
    "on_outputFileTextBox_returnPressed()\0"
    "on_outputFileTextBox_textChanged()\0"
    "on_runButton_clicked()\0on_resetButton_clicked()\0"
};

const QMetaObject ProsimMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ProsimMainWindow,
      qt_meta_data_ProsimMainWindow, 0 }
};

const QMetaObject *ProsimMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *ProsimMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProsimMainWindow))
        return static_cast<void*>(const_cast< ProsimMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ProsimMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_algorithmModes_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_quantumSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_expirySpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: on_simulationModes_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_inputFileDialogue_clicked(); break;
        case 5: on_inputFileTextBox_returnPressed(); break;
        case 6: on_inputFileTextBox_textChanged(); break;
        case 7: on_outputFileDialogue_clicked(); break;
        case 8: on_outputFileTextBox_returnPressed(); break;
        case 9: on_outputFileTextBox_textChanged(); break;
        case 10: on_runButton_clicked(); break;
        case 11: on_resetButton_clicked(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
