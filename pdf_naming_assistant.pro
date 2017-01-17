#-------------------------------------------------
#
# Project created by QtCreator 2016-02-13T00:54:58
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdf_naming_assistant
TEMPLATE = app


SOURCES += main.cpp \
    widget_test.cpp \
    widget_main.cpp \
    widget_list.cpp \
    widget_document.cpp \
    widget_constructor.cpp \
    widget_settings.cpp \
    core_pattern_manager.cpp

LIBS += -lpoppler-qt5

HEADERS  += \
    widget_test.h \
    widget_main.h \
    widget_list.h \
    widget_document.h \
    widget_constructor.h \
    widget_settings.h \
    core_pattern_manager.h
