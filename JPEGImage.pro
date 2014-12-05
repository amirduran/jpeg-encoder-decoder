#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T23:18:21
#
#-------------------------------------------------

TARGET = JPEGImage
TEMPLATE = lib

DEFINES += JPEGIMAGE_LIBRARY

SOURCES += jpegimage.cpp \
    component.cpp \
    huffmantable.cpp \
    quantizationtable.cpp \
    exportpicture.cpp \
    huffmanelementscount.cpp \
    jpegdecode.cpp \
    jpegencode.cpp

HEADERS += jpegimage.h\
        JPEGImage_global.h \
    component.h \
    huffmantable.h \
    quantizationtable.h \
    exportpicture.h \
    huffmanelementscount.h \
    jpegdecode.h \
    jpegencode.h

CONFIG += plugin debug

LIBS = -L../../imagelib-build-desktop/ -limagelib
INCLUDEPATH += ../../imagelib/include
DESTDIR      = ../../etfshop-build-desktop/plugins






















