TARGET=ezPPTesting
OBJECTS_DIR=obj
QMAKE_CXXFLAGS += -DGLEW_STATIC
LIBS += -L/usr/local/lib/ -lGLEW
#Including the ezPP library
INCLUDEPATH += ../ezPP/include
DEPENDPATH += ../ezPP/include
LIBS += -L ../ezPP/lib -lezPP
QMAKE_RPATHDIR += $$PWD/../../ezPP/lib


CONFIG += console

LIBS+=-lgtest

SOURCES += \
    main.cpp \
    eztesteffect.cpp

HEADERS += \
    eztesteffect.h
