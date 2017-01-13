#############
QT += opengl

TARGET = pc_ahrs

TEMPLATE = app

SOURCES += src/main.cpp
SOURCES += src/mainwindow.cpp
SOURCES += src/objectgl.cpp
SOURCES += src/bhsocket.cpp
SOURCES += src/roc_timer.cpp
SOURCES += src/rOc_serial.cpp
SOURCES += src/MadgwickAHRS.cpp

HEADERS += include/mainwindow.h
HEADERS += include/AHRS.h
HEADERS += include/MadgwickAHRS.h
HEADERS += include/objectgl.h
HEADERS += include/rOc_Timer.h
HEADERS += include/bhsocket.h
HEADERS += include/rOc_serial.h

INCLUDEPATH += src
INCLUDEPATH += include

OBJECTS_DIR = tmp/
MOC_DIR = tmp/
DESTDIR = bin/
