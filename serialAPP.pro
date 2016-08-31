#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T15:07:44
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialAPP
TEMPLATE = app


SOURCES += main.cpp\
        serial.cpp \
    aboutdialog.cpp \
    setdialog.cpp \
    helpdialog.cpp

HEADERS  += serial.h \
    aboutdialog.h \
    setdialog.h \
    helpdialog.h

FORMS    += serial.ui \
    aboutdialog.ui \
    setdialog.ui \
    helpdialog.ui

RESOURCES += \
    serialresource.qrc \
    about.qrc \
    sound.qrc
