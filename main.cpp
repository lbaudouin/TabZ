#define CURRENT_VERSION "0.1.3"
#define USE_UNZIP 0

#include <QApplication>
#include "mainwindow.h"

#ifdef Q_OS_WIN
#include "updatemanager.h"
#endif

#include "qtsingleapplication/qtsingleapplication.h"

/** Main function
 * Test if one instance is already running
 * Translate in french if needed
 * Download update for WINDOWS
**/
int main(int argc, char *argv[])
{
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

    MainWindow w;
    w.handleMessage(message);
    w.setVersion(CURRENT_VERSION);
    w.show();

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)), &w, SLOT(handleMessage(const QString&)));

    instance.setActivationWindow(&w, false);

    QObject::connect(&w, SIGNAL(needToShow()), &instance, SLOT(activateWindow()));

#ifdef Q_OS_WIN
    UpdateManager *up = new UpdateManager;
    QObject::connect(up,SIGNAL(restart(QString)),&w,SLOT(restart(QString)));
    up->setVersion(CURRENT_VERSION);
    up->setMessageUrl("http://lbaudouin.chez.com/TABZ_MESSAGE");
    up->setVersionUrl("http://lbaudouin.chez.com/TABZ_VERSION");
    up->setExecUrl("http://lbaudouin.chez.com/TabZ-update.exe");
    //up->setZipUrl("http://lbaudouin.chez.com/TabZ-update.zip");
    up->getMessage();
    up->setDiscret(true);
    up->startUpdate();
#endif
    
    return instance.exec();
}
