#-------------------------------------------------
#
# Project created by QtCreator 2013-04-24T04:00:56
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
    connectDialog.cpp \
    scanwindow.cpp \
    clientconnection.cpp \
    scanworker.cpp \
    scrollgrid.cpp \
    clicablelabel.cpp \
    confirmvote.cpp
#    ../Server/pollsclient.cpp

HEADERS  += \
    connectDialog.h \
    scanwindow.h \
    clientconnection.h \
    scanworker.h \
    scrollgrid.h \
    clicablelabel.h \
    confirmvote.h

FORMS    += \
    connectDialog.ui \
    scanwindow.ui \
    scrollgrid.ui \
    confirmvote.ui

RESOURCES += \
    rc.qrc

QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS += --std=c++11 -O3

OTHER_FILES +=

unix:!macx|win32: LIBS += -L$$PWD/lib/ -llibzbar-0

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
