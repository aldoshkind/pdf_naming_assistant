#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T00:54:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = poppler
TEMPLATE = app


SOURCES += main.cpp \
    widget_test.cpp \
    widget_main.cpp \
    widget_list.cpp \
    widget_document.cpp \
    widget_constructor.cpp

LIBS += -lpoppler-qt5

HEADERS  += \
    widget_test.h \
    widget_main.h \
    widget_list.h \
    widget_document.h \
    widget_constructor.h
