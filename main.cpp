#include <QtGui/QApplication>
#include "mainwindow.h"
#define CURRENT_VERSION "0.0.13"

#include "httpupdate.h"
#include "qtsingleapplication/qtsingleapplication.h"


int main(int argc, char *argv[])
{
    QString path(argv[0]);
    qDebug() << path;

    for(int i=0;i<argc;i++){
        if(!strcmp(argv[i],"-v")){              //return version ID integer
            QString version(CURRENT_VERSION);
            QStringList n = version.split(".");
            int ID = n.at(0).toInt()*100*100 + n.at(1).toInt()*100 + n.at(2).toInt();
            qDebug() << QString("Version : ") + version + QString(" - ID : ") + QString::number(ID);
            return ID;
        }
        if(!strcmp(argv[i],"-n")){              //return 1 if CURRENT_VERSION > version
            Version currentVersion(CURRENT_VERSION);
            Version version(argv[i+1]);
            if(currentVersion>version) return 1;
            return 0;
        }
    }

    QtSingleApplication instance("TabS", argc, argv);
    instance.setWindowIcon( QIcon(":images/TabS" ) );
    QString message;
    for(int a = 1; a < argc; ++a) {
        message += QString::fromUtf8(argv[a]);
        if (a < argc-1)
            message += " ";
    }

    if(instance.sendMessage(message))
        return 0;

#if defined(__WIN32__)
    if(path.contains("-update.exe")){
        QString original_path = path;
        original_path = original_path.remove("-update");

        if(QFile::exists(original_path)){
            while(!QFile::remove(original_path)){
               QMessageBox mess;
               mess.setText(QString("Can't remove: %1\nPlease close the software").arg(original_path));
               mess.setWindowTitle("Error");
               mess.setIcon(QMessageBox::Critical);
               mess.exec();
            }
        }

        QFile::copy(path,original_path);
        QProcess process;

        process.startDetached("\""+original_path+"\"");
        exit(1);
    }else{
        QString update = path;
        update.remove(".exe");
        update += "-update.exe";
        qDebug() << update;

        if(QFile::exists(update)){
            QProcess process;
            QStringList arg;
            arg << "-v";
            int updateID = process.execute("\""+update+"\"",arg);
            QString currentVersion(CURRENT_VERSION);
            QStringList n = currentVersion.split(".");
            int currentID = n.at(0).toInt()*100*100 + n.at(1).toInt()*100 + n.at(2).toInt();
            if(currentID>=updateID){
                qDebug() << "Already up to date";
                QFile::remove(update);
            }else{
                process.startDetached("\""+update+"\"");
                exit(1);
            }
        }
    }
#endif

    QString lang = QLocale::system().name().section('_', 0, 0);
    lang = lang.toLower();

    if(lang=="fr"){
        QTranslator *translator = new QTranslator();
        translator->load(QString(":/lang/lang_") + lang);
        qApp->installTranslator( translator );

        QTranslator *translatorQt = new QTranslator();
        translatorQt->load(QString("qt_") + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        qApp->installTranslator( translatorQt );
    }

    MainWindow w;
    w.handleMessage(message);
    w.show();

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)), &w, SLOT(handleMessage(const QString&)));

    instance.setActivationWindow(&w, false);

    QObject::connect(&w, SIGNAL(needToShow()), &instance, SLOT(activateWindow()));

#if defined(__WIN32__)
    QString updateFilename = path;
    updateFilename.remove(".exe");
    updateFilename.push_back("-update.exe");
    HttpUpdate *http = new HttpUpdate(CURRENT_VERSION,"TABS_VERSION","TabS.exe",updateFilename,w.centralWidget());
    http->setCurrentExecutable(path);
    http->setDiscret(true);
    http->startUpdate();
    QObject::connect(http,SIGNAL(restart(QString)),&w,SLOT(restart(QString)));
#endif

    
    return instance.exec();
}
