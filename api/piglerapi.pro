#-------------------------------------------------
#
# Project created by QtCreator 2023-07-29T16:27:05
#
#-------------------------------------------------

QT       -= core gui

TARGET = piglerapi
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../inc
HEADERS += ../inc/PiglerProtocol.h

INCLUDEPATH += inc
SOURCES += src/PiglerAPI.cpp
HEADERS += inc/PiglerAPI.h
