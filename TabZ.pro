#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T19:21:53
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
    qprintpreview.cpp \
    downloadxta.cpp \
    chordsmanager.cpp \
    readwritexml.cpp \
    recent.cpp \
    lilypond.cpp \
    mytextedit.cpp \
    mycompleter.cpp \
    findreplacedialog.cpp \
    qwidgets/mycheckabletree.cpp
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
    qprintpreview.h \
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
    qprogressbardialog.h \
    qwidgets/mycheckabletree.h
    #myprintpreviewwidget.h #\
    #myprinterprivate.h

unix {
    TARGET = tabz
    HEADERS +=    epubgenerator.h
    SOURCES +=  epubgenerator.cpp
}

win32 {
    TARGET = TabZ
    SOURCES += updatemanager.cpp
    HEADERS += updatemanager.h \
    qprogressbardialog.h

    RC_FILE = TabZ.rc
}

FORMS    += mainwindow.ui

OTHER_FILES += \
    TODO

RESOURCES += ressource.qrc


include(qtsingleapplication/qtsingleapplication.pri)
