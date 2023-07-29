QT       -= core gui

TARGET = piglerapi_qt
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../inc
HEADERS += ../inc/PiglerProtocol.h

INCLUDEPATH += inc
SOURCES += src/PiglerAPI.cpp
HEADERS += inc/PiglerAPI.h
