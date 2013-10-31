#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QFileInfo>

#include <QProcess>

#include <QMessageBox>

#include "qprogressbardialog.h"

class UpdateManager : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateManager(QWidget *parent = 0);
    void setVersion(QString version);

    inline void setVersionUrl(QString url) {versionUrl = url.trimmed();}
    inline void setExecUrl(QString url) {execUrl = url.trimmed();}
    //inline void setZipUrl(QString url) {zipUrl = url.trimmed();}
    //inline void setLzmaUrl(QString url) {lzmaUrl = url.trimmed();}


    inline void setExecFilename(QString exec) {execFilename = exec;}

    bool replaceByUpdate();
    bool replaceMainExec();

    static int getVersionID(QString version);

protected:
    bool isValidVersion(QString);

private:
    QNetworkAccessManager *manager;
    QProgressBarDialog *progress;

    QString currentVersion;

    QString versionUrl;
    QString execUrl;
    //QString zipUrl;
    //QString lzmaUrl;

    QString execFilename;
    QString updateFilename;

    bool discretUpdate;

signals:
    void status(QString);
    void restart(QString);

public slots:
    void startUpdate(QString url = QString());

    void checkVersion();

    void getExec(QString url = QString());
    void saveExec();

    void downloadProgress(qint64,qint64);
    void abort();

    void setDiscret(bool);
};

#endif // UPDATEMANAGER_H
