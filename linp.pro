#-------------------------------------------------
#
# Project created by QtCreator 2015-03-24T03:39:16
#
#-------------------------------------------------

QT += core gui widgets sql

TARGET = linp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    semantic.cpp \
    speech_to_text.cpp \
    linp_exception.cpp \
    linp_core.cpp

HEADERS  += mainwindow.h \
    database.h \
    semantic.h \
    speech_to_text.h \
    linp_exception.h \
    linp_core.h

FORMS    += mainwindow.ui
