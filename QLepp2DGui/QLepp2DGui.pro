QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlepp2d
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

TRANSLATIONS += qlepp2d_es.ts \

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QLepp2DLib/release/ -lqlepp2d-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QLepp2DLib/debug/ -lqlepp2d-lib
else:unix: LIBS += -L$$OUT_PWD/../QLepp2DLib/ -lqlepp2d-lib

INCLUDEPATH += $$PWD/../QLepp2DLib
DEPENDPATH += $$PWD/../QLepp2DLib

# Useful when not installing the app, but testing it in a local environment
QMAKE_LFLAGS += -Wl,--rpath=$$OUT_PWD/../QLepp2DLib/

RESOURCES += \
    res/resources.qrc

# Translations compiling
QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input = TRANSLATIONS
lrelease.output = ${QMAKE_FILE_BASE}.qm
lrelease.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm $$PWD/res/i18n/${QMAKE_FILE_BASE}.qm
lrelease.CONFIG += no_link target_predeps

PRE_TARGETDEPS += compiler_lrelease_make_all

unix {
    isEmpty(PREFIX)
    {
        PREFIX = /usr/bin
    }
    target.path = $$PREFIX
    INSTALLS += target
}
