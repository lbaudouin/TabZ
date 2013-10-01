#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T19:21:53
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = TabZ
TEMPLATE = app

TRANSLATIONS = TabZ_fr.ts

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
    lilypond.cpp \
    mytextedit.cpp \
    mycompleter.cpp \
    findreplacedialog.cpp \
    epubgenerator.cpp
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
    qprogressbardialog.h \
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
    openeditlayout.h \
    mytextedit.h \
    fontlabellayout.h \
    mycompleter.h \
    findreplacedialog.h \
    epubgenerator.h
    #myprintpreviewwidget.h #\
    #myprinterprivate.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    TODO.diff \
    win32/TABS_VERSION \
    debian/DEBIAN/control

RESOURCES += ressource.qrc

RC_FILE = TabZ.rc

include(qtsingleapplication/qtsingleapplication.pri)
