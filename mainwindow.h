#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTabWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QUrl>

#include "tab.h"
#include "xta.h"
#include "options.h"
#include "optionsform.h"
#include "chords.h"
#include "chordsmanager.h"
#include "downloadxta.h"
#include "mytabwidget.h"
#include "tools.h"
#include "recent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVersion(QString _version) { version = _version; }

protected:
    void setUpToolBar();
    void setUpMenu();
    void moveToolBar(int toolBarPosition);
    int addTab(XTAinfo info);
    Tab* getCurrentTab();

    void loadFiles(QStringList files);

    void updateRecent();
    void addRecent(XTAinfo &info);

private:
    Ui::MainWindow *ui;
    Qt::WindowStates previousState;
    XTA *xta;
    Chords *chords;
    Recent *recent;

    Options *options;

    QList<QAction*> listTabAction;

    QMap<QAction*,Tab*> mapTabAction;

    QList<RecentFile> recentList;
    QList<QObject*> recentAction;

    QTime startTime;

    QAction *exitFullScreenAction;

    QString version;

signals:
    void setColorsEnabled(bool);
    void setColors(QList<ColorRegExp>);
    void optionsChanged(OptionsValues);

    //For single instance
    void needToShow();

private slots:
    void openFile();
    void openFile(QString);
    void clearRecent();

public slots:
    void pressNew(QString text = QString());
    void pressOpen();
    void pressOpenFolder();
    void pressOpenPrevious();
    void pressSave();
    void pressSaveAs();
    void pressClose();
    void pressCloseAll();
    void pressSetFullScreen(bool);
    void pressExitFullScreen();

    void pressPrevious();
    void pressNext();
    void pressGoTo();

    void pressEditMode();
    void pressReadOnlyMode();

    void pressSelectAll();
    void pressUndo();
    void pressRedo();
    void pressCut();
    void pressCopy();
    void pressPaste();

    void pressPreview();
    void pressPrint();
    void pressExportPDF();

    void pressPreference();
    void pressChordsManager();

    void pressOpenMp3();
    void pressOpenGP();

    void pressSearchLyrics();
    void pressSearchXTA();
    void pressDownloadXTA();

    void displaySaveIcon(int,bool);

    void tabMoved(int,int);
    void tabCloseRequest(int);

    void currentTabChanged(int);
    void setUndoAvailable(bool);
    void setRedoAvailable(bool);

    void restart(QString);

    void pressAbout();

    void changeTabName(QString name);

    //For single instance
    void handleMessage(const QString& message);

};

#endif // MAINWINDOW_H
