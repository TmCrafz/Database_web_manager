#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T19:28:43
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Database_web_manager
TEMPLATE = app


SOURCES += main.cpp\        
        mainwindow.cpp \
    dialogdatabasemanager.cpp \
    xmlhandler.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    dialogdatabasemanager.h \
    xmlhandler.h \
    database.h

FORMS    += mainwindow.ui \
    dialogdatabasemanager.ui
