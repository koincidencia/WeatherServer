TEMPLATE = app

QT += qml quick
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++14

CONFIG += static

SOURCES += main.cpp \
    Application.cpp \
    SocketServer.cpp \
    qcustomplot.cpp \
    mainwindow.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    WeatherServer.pro.user

HEADERS += \
    Application.h \
    SocketServer.h \
    qcustomplot.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

