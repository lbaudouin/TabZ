#include "httpupdate.h"

/** Create HTTP object to download updates
* @param current version string
* @param name of the file containing the version number
* @param name of the file on internet
* @param name of the file on local
* @warning Download on lbaudouin.chez.com
*/
HttpUpdate::HttpUpdate(QString version, QString versionFile, QString softwareFile, QString updateFile, QWidget* parent) : QWidget(parent), http(0), download(0), buf(0)
{
    currentVersionString = version;
    setVersionFileName(versionFile);
    setSoftwareFileName(softwareFile);
    setSoftwareTempFileName(updateFile);

    progress = new QProgressBarDialog;
    progress->setFormat("%v"+tr("Kb")+"/%m"+tr("Kb"));
    progress->setMinimum(0);

    buf = new QBuffer;
    http = new QHttp;
    http->setHost("lbaudouin.chez.com");
    connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(downloadFinished(int,bool)));
    connect(http,SIGNAL(dataReadProgress(int,int)),this,SLOT(dataReadProgress(int,int)));
    connect(progress,SIGNAL(cancel()),this,SLOT(abort()));
}

HttpUpdate::~HttpUpdate()
{
    if(buf) { buf->close(); delete buf; }
    if(http) { http->close(); delete http;}
    if(download) { download->close(); delete download;}
}

void HttpUpdate::startUpdate()
{
    getVersion();
}

void HttpUpdate::getVersion()
{
     downloadID_version = http->get(versionFileName,buf);
}

void HttpUpdate::downloadFinished(int id, bool error)
{
    if(!http->currentRequest().isValid())
        return;

    if(error){
        QString errorText;

        switch(http->error()){
        case QHttp::HostNotFound : errorText = tr("The host name lookup failed"); break;
        case QHttp::ConnectionRefused : errorText = tr("The server refused the connection"); break;
        case QHttp::UnexpectedClose : errorText = tr("The server closed the connection unexpectedly"); break;
        case QHttp::InvalidResponseHeader : errorText = tr("The server sent an invalid response header"); break;
        case QHttp::WrongContentLength : errorText = tr("The client could not read the content correctly because an error with respect to the content length occurred"); break;
        case QHttp::Aborted : errorText = tr("The request was aborted with abort()"); break;
        case QHttp::ProxyAuthenticationRequiredError : errorText = tr("QHttp is using a proxy, and the proxy server requires authentication to establish a connection"); break;
        case QHttp::AuthenticationRequiredError : errorText = tr("The web server requires authentication to complete the request"); break;
        case QHttp::UnknownError : errorText = tr("An unknown error occurred"); break;
        case QHttp::NoError : errorText =  ""; break;
        }

        if(!errorText.isEmpty()){
            if(!discretUpdate){
                QMessageBox::critical(this,tr("Error"),errorText);
            }
            emit this->status(errorText);
        }

        progress->hide();
        return;
    }

    if(id==downloadID_version){
        QString lastestVersionString(buf->buffer());
        buf->close();
        lastestVersionString.remove("\n");

        Version lastestVersion(lastestVersionString);
        Version currentVersion(currentVersionString);

        if(!lastestVersion.valid){
            emit this->status(tr("Error in version file"));
            if(!discretUpdate) QMessageBox::critical(this,tr("Error"),tr("Could not get the last version"));
            return;
        }

        if( lastestVersion > currentVersion ){
                int button = QMessageBox::information(this,tr("Information"),tr("New version available : %1").arg(lastestVersionString),tr("Don't download"),tr("Download"),"",1,0);
                if(button==1)
                    getSoftware();
        }else{
            emit this->status(tr("No new version available"));
            if(!discretUpdate) QMessageBox::information(this,tr("Information"),tr("No new version available"));
        }
        return;
    }

    if(id==downloadID_software){
        download->close();
        int size = download->size()/1000.0;

        progress->close();

        //TODO : Force restart (create an options ?)
        bool forceRestart = true;
        if(forceRestart){
            emit this->restart(currentExecutable);
            return;
        }

        //Restart software
        QMessageBox mess;
        mess.setText(tr("%1Kb downloaded\nPlease restart software to use the new version").arg(size));
        mess.addButton(tr("Restart"),QMessageBox::AcceptRole);
        mess.addButton(tr("Do not restart"),QMessageBox::RejectRole);
        mess.setIcon(QMessageBox::Question);
        mess.setDetailedText("Software:\n"+softwareFileName+"\n\n"
                             "Update:\n"+softwareTempFileName);

        int button = mess.exec();
        if(button==QMessageBox::AcceptRole){
            emit this->restart(currentExecutable);
        }
        return;
    }
}

void HttpUpdate::getSoftware()
{
    download = new QFile(softwareTempFileName);
    download->open(QIODevice::WriteOnly);
    downloadID_software = http->get(softwareFileName,download);
    progress->show();
}

void HttpUpdate::dataReadProgress(int done, int total)
{
    progress->setMaximum(total/1000);
    progress->setValue(done/1000);
}

void HttpUpdate::abort()
{
    http->abort();
    progress->close();
    this->close();
}
