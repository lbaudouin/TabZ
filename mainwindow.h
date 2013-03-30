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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void setUpToolBar();
    int addTab(XTAinfo info);
    Tab* getCurrentTab();
    
private:
    Ui::MainWindow *ui;
    Qt::WindowStates previousState;
    XTA *xta;

    QMap<QAction*,Tab*> mapTabAction;

private slots:
    void pressNew(QString text = QString());
    void pressOpen();
    void pressOpenFolder();
    void pressSave();
    void pressSaveAs();
    void pressClose();
    void pressCloseAll();
    void pressSetFullScreen(bool);

    void pressPrevious();
    void pressNext();
    void pressGoTo();

    void pressSelectAll();
    void pressUndo();
    void pressRedo();
    void pressCut();
    void pressCopy();
    void pressPaste();

    void pressSearchLyrics();
    void pressSearchXTA();

    void displaySaveIcon(int,bool);

    void tabCloseRequest(int);
};

#endif // MAINWINDOW_H
