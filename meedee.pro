TEMPLATE = app

TARGET = MeeDee

VERSION = 1.0.0

DEFINES += APP_VERSION=\\\"$${VERSION}\\\"

QT += core gui concurrent widgets

CONFIG += ios

CONFIG += debug release

CONFIG += c++17

CONFIG +=sdk_no_version_check

CONFIG += device

CONFIG += no_autoqmake

QMAKE_INFO_PLIST = Info.plist

SRCROOT = ./
INCROOT = ./
FRMROOT = ./

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $${SRCROOT}/main.cpp
SOURCES += $${SRCROOT}/mainwindow/mainwindow.cpp
SOURCES += $${SRCROOT}/about/about.cpp
SOURCES += $${SRCROOT}/utils/utils.cpp
SOURCES += $${SRCROOT}/midiinputport/midiinputport.cpp
SOURCES += $${SRCROOT}/midioutputport/midioutputport.cpp
SOURCES += $${SRCROOT}/midirecorder/midirecorder.cpp
SOURCES += $${SRCROOT}/midiplayer/midiplayer.cpp
SOURCES += $${SRCROOT}/midiplaybackrecorder/midiplaybackrecorder.cpp

HEADERS += $${INCROOT}/mainwindow/mainwindow.h
HEADERS += $${INCROOT}/about/about.h
HEADERS += $${INCROOT}/utils/utils.h
HEADERS += $${INCROOT}/midiinputport/midiinputport.h
HEADERS += $${INCROOT}/lighthouse/lighthouse.h
HEADERS += $${INCROOT}/midioutputport/midioutputport.h
HEADERS += $${INCROOT}/midirecorder/midirecorder.h
HEADERS += $${INCROOT}/midiplayer/midiplayer.h
HEADERS += $${INCROOT}/midiplaybackrecorder/midiplaybackrecorder.h

#interfaces
HEADERS += $${INCROOT}/midirecorder/imidirecorder.h

FORMS += $${FRMROOT}/about/about.ui
FORMS += $${FRMROOT}/midiplaybackrecorder/midiplaybackrecorder.ui

INCLUDEPATH += $${INCROOT}
INCLUDEPATH += $${INCROOT}/mainwindow
INCLUDEPATH += $${INCROOT}/about
INCLUDEPATH += $${INCROOT}/utils
INCLUDEPATH += $${INCROOT}/lighthouse
INCLUDEPATH += $${INCROOT}/midiinputport
INCLUDEPATH += $${INCROOT}/midioutputport
INCLUDEPATH += $${INCROOT}/midirecorder
INCLUDEPATH += $${INCROOT}/midiplayer
INCLUDEPATH += $${INCROOT}/midiplaybackrecorder

RESOURCES += meedee.qrc

mac{
    LIBS += -framework CoreMIDI
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
