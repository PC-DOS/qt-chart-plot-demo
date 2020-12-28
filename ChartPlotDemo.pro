#-------------------------------------------------
#
# Project created by QtCreator 2020-12-21T11:29:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ChartPlotDemo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    DataSourceProvider.cpp \
    StateMachine.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    DataSourceProvider.h \
    StateMachine.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS_DEBUG += -O2
QMAKE_CXXFLAGS_RELEASE += -O2
