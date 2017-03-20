#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T20:01:57
#
#-------------------------------------------------

QT       += testlib opengl

QT       -= gui

TARGET = ezPPtest
CONFIG   += console
CONFIG-=app_bundle

SOURCES += tst_eztest.cpp \
    mockezpper.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

#Including the ezPP library
INCLUDEPATH += ../ezPP/include
DEPENDPATH += ../ezPP/include
LIBS += -L ../ezPP/lib -lezPP
QMAKE_RPATHDIR += $$PWD/../ezPP/lib


QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

linux: {
  LIBS += $$system(sdl2-config --libs)
  LIBS += -L/usr/local/lib/ -lSDL2 -lSDL2_image -lGLEW
}

macx: {
  QMAKE_MAC_SDK = macosx10.12
  DEFINES+=MAC_OS_X_VERSION_MIN_REQUIRED=1060
  QMAKE_LFLAGS += -F/Library/Frameworks
  LIBS += -framework SDL2
  LIBS += -framework SDL2_image
  INCLUDEPATH += /Library/Frameworks/SDL2_image.framework/Headers
  INCLUDEPATH += /Library/Frameworks/SDL2.framework/Headers
  INCLUDEPATH += /usr/local/include
  LIBS+= -framework OpenGL
}

OBJECTS_DIR=obj

HEADERS += \
    mockezpper.h

