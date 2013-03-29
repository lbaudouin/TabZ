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

    void diff(XTAinfo &info){
        if(info.capo!=capo) qDebug() << "Capo";
        if(info.tuning!=tuning) qDebug() << "Tuning";
        if(info.title!=title) qDebug() << "Title";
        if(info.artist!=artist) qDebug() << "Artist";
        if(info.album!=album) qDebug() << "Album";
        if(info.file_gp!=file_gp) qDebug() << "FileGP";
        if(info.file_mp3!=file_mp3) qDebug() << "FileMP3";
        if(info.text!=text) qDebug() << "Text";
        if(info.chords!=chords) qDebug() << "Chords";
    }

    bool isEqual(XTAinfo &info){
        if(info.capo!=capo) return false;
        if(info.tuning!=tuning) return false;
        if(info.title!=title) return false;
        if(info.artist!=artist) return false;
        if(info.album!=album) return false;
        if(info.file_gp!=file_gp) return false;
        if(info.file_mp3!=file_mp3) return false;
        if(info.text!=text) return false;
        if(info.chords!=chords) return false;
        return true;
    }
};

#endif // XTA_INFO_H
