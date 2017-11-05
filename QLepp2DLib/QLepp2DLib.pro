QT       -= gui

TARGET = QLepp2DLib
TEMPLATE = lib

DEFINES += QLEPP2DLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        model.cpp \
        off/offhandler.cpp \
        engine/cpuengine.cpp \
        engine/openclengine.cpp

HEADERS += \
        qlepp2dlib_global.h \
        model.h \
        structs/triangle.h \
        structs/vertex.h \
        structs/edge.h \
        engine/abstractengine.h \
        engine/cpuengine.h \
        engine/openclengine.h \
        off/offhandler.h \
        off/offmetadata.h

RESOURCES += \
        res/libresources.qrc

LIBS += -lOpenCL

unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr/lib
    }
    target.path = $$PREFIX
    INSTALLS += target
}
