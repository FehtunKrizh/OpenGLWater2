#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T12:27:36
#
#-------------------------------------------------
CONFIG+=console
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Water
TEMPLATE = app


SOURCES += main.cpp \
    GLWindow.cpp

HEADERS  += \
    GLWindow.h

LIBS+=-lGLU

RESOURCES += \
    texture.qrc
