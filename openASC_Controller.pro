TEMPLATE = app
QT = gui \
    core \
    network \
    widgets
CONFIG += qt \
    warn_on
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
    ui/rotatorwindow.ui \
    ui/settingsdialog.ui \
    ui/terminaldialog.ui \
    ui/keypad.ui \
    ui/powermeterdialog.ui
HEADERS = src/mainwindowimpl.h \
    src/generic.h \
    src/main.h \
    src/rotatordialog.h \
    src/settingsdialog.h \
    src/commands.h \
    src/terminaldialog.h \
    src/tcpclass.h \
    src/keypad.h \
    src/powermeterdialog.h \
    src/wmvbusclient.h
SOURCES = src/mainwindowimpl.cpp \
    src/main.cpp \
    src/generic.cpp \
    src/rotatordialog.cpp \
    src/settingsdialog.cpp \
    src/terminaldialog.cpp \
    src/tcpclass.cpp \
    src/keypad.cpp \ 
    src/wmvbusclient.cpp \
    src/powermeterdialog.cpp

