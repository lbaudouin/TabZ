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
    
private:
    Ui::MainWindow *ui;
    XTA *xta;

private slots:
    void pressNew();
    void pressOpen();
    void pressSave();
    void pressClose();
};

#endif // MAINWINDOW_H
