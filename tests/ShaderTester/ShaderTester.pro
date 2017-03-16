TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
SOURCES += main.cpp
TARGET=ShaderTester
QMAKE_CXXFLAGS += -DGLEW_STATIC


# this demo uses SDL so add the paths using the sdl2-config tool
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
