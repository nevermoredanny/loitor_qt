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
    clickablelabel.cpp \
    algprocess.cpp

HEADERS  += mainwindow.h \
    camera_loitor.h \
    clickablelabel.h \
    algprocess.h

FORMS    += mainwindow.ui

#loitor
THIRD_PARTIES = ./third_parties
INCLUDEPATH += $${THIRD_PARTIES}/loitor/include
LIBS +=  ../LOITOR_QT/third_parties/loitor/lib/libloitorusbcam.a
LIBS +=  ../LOITOR_QT/third_parties/loitor/lib/libloitorimu.a
LIBS += -lpthread
LIBS += -lusb-1.0

#opencv
INCLUDEPATH += /usr/local/include/opencv \
                /usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_video.so  \
        /usr/local/lib/libopencv_objdetect.so \
        /usr/local/lib/libopencv_ml.so  \
        /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_features2d.so  \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_flann.so   \
        /usr/local/lib/libopencv_calib3d.so \
        /usr/local/lib/libopencv_imgcodecs.so


#boost
LIBS += -lboost_thread \
        -lboost_system
