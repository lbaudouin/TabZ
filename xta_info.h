#ifndef XTA_INFO_H
#define XTA_INFO_H

#include <QString>
#include <QImage>

#include <QTextStream>

class XTAinfo{
public:
    XTAinfo() : capo(0), tuning("EADGBE"), instrument("guitar") {}
    XTAinfo(QString path, QString name) : filepath(path), filename(name), capo(0), tuning("EADGBE"), instrument("guitar") {}

    QString filepath;
    QString filename;

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

    QList<QImage> images;
    QList<int> refImages;

    QString instrument;

    void diff(XTAinfo &info, QTextStream &stream){
        if(info.capo!=capo) stream << "Capo";
        if(info.tuning!=tuning) stream << "Tuning";
        if(info.title!=title) stream << "Title";
        if(info.artist!=artist) stream << "Artist";
        if(info.album!=album) stream << "Album";
        if(info.file_gp!=file_gp) stream << "FileGP";
        if(info.file_mp3!=file_mp3) stream << "FileMP3";
        if(info.text!=text) stream << "Text";
        if(info.chords!=chords) stream << "Chords";
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

    QString createFileName(QString defaultFileName = "output"){
        QString output;
        if(!artist.isEmpty() && !title.isEmpty()){
            output = artist + " - " + title;
        }else if(artist.isEmpty() && !title.isEmpty()){
            output = title;
        }else if(!artist.isEmpty() && title.isEmpty()){
            output = artist;
        }else{
            output = defaultFileName;
        }
        output.replace("/","_");
        output.replace("\\","_");
        return output;
    }
};

#endif // XTA_INFO_H
