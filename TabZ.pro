#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T19:21:53
#
#-------------------------------------------------

QT       += core gui xml network

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
    qwidgets/mycheckabletree.h
    #myprintpreviewwidget.h #\
    #myprinterprivate.h

unix {
    TARGET = tabz

    HEADERS +=    epubgenerator.h \
    quazip/ioapi.h \
    quazip/quaziodevice.h \
    quazip/quazip.h \
    quazip/quazip_global.h \
    quazip/quazipdir.h \
    quazip/quazipfile.h \
    quazip/quazipfileinfo.h \
    quazip/quazipnewinfo.h \
    quazip/zip.h \
    quazip/unzip.h \
    quazip/zlib.h \
    quazip/zconf.h \
    quazip/crypt.h

    SOURCES +=  epubgenerator.cpp \
    quazip/qioapi.cpp \
    quazip/quaziodevice.cpp \
    quazip/quazip.cpp \
    quazip/quazipdir.cpp \
    quazip/quazipfile.cpp \
    quazip/quazipfileinfo.cpp \
    quazip/quazipnewinfo.cpp \
    quazip/zip.c\
    quazip/unzip.c

}

win32 {
    TARGET = TabZ
    SOURCES += updatemanager.cpp
    HEADERS += updatemanager.h

    RC_FILE = TabZ.rc
}

FORMS    += mainwindow.ui

OTHER_FILES += TODO.diff

RESOURCES += ressource.qrc


include(qtsingleapplication/qtsingleapplication.pri)
