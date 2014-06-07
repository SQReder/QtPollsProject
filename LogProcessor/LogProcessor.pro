#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T06:49:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogProcessor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logitem.cpp

HEADERS  += mainwindow.h \
    logitem.h

FORMS    += mainwindow.ui

CONFIG += c++11
