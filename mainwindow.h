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
    
private:
    Ui::MainWindow *ui;
    Qt::WindowStates previousState;
    XTA *xta;

    QMap<QAction*,Tab*> mapTabAction;

private slots:
    void pressNew();
    void pressOpen();
    void pressSave();
    void pressSaveAs();
    void pressSaveAs(XTAinfo);
    void pressClose();
    void pressSetFullScreen(bool);

    void pressPrevious();
    void pressNext();
    void pressGoTo();
};

#endif // MAINWINDOW_H
