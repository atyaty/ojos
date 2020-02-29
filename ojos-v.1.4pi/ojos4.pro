#-------------------------------------------------
#
# Project created by QtCreator 2016-09-16T21:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ojos2
TEMPLATE = app

INCLUDEPATH += /usr/include/openCV\
               /usr/include/openCV2\
               /usr/include/c++/7\
               ../atlib

#LIBS+=-lopencv_core -lopencv_highgui -lopencv_imgproc
LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp\
        mainwindow.cpp \
    at_kamera.cpp \
    at_majak.cpp \
    at_omam.cpp \
    at_oczy.cpp \
    at_omamy.cpp \
    ../atlib/atFile.cpp

HEADERS  += mainwindow.h \
    at_kamera.h \
    at_majak.h \
    at_omam.h \
    at_oczy.h \
    at_omamy.h \
    ../atlib/atFile.h

FORMS    += mainwindow.ui
