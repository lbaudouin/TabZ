#define CURRENT_VERSION "0.1.2"
#define USE_UNZIP 0

#include <QtGui/QApplication>
#include "mainwindow.h"

#if defined(__WIN32__)
#include "updatemanager.h"
#endif

#include "qtsingleapplication/qtsingleapplication.h"

/** Main function
 * Read arguments and return id/version if needed
 * Test if one instance is already running
 * Translate in french if needed
 * Download update for WIN32
**/
int main(int argc, char *argv[])
{

#if defined(__WIN32__)
    for(int i=0;i<argc;i++){
        if(!strcmp(argv[i],"-v")){              //return version ID integer
            int ID = UpdateManager::getVersionID(CURRENT_VERSION);
            qDebug() << "VersionID:" << ID;
            return ID;
        }
        if(!strcmp(argv[i],"-n")){              //return 1 if CURRENT_VERSION > version
            if(QString(CURRENT_VERSION)>QString(argv[i+1])) return 1;
            return 0;
        }
    }
#endif

    QtSingleApplication instance("TabZ", argc, argv);
    instance.setWindowIcon( QIcon(":/images/TabZ.png" ) );

    QString message;
    for(int a = 1; a < argc; ++a) {
        message += QString::fromLocal8Bit(argv[a]);
        if (a < argc-1)
            message += "\n";
    }

    if(instance.sendMessage(message))
        return 0;


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

#if defined(__WIN32__)
    UpdateManager *up = new UpdateManager;
    up->setVersion(CURRENT_VERSION);
    up->setExecFilename(argv[0]);

    if(up->replaceMainExec())
        return 0;

    if(up->replaceByUpdate())
        return 0;
#endif

    MainWindow w;
    w.handleMessage(message);
    w.setVersion(CURRENT_VERSION);
    w.show();

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)), &w, SLOT(handleMessage(const QString&)));

    instance.setActivationWindow(&w, false);

    QObject::connect(&w, SIGNAL(needToShow()), &instance, SLOT(activateWindow()));



#if defined(__WIN32__)
    up->setVersionUrl("http://lbaudouin.chez.com/TABZ_VERSION");
    up->setExecUrl("http://lbaudouin.chez.com/TabZ.exe");
    //up->setZipUrl("http://lbaudouin.chez.com/TabZ.zip");
    up->setDiscret(true);
    up->startUpdate();
    QObject::connect(up,SIGNAL(restart(QString)),&w,SLOT(restart(QString)));
#endif
    
    return instance.exec();
}
