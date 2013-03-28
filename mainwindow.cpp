#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpToolBar();


    xta = new XTA(this->centralWidget());


    QString testFile = "test.xta";

    int index = addTab(testFile);

    ui->tabWidget->setCurrentIndex( index );

    XTAinfo info = xta->parse(testFile);

    ((Tab*)(ui->tabWidget->currentWidget()))->setXTA(info);

    this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpToolBar()
{
    ui->actionNew->setIcon( this->style()->standardIcon(QStyle::SP_FileIcon) );
    ui->actionSave->setIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton) );
    ui->actionSave_as->setIcon( this->style()->standardIcon(QStyle::SP_DriveHDIcon) );
    ui->actionOpen->setIcon( this->style()->standardIcon(QStyle::SP_DirOpenIcon) );
    ui->actionClose->setIcon( this->style()->standardIcon(QStyle::SP_DialogCloseButton) );
    ui->actionQuit->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarCloseButton) );

    ui->actionPrevious->setIcon( this->style()->standardIcon(QStyle::SP_ArrowLeft) );
    ui->actionNext->setIcon( this->style()->standardIcon(QStyle::SP_ArrowRight) );

    ui->actionFull_Screen->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarMaxButton) );

    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionSave_as);
    ui->mainToolBar->addAction(ui->actionClose);
    ui->mainToolBar->addAction(ui->actionFull_Screen);

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(pressNew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(pressOpen()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(pressSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(pressSaveAs()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(pressClose()));

    connect(ui->actionPrevious,SIGNAL(triggered()),this,SLOT(pressPrevious()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(pressNext()));

    connect(ui->actionFull_Screen,SIGNAL(toggled(bool)),this,SLOT(pressSetFullScreen(bool)));
}

int MainWindow::addTab(QString name)
{
    QAction *action = new QAction(name,this);
    Tab *tab = new Tab(ui->tabWidget);
    mapTabAction.insert(action,tab);

    ui->menuTabs->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(pressGoTo()));

    return ui->tabWidget->addTab(tab,name);
}

void MainWindow::pressNew()
{
    int index = addTab("*.new");
    ui->tabWidget->setCurrentIndex( index );
}

void MainWindow::pressOpen()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open file"),"."/*QDir::homePath()*/,"*.xta");

    if(filepath.isEmpty())
        return;

    QFileInfo fi(filepath);

    int index = addTab(fi.fileName());

    ui->tabWidget->setCurrentIndex( index );

    XTAinfo info = xta->parse(filepath);

    ((Tab*)(ui->tabWidget->widget(index)))->setXTA(info);
}

void MainWindow::pressSave()
{
    XTAinfo info = ((Tab*)(ui->tabWidget->currentWidget()))->getXTA();
    if(info.filename.isEmpty()){
        pressSaveAs(info);
    }else{
        xta->save(info.filename,info);
    }
}

void MainWindow::pressSaveAs()
{
    XTAinfo info = ((Tab*)(ui->tabWidget->currentWidget()))->getXTA();
    pressSaveAs(info);
}

void MainWindow::pressSaveAs(XTAinfo info)
{
    QString filepath = QFileDialog::getSaveFileName(this,tr("Save as"),".",tr("XTA files (*.xta)"));
    if(!filepath.isEmpty()){
        QFileInfo fi(filepath);
        if(fi.suffix()!=".xta"){
            filepath.push_back(".xta");
        }
        xta->save(filepath,info);
    }
}

void MainWindow::pressClose()
{
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    QAction* action = mapTabAction.key(currentTab);

    mapTabAction.remove(action);

    delete action;
    delete ui->tabWidget->currentWidget();
}

void MainWindow::pressSetFullScreen(bool fullScreen)
{
    if(fullScreen){
        previousState = this->windowState();
        this->setWindowState(Qt::WindowFullScreen);
    }else
        this->setWindowState(previousState);
}

void MainWindow::pressPrevious()
{
    int currentIndex = ui->tabWidget->currentIndex();
    int maxIndex = ui->tabWidget->count();
    if(maxIndex==0) return;
    if(currentIndex==0){
        ui->tabWidget->setCurrentIndex(maxIndex-1);
    }else{
        ui->tabWidget->setCurrentIndex(currentIndex-1);
    }
}

void MainWindow::pressNext()
{
    int currentIndex = ui->tabWidget->currentIndex();
    int maxIndex = ui->tabWidget->count();
    if(maxIndex==0) return;
    ui->tabWidget->setCurrentIndex((currentIndex+1)%maxIndex);
}

void MainWindow::pressGoTo()
{
    QAction *action = (QAction*)sender();
    ui->tabWidget->setCurrentWidget(mapTabAction[action]);
}
