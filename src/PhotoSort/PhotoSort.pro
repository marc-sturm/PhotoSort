#-------------------------------------------------
#
# Project created by QtCreator 2013-08-13T08:31:34
#
#-------------------------------------------------

QT       +=  core widgets gui printsupport

TARGET = PhotoSort
TEMPLATE = app
RC_FILE	 = icon.rc

SOURCES += main.cpp\
        MainWindow.cpp \
    ImageWorker.cpp \
    ResizeDialog.cpp \
    Image.cpp \
    DuplicateDialog.cpp

HEADERS  += MainWindow.h \
    ImageWorker.h \
    ResizeDialog.h \
    Image.h \
    DuplicateDialog.h

FORMS    += MainWindow.ui \
    ResizeDialog.ui \
    DuplicateDialog.ui

#include cppCORE library
INCLUDEPATH += $$PWD/../../src/cppCORE
LIBS += -L$$PWD/../../bin -lcppCORE

#include cppGUI library
INCLUDEPATH += $$PWD/../../src/cppGUI
LIBS += -L$$PWD/../../bin -lcppGUI

#copy EXE to bin folder
DESTDIR = $$PWD/../../bin

RESOURCES += \
    resources.qrc
