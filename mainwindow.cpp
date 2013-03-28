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
    connect(ui->actionOpen_Folder,SIGNAL(triggered()),this,SLOT(pressOpenFolder()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(pressSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(pressSaveAs()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(pressClose()));
    connect(ui->actionClose_All,SIGNAL(triggered()),this,SLOT(pressCloseAll()));

    connect(ui->actionPrevious,SIGNAL(triggered()),this,SLOT(pressPrevious()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(pressNext()));

    connect(ui->actionFull_Screen,SIGNAL(toggled(bool)),this,SLOT(pressSetFullScreen(bool)));

    connect(ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(pressSelectAll()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(pressCut()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(pressCopy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(pressPaste()));

    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(pressUndo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(pressRedo()));
}

int MainWindow::addTab(XTAinfo info)
{
    QString tabName = info.filename;
    if(!info.artist.isEmpty() && !info.title.isEmpty())
        tabName = info.artist + " - " + info.title;
    if(tabName.isEmpty())
        tabName = tr("New");

    int index = addTab(tabName);

    ((Tab*)(ui->tabWidget->widget(index)))->setXTA(info);
}

int MainWindow::addTab(QString name)
{
    QAction *action = new QAction(name,this);
    action->setIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton ) );

    Tab *tab = new Tab(ui->tabWidget);

    mapTabAction.insert(action,tab);

    ui->menuTabs->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(pressGoTo()));

    int index = ui->tabWidget->addTab(tab,name);

    ui->tabWidget->setCurrentIndex( index );

    return index;
}

void MainWindow::pressNew(QString text)
{
    XTAinfo info("","");
    info.text = text;
    int index = addTab(info);

    ui->tabWidget->setTabIcon(index, QIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton )) );
}

void MainWindow::pressOpen()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open file"),"."/*QDir::homePath()*/,"*.xta *.txt");

    if(filepath.isEmpty())
        return;

    QFileInfo fi(filepath);

    XTAinfo info(fi.filePath(), fi.fileName());
    info = xta->parse(filepath);

    addTab( info );
}

void MainWindow::pressOpenFolder()
{
    QString folderpath = QFileDialog::getExistingDirectory(this,tr("Open file"),".");

    if(folderpath.isEmpty())
        return;

    QStringList filters;
    filters << "*.xta" << "*.XTA" << "*.txt" << "*.TXT";

    QDir dir(folderpath);
    dir.setNameFilters(filters);
    QStringList listFiles = dir.entryList();

    foreach(QString filepath, listFiles){
        QFileInfo fi(filepath);
        XTAinfo info(fi.filePath(), fi.fileName());
        info = xta->parse(filepath);
        addTab(info);
    }
}

void MainWindow::pressSave()
{
    if(ui->tabWidget->currentIndex()<0) return;
    XTAinfo info = ((Tab*)(ui->tabWidget->currentWidget()))->getXTA();
    if(info.filename.isEmpty()){
        pressSaveAs(info);
    }else{
        xta->save(info.filename,info);
    }
}

void MainWindow::pressSaveAs()
{
    if(ui->tabWidget->currentIndex()<0) return;
    XTAinfo info = ((Tab*)(ui->tabWidget->currentWidget()))->getXTA();
    pressSaveAs(info);
}

void MainWindow::pressSaveAs(XTAinfo info)
{
    if(ui->tabWidget->currentIndex()<0) return;
    QString selectedFilter;

    QString sample = ".";
    if(!info.artist.isEmpty() && !info.title.isEmpty())
        sample = info.artist + " - " + info.title;


    QString filepath = QFileDialog::getSaveFileName(this,tr("Save as"),sample,QString("%1;;%2").arg(tr("XTA files (*.xta)"),tr("TXT files (*.txt)")),&selectedFilter);
    qDebug() << selectedFilter;

    if(!filepath.isEmpty()){
        QFileInfo fi(filepath);
        if(selectedFilter==tr("XTA files (*.xta)") && fi.suffix()!="xta"){
            filepath.push_back(".xta");
        }
        if(selectedFilter==tr("TXT files (*.txt)") && fi.suffix()!="txt"){
            filepath.push_back(".txt");
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

void MainWindow::pressCloseAll()
{
    while(ui->tabWidget->count()!=0){
        Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
        QAction* action = mapTabAction.key(currentTab);

        mapTabAction.remove(action);

        delete action;
        delete ui->tabWidget->currentWidget();
    }
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

void MainWindow::pressSelectAll()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->selectAll();
}

void MainWindow::pressUndo()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->undo();
}

void MainWindow::pressRedo()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->redo();
}

void MainWindow::pressCut()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->cut();
}

void MainWindow::pressCopy()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->copy();
}

void MainWindow::pressPaste()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = (Tab*)(ui->tabWidget->currentWidget());
    currentTab->paste();
}
