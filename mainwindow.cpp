#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QHBoxLayout *layout = new QHBoxLayout(ui->tab1);
    //layout->addWidget(tab);
    //ui->tab1->layout()->addWidget(tab);

    delete ui->tabNew;

    //QTabWidget *qtab = new QTabWidget;

    setUpToolBar();


    xta = new XTA(this->centralWidget());

    ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab(new Tab(ui->tabWidget),"maman papa.xta") );

    XTAinfo info = xta->parse("maman papa.xta");

    ((Tab*)(ui->tabWidget->currentWidget()))->setXTA(info);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpToolBar()
{
    ui->actionNew->setIcon( this->style()->standardIcon(QStyle::SP_FileIcon) );
    ui->actionSave->setIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton) );
    ui->actionOpen->setIcon( this->style()->standardIcon(QStyle::SP_DirOpenIcon) );
    ui->actionClose->setIcon( this->style()->standardIcon(QStyle::SP_DialogCloseButton) );
    ui->actionQuit->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarCloseButton) );

    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionClose);

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(pressNew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(pressOpen()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(pressSave()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(pressClose()));
}

void MainWindow::pressNew()
{
    ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab(new Tab(ui->tabWidget),"*new") );
}

void MainWindow::pressOpen()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open file"),"."/*QDir::homePath()*/,"*.xta");

    if(filepath.isEmpty())
        return;

    QFileInfo fi(filepath);

    ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab(new Tab(ui->tabWidget),fi.fileName()) );

    XTAinfo info = xta->parse(filepath);

    ((Tab*)(ui->tabWidget->currentWidget()))->setXTA(info);
}

void MainWindow::pressSave()
{
    XTAinfo info = ((Tab*)(ui->tabWidget->currentWidget()))->getXTA();
    xta->save("test.xta",info);
}

void MainWindow::pressClose()
{
    delete ui->tabWidget->currentWidget();
}
