#ifndef HTTPUPDATE_H
#define HTTPUPDATE_H

#include <QHttp>
#include <QFile>
#include <QBuffer>
#include <QMessageBox>
#include <QProcess>

#if USE_UNZIP
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#endif

#include "qprogressbardialog.h"

struct Version {
    Version(int major, int minor, int revision) : major_(major), minor_(minor), revision_(revision), valid(true) {}
    Version(QString version) : major_(0), minor_(0), revision_(0){
        QStringList s = version.split(".");
        if(s.size()!=3){
            valid = false;
        }else{
            bool ok1,ok2,ok3;
            major_ = s.at(0).toInt(&ok1);
            minor_ = s.at(1).toInt(&ok2);
            revision_ = s.at(2).toInt(&ok3);
            valid = ok1 && ok2 && ok3;
        }
    }

    int major_,minor_,revision_;
    bool valid;
};

inline bool operator<(const Version &v1, const Version &v2){
    if(v1.major_ < v2.major_) return true;
    if(v1.minor_ < v2.minor_) return true;
    if(v1.revision_ < v2.revision_) return true;
    return false;
}
inline bool operator>(const Version &v1, const Version &v2){
    if(v1.major_ > v2.major_) return true;
    if(v1.minor_ > v2.minor_) return true;
    if(v1.revision_ > v2.revision_) return true;
    return false;
}
inline bool operator==(const Version &v1, const Version &v2){
    if(v1.major_ != v2.major_) return false;
    if(v1.minor_ != v2.minor_) return false;
    if(v1.revision_ != v2.revision_) return false;
    return true;
}

class HttpUpdate : public QWidget
{
    Q_OBJECT
public:
    HttpUpdate(QString version, QString versionFile, QString softwareFile, QString updateFile, QWidget *parent = 0);
    ~HttpUpdate();
    void getVersion();
    void getSoftware();
    inline void setDiscret(bool discret) {discretUpdate = discret;}
    inline void setVersionFileName(QString name){versionFileName = name;}
    inline void setSoftwareFileName(QString name){softwareFileName = name;}
    inline void setSoftwareTempFileName(QString name){softwareTempFileName = name;}
    inline void setCurrentExecutable(QString name){currentExecutable = name;}

    static bool copyData(QIODevice &inFile, QIODevice &outFile)
    {
        while (!inFile.atEnd()) {
            char buf[4096];
            qint64 readLen = inFile.read(buf, 4096);
            if (readLen <= 0)
                return false;
            if (outFile.write(buf, readLen) != readLen)
                return false;
        }
        return true;
    }

protected:
    bool unzip(QString zipPath, QString outputPath);


private :
    QString currentVersionString;
    bool discretUpdate;
    QHttp *http;
    QFile *download;
    QBuffer *buf;
    int downloadID_version;
    int downloadID_software;

    QString versionFileName;
    QString softwareFileName;
    QString softwareTempFileName;
    QString currentExecutable;

    QProgressBarDialog *progress;

private slots:
    void downloadFinished(int id, bool error);

public slots:
    void startUpdate();
    void dataReadProgress(int,int);
    void abort();

signals:
    void restart(QString filepath);
    void status(QString status);

};

#endif // HTTPUPDATE_H
