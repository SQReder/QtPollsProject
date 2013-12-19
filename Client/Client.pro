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


LIBS += -L$$PWD/lib/ -lzxing
LIBS += -Lc:\cv\opencv\build\x86\mingw\lib
LIBS += -lopencv_core247.dll
LIBS += -lopencv_highgui247.dll -lopencv_imgproc247.dll

INCLUDEPATH += $$PWD/include
INCLUDEPATH += c:\cv\opencv\build\include
