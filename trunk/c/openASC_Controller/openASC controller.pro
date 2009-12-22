TEMPLATE = app
QT = gui \
    core
CONFIG += qt \
    warn_on \
    console \
    debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
    ui/rotatorwindow.ui \
    ui/mapviewwidget.ui \
    ui/settingsdialog.ui
HEADERS = src/mainwindowimpl.h \
    src/generic.h \
    src/main.h \
    src/commclass.h \
    src/posix_qextserialport.h \
    src/qextserialbase.h \
    src/qextserialport.h \
    src/rotatordialog.h \
    src/mapviewwidget.h \
    src/settingsdialog.h
SOURCES = src/mainwindowimpl.cpp \
    src/main.cpp \
    src/generic.cpp \
    src/posix_qextserialport.cpp \
    src/qextserialbase.cpp \
    src/qextserialport.cpp \
    src/rotatordialog.cpp \
    src/mapviewwidget.cpp \
    src/commclass.cpp \
    src/settingsdialog.cpp
