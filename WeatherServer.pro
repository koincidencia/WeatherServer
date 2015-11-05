TEMPLATE = app

QT += qml quick
CONFIG += c++14

SOURCES += main.cpp \
    Application.cpp \
    SocketServer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    WeatherServer.pro.user

HEADERS += \
    Application.h \
    SocketServer.h

