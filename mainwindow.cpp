#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpToolBar();

    options.parse(this);


    xta = new XTA(this->centralWidget());


    QString testFile = "test.xta";

    XTAinfo info = xta->parse(testFile);
    int index = addTab(info);

    ui->tabWidget->setCurrentIndex( index );

    //this->setWindowState(Qt::WindowMaximized);

    /*QPrinter *printer = new QPrinter;

    QPrintPreviewWidget *pre = new QPrintPreviewWidget(printer);

    ui->tabWidget->addTab(pre,"preview");


    QTextEdit *edit = new QTextEdit;
    edit->setText(info.text);

    QPrintEngine *pe = new QPrintEngine;
    QPaintEngine *ppe = new QPaintEngine;

    printer->setEngines(pe,ppe);*/


    //QPrintDialog *pd = new QPrintDialog(printer,this);
    //pd->show();

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseRequest(int)));
    connect(ui->actionSearch_lyrics,SIGNAL(triggered()),this,SLOT(pressSearchLyrics()));
    connect(ui->actionSearch_XTA,SIGNAL(triggered()),this,SLOT(pressSearchXTA()));
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(pressPreference()));
}

MainWindow::~MainWindow()
{
    pressCloseAll();
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

void MainWindow::tabCloseRequest(int index)
{
    Tab* tab = (Tab*)ui->tabWidget->widget(index);

    if(tab->isModified()){
        int button = QMessageBox::warning(this,"modified",QString("Do you want to save : %1").arg(ui->tabWidget->tabText(index)),QMessageBox::Yes,QMessageBox::No);
        //TODO
        /*if(button==QMessageBox::Yes){
            pressSave();
        }*/
    }

    QAction* action = mapTabAction.key(tab);

    mapTabAction.remove(action);

    delete action;
    delete tab;
}

Tab* MainWindow::getCurrentTab()
{
    if(ui->tabWidget->count()==0) return 0;
    else return (Tab*)ui->tabWidget->currentWidget();
}

int MainWindow::addTab(XTAinfo info)
{
    QString tabName = info.filename;
    if(!info.artist.isEmpty() && !info.title.isEmpty())
        tabName = info.artist + " - " + info.title;
    if(tabName.isEmpty())
        tabName = tr("New");

    Tab *tab = new Tab(info, ui->tabWidget);
    tab->setOptions(options);
    connect(tab,SIGNAL(setSaveIcon(int,bool)),this,SLOT(displaySaveIcon(int,bool)));
    connect(this,SIGNAL(setColorsEnabled(bool)),tab,SLOT(enableColors(bool)));

    QAction *action = new QAction(tabName,this);
    action->setIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton ) );

    mapTabAction.insert(action,tab);

    ui->menuTabs->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(pressGoTo()));

    int index = ui->tabWidget->addTab(tab,tabName);

    if(options.selectNewTab)
        ui->tabWidget->setCurrentIndex( index );

    return index;
}

void MainWindow::displaySaveIcon(int index, bool state)
{
    if(index<0)
        index = ui->tabWidget->indexOf((Tab*)sender());

    if(index>=ui->tabWidget->count())
        return;
    if(state)
        ui->tabWidget->setTabIcon(index, QIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton )) );
    else
        ui->tabWidget->setTabIcon(index, QIcon());
}

void MainWindow::pressNew(QString text)
{
    XTAinfo info("","");
    info.text = text;
    addTab(info);
}

void MainWindow::pressOpen()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this,tr("Open file"),options.defaultPath.isEmpty()?QDir::homePath():options.defaultPath,"*.xta *.txt");

    if(filepaths.isEmpty())
        return;

    foreach(QString filepath, filepaths){
        QFileInfo fi(filepath);

        XTAinfo info(fi.filePath(), fi.fileName());
        info = xta->parse(filepath);

        addTab( info );
    }
}

void MainWindow::pressOpenFolder()
{
    QString folderpath = QFileDialog::getExistingDirectory(this,tr("Open file"),options.defaultPath.isEmpty()?QDir::homePath():options.defaultPath);

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
    Tab *currentTab = getCurrentTab();
    XTAinfo info = currentTab->getXTA();
    if(info.filename.isEmpty()){
        pressSaveAs();
    }else{
        xta->save(info.filename,info);
        currentTab->saved();
        displaySaveIcon(ui->tabWidget->indexOf(currentTab),false);
    }
}

void MainWindow::pressSaveAs()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab *currentTab = getCurrentTab();
    XTAinfo info = currentTab->getXTA();

    QString selectedFilter;

    QString sample = (options.defaultPath.isEmpty()?QDir::homePath():options.defaultPath) + QDir::separator();
    if(!info.artist.isEmpty() && !info.title.isEmpty())
        sample += info.artist + " - " + info.title + ".xta";


    QString filepath = QFileDialog::getSaveFileName(this,tr("Save as"),sample,QString("%1;;%2").arg(tr("XTA files (*.xta)"),tr("TXT files (*.txt)")),&selectedFilter,QFileDialog::DontConfirmOverwrite);

    if(!filepath.isEmpty()){
        QFileInfo fi(filepath);
        if(selectedFilter==tr("XTA files (*.xta)") && fi.suffix()!="xta"){
            filepath.push_back(".xta");
        }
        if(selectedFilter==tr("TXT files (*.txt)") && fi.suffix()!="txt"){
            filepath.push_back(".txt");
        }
        fi.setFile(filepath);

        if(QFile::exists(filepath)){
            int button = QMessageBox::warning(this, windowTitle(), tr("'%1' already exists.\nDo you want to replace it?")
                                              .arg(fi.fileName()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if(button == QMessageBox::No){
                return;
            }
        }

        xta->save(filepath,info);
        currentTab->saved();
        displaySaveIcon(ui->tabWidget->indexOf(currentTab),false);
    }
}

void MainWindow::pressClose()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* currentTab = getCurrentTab();
    if(currentTab->isModified()){
        int button = QMessageBox::warning(this,"modified",QString("Do you want to save : %1").arg(ui->tabWidget->tabText(ui->tabWidget->currentIndex())),QMessageBox::Yes,QMessageBox::No);
        if(button==QMessageBox::Yes){
            pressSave();
        }
    }

    QAction* action = mapTabAction.key(currentTab);

    mapTabAction.remove(action);

    delete action;
    delete ui->tabWidget->currentWidget();
}

void MainWindow::pressCloseAll()
{
    while(ui->tabWidget->count()!=0){
        pressClose();
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
    getCurrentTab()->selectAll();
}

void MainWindow::pressUndo()
{
    if(ui->tabWidget->currentIndex()<0) return;
    getCurrentTab()->undo();
}

void MainWindow::pressRedo()
{
    if(ui->tabWidget->currentIndex()<0) return;
    getCurrentTab()->redo();
}

void MainWindow::pressCut()
{
    if(ui->tabWidget->currentIndex()<0) return;
    getCurrentTab()->cut();
}

void MainWindow::pressCopy()
{
    if(ui->tabWidget->currentIndex()<0) return;
    getCurrentTab()->copy();
}

void MainWindow::pressPaste()
{
    if(ui->tabWidget->currentIndex()<0) return;
    getCurrentTab()->paste();
}

void MainWindow::pressSearchLyrics()
{
    QString search;
    if(ui->tabWidget->currentIndex()>=0){
        XTAinfo info = getCurrentTab()->getXTA();
        search += info.artist + " " + info.title;
        search.trimmed();
    }
    search.prepend(tr("lyrics") + " ");

    bool ok;
    search = QInputDialog::getText(this,tr("Search lyrics"),tr("Search text:"),QLineEdit::Normal,search,&ok);
    if(!ok)
        return;

    search = QString("https://www.google.fr/#hl=fr&q=%1").arg(search);

    QDesktopServices::openUrl(QUrl(search));
}

void MainWindow::pressSearchXTA()
{
    QString search;
    if(ui->tabWidget->currentIndex()>=0){
        XTAinfo info = getCurrentTab()->getXTA();
        search += info.artist + " " + info.title;
        search.trimmed();
    }

    bool ok;
    search = QInputDialog::getText(this,tr("Search lyrics"),tr("Search text:"),QLineEdit::Normal,search,&ok);
    if(!ok)
        return;

    search = QString("http://guythoun.free.fr/TabS/xta/index.php?page=search&q=%1").arg(search);

    QDesktopServices::openUrl(QUrl(search));
}

void MainWindow::pressPreference()
{
    options.colors.push_back( ColorRegExp("\\b[A-G]#(?!m)",Qt::red,QFont::Normal,0) );
    Options *opt = new Options(options,this);
    if(opt->exec()){
        OptionsValues o = opt->getOptions();

        if(o.enableColors!=options.enableColors)
            emit setColorsEnabled(o.enableColors);

        options = o;

        options.save(this);
    }
}
