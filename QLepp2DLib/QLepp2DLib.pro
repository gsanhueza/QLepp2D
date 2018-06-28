QT       -= gui

TARGET = qlepp2d-lib
TEMPLATE = lib

DEFINES += QLEPP2DLIB_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you
# use any feature of Qt which has been marked as deprecated (the
# exact warnings depend on your compiler). Please consult the
# documentation of the deprecated API in order to know how to
# port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use
# deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a
# certain version of Qt.

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        filehandlers/filemanager.cpp \
        filehandlers/offhandler.cpp \
        engine/cpuengine.cpp \
        engine/openclengine.cpp \
        model.cpp \
        model_impl.cpp

HEADERS += \
        qlepp2dlib_global.h \
        structs/triangle.h \
        structs/vertex.h \
        structs/edge.h \
        engine/cpuengine.h \
        engine/openclengine.h \
        filehandlers/filemanager.h \
        filehandlers/filehandler.h \
        filehandlers/offhandler.h \
        model.h \
        model_impl.h \
        engine/engine.h \

# Installable headers
header_files.path   = /usr/include/QLepp2D
header_files.files += model.h \
                      qlepp2dlib_global.h \
                      structs/

INSTALLS += header_files

RESOURCES += \
        res/libresources.qrc

unix:!macx {
    LIBS += -lOpenCL
}

macx: {
    LIBS += -framework OpenCL
}

unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr/lib
    }
    target.path = $$PREFIX
    INSTALLS += target
}
