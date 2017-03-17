#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T13:27:08
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LOITOR_QT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera_loitor.cpp \
    clickablelabel.cpp

HEADERS  += mainwindow.h \
    camera_loitor.h \
    clickablelabel.h

FORMS    += mainwindow.ui

#loitor
THIRD_PARTIES = ./third_parties
INCLUDEPATH += $${THIRD_PARTIES}/loitor/include
LIBS +=  ../LOITOR_QT/third_parties/loitor/lib/libloitorusbcam.a
LIBS +=  ../LOITOR_QT/third_parties/loitor/lib/libloitorimu.a
LIBS += -lpthread
LIBS += -lusb-1.0

RESOURCES +=

