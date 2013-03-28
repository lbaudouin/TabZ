#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTabWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>

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
    int addTab(QString name);
    int addTab(XTAinfo info);
    
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
    void pressSaveAs(XTAinfo);
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
};

#endif // MAINWINDOW_H
