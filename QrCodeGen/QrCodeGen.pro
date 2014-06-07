#-------------------------------------------------
#
# Project created by QtCreator 2013-04-15T20:45:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = QrCodeGen
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

DESTDIR = ../../deploy
