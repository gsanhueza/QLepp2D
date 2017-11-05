#-------------------------------------------------
#
# Project created by QtCreator 2017-10-30T11:10:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLepp2DGui
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
        about.cpp \
        tutorial.cpp \
        openglwidget.cpp

HEADERS += \
        mainwindow.h \
        about.h \
        tutorial.h \
        openglwidget.h

FORMS += \
        mainwindow.ui \
        about.ui \
        tutorial.ui

LIBS += -lOpenCL

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QLepp2DLib/release/ -lQLepp2DLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QLepp2DLib/debug/ -lQLepp2DLib
else:unix: LIBS += -L$$OUT_PWD/../QLepp2DLib/ -lQLepp2DLib

INCLUDEPATH += $$PWD/../QLepp2DLib
DEPENDPATH += $$PWD/../QLepp2DLib

# Useful when not installing the app, but testing it in a local environment
QMAKE_LFLAGS += -Wl,--rpath=$$OUT_PWD/../QLepp2DLib/

RESOURCES += \
    res/resources.qrc

unix {
        target.path = /usr/bin
        INSTALLS += target
}
