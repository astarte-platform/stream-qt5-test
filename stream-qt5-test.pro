TEMPLATE = app
TARGET = stream-qt5-test
INCLUDEPATH += .
INCLUDEPATH += /usr/include/AstarteDeviceSDKQt5

CONFIG += console c++11
QT -= gui

LIBS += -lmosquitto
LIBS += -lmosquittopp
LIBS += -lAstarteDeviceSDKQt5

HEADERS += AstarteStreamQt5Test.h AstarteStreamTelemetry.h

SOURCES += main.cpp AstarteStreamQt5Test.cpp AstarteStreamTelemetry.cpp
