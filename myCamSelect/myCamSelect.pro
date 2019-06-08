#-------------------------------------------------
#
# Project created by QtCreator 2015-09-10T19:57:34
#
#-------------------------------------------------

QT       += core gui serialport multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myCamSelect
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
QMAKE_CXXFLAGS += -std=gnu++14

RESOURCES += \
    imagens.qrc \
    imagens.qrc

DISTFILES +=
