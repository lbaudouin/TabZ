#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T19:21:53
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = TabS
TEMPLATE = app

TRANSLATIONS = TabS_fr.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    tab.cpp \
    highlighter.cpp \
    guitar.cpp \
    xta.cpp \
    options.cpp \
    optionsform.cpp \
    chords.cpp \
    httpupdate.cpp \
    qprintpreview.cpp \
    downloadxta.cpp \
    chordsmanager.cpp \
    readwritexml.cpp \
    recent.cpp \
    lilypond.cpp
    #myprintpreviewwidget.cpp# \
    #myprinterprivate.cpp

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
    qprintpreview.h \
    qprogressbardialog.h \ #\
    downloadxta.h \
    mycheckbox.h \
    mytabwidget.h \
    chordsmanager.h \
    tools.h \
    colorregexpform.h \
    readwritexml.h \
    colorregexp.h \
    recent.h \
    lilypond.h \
    openeditlayout.h
    #myprintpreviewwidget.h #\
    #myprinterprivate.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    TODO.diff \
    win32/TABS_VERSION \
    debian/DEBIAN/control

RESOURCES += \
    ressource.qrc

RC_FILE = TabS.rc

include(qtsingleapplication/qtsingleapplication.pri)
