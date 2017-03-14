TARGET=ezPPTesting
OBJECTS_DIR=obj
QMAKE_CXXFLAGS += -DGLEW_STATIC
#Including the ezPP library
INCLUDEPATH += ../ezPP/include
DEPENDPATH += ../ezPP/include
LIBS += -L ../ezPP/lib -lezPP
QMAKE_RPATHDIR += $$PWD/../../ezPP/lib

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

linux: {
  LIBS += $$system(sdl2-config --libs)
  LIBS += -L/usr/local/lib/ -lSDL2 -lSDL2_image -lGLEW
}

CONFIG += console

LIBS+=-lgtest

SOURCES += \
    main.cpp \
    eztesteffect.cpp

HEADERS += \
    eztesteffect.h
