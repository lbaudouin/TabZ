#ifndef XTA_INFO_H
#define XTA_INFO_H

#include <QString>

struct XTAinfo{
    XTAinfo(QString path, QString name) : filepath(path), filename(name), capo(0), tuning("EADGBE") {}

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

    QString filename;
    QString filepath;
};

#endif // XTA_INFO_H
