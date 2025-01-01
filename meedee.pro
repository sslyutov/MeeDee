TEMPLATE = app

TARGET = MeeDee

VERSION = 1.0.0

DEFINES += APP_VERSION=\\\"$${VERSION}\\\"

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += ios

CONFIG += debug release

CONFIG += c++17

CONFIG +=sdk_no_version_check

CONFIG += device

SRCROOT = ./
INCROOT = ./
FRMROOT = ./

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    $${SRCROOT}/about/about.cpp \
    mididevman/mididevman.cpp
    $${SRCROOT}/mididevman/mididevman.cpp
#    mainwindow.cpp

HEADERS += \
    $${INCROOT}/about/about.h \
    mididevman/mididevman.h
    $${INCROOT}/mididevman/mididevman.h
#    mainwindow.h

FORMS += \
    $${FRMROOT}/about/about.ui

INCLUDEPATH += $${INCROOT}
INCLUDEPATH += $${INCROOT}/about

LIBS += -framework CoreMIDI

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
