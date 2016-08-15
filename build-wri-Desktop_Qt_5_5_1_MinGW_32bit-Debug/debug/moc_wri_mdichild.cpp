/****************************************************************************
** Meta object code from reading C++ file 'wri_mdichild.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/wri_mdichild.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wri_mdichild.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_wRI_MdiChild_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_wRI_MdiChild_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_wRI_MdiChild_t qt_meta_stringdata_wRI_MdiChild = {
    {
QT_MOC_LITERAL(0, 0, 12) // "wRI_MdiChild"

    },
    "wRI_MdiChild"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_wRI_MdiChild[] = {

 // content:
       7,       // revision
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

void wRI_MdiChild::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject wRI_MdiChild::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_wRI_MdiChild.data,
      qt_meta_data_wRI_MdiChild,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *wRI_MdiChild::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *wRI_MdiChild::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_wRI_MdiChild.stringdata0))
        return static_cast<void*>(const_cast< wRI_MdiChild*>(this));
    return QDialog::qt_metacast(_clname);
}

int wRI_MdiChild::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
