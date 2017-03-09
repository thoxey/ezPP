#---------------------------------------------------------------------------
#Adapted From Jon Macey's original by Tom Hoxey
#---------------------------------------------------------------------------
# This specifies the exe name
TARGET=ezPPSDLTextured
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core

# as I want to support 4.8 and 5 this will set a flag for some of the mac stuff
# mainly in the types.h file for the setMacVisual which is native in Qt5
isEqual(QT_MAJOR_VERSION, 5) {
	cache()
	DEFINES +=QT5BUILD
}

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
}

INCLUDEPATH =+ ../SOIL/src
LIBS += -L ../SOIL/lib -lSOIL

# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/*.cpp
# same for the .h files
HEADERS+= $$PWD/include/*.h
# and add the include dir into the search path for Qt and make
INCLUDEPATH += $$PWD/include
#Including the ezPP library, this is a lazy link since its a packaged demo  --TH
INCLUDEPATH += ../../ezPP/include
DEPENDPATH += ../../ezPP/include
LIBS += -L ../../ezPP/lib -lezPP
QMAKE_RPATHDIR += $$PWD/../../ezPP/lib

# were are going to default to a console app
CONFIG += console
