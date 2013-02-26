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
 src/win_qextserialport.h \
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
 src/win_qextserialport.cpp \
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

OTHER_FILES += \
    android/version.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/AndroidManifest.xml \
    android/res/values-it/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values-ru/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/layout/splash.xml \
    android/res/values-ms/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-id/strings.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/values-ro/strings.xml
