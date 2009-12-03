TEMPLATE = app
QT = gui core
CONFIG += qt warn_on console debug
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui ui/submenu.ui
HEADERS = src/mainwindowimpl.h \
 src/generic.h \
 src/bandclass.h \
 src/main.h \
 src/posix_qextserialport.h \
 src/qextserialbase.h \
 src/qextserialport.h \
 src/sequencerclass.h \
 src/radiointerfaceclass.h \
 src/commclass.h \
 src/rxantennaclass.h \
 src/submenuimpl.h \
 src/external_input.h \
 src/settings.h \
 src/rotatorclass.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/generic.cpp \
 src/bandclass.cpp \
 src/posix_qextserialport.cpp \
 src/qextserialbase.cpp \
 src/qextserialport.cpp \
 src/sequencerclass.cpp \
 src/radiointerfaceclass.cpp \
 src/commclass.cpp \
 src/rxantennaclass.cpp \
 src/submenuimpl.cpp \
 src/external_input.cpp \
 src/settings.cpp \
 src/rotatorclass.cpp
