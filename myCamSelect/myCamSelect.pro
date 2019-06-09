#-------------------------------------------------
#
# Project created by QtCreator 2015-09-10T19:57:34
#
#-------------------------------------------------

QT       += core gui serialport multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myCamSelect
TEMPLATE = app

SUBDIRS += \
 gallery-core \
 gallery-desktop \
 gallery-mobile
 gallery-desktop.depends = gallery-core
 gallery-mobile.depends = gallery-core

INCLUDEPATH += /usr/include/opencv
LIBS += "-L / usr / lib / x86_64-linux-gnu" -lopencv_highgui -lopencv_core -lopencv_imgproc -lboost_system


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += \
    widget.ui
QMAKE_CXXFLAGS += -std=gnu++14

RESOURCES += \
    imagens.qrc \
    imagens.qrc





DISTFILES +=


