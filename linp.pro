#-------------------------------------------------
#
# Project created by QtCreator 2015-03-24T03:39:16
#
#-------------------------------------------------

QT       += core gui

QT += sql

TARGET = linp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    semantic.cpp \
    speech_to_text.cpp \
    application.cpp \
    linp_exception.cpp

HEADERS  += mainwindow.h \
    database.h \
    semantic.h \
    speech_to_text.h \
    application.h \
    linp_exception.h

FORMS    += mainwindow.ui
