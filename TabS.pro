#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T19:21:53
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = TabS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tab.cpp \
    highlighter.cpp \
    guitar.cpp \
    xta.cpp \
    options.cpp \
    optionsform.cpp \
    chords.cpp \
    httpupdate.cpp

HEADERS  += mainwindow.h \
    tab.h \
    highlighter.h \
    guitar.h \
    xta.h \
    xta_info.h \
    options.h \
    optionsform.h \
    chords.h \
    httpupdate.h \
    QProgressBarDialog.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    TODO.txt

RESOURCES += \
    ressource.qrc
