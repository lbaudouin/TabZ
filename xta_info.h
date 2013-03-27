#ifndef XTA_INFO_H
#define XTA_INFO_H

#include <QString>

struct XTAinfo{
    XTAinfo() : capo(0) {}

    QString version;
    int capo;
    QString tuning;
    QString title;
    QString artist;
    QString album;
    QString file_gp;
    QString file_mp3;

    QString text;
    QString chords;
};

#endif // XTA_INFO_H
