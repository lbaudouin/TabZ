#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    startTime = QTime::currentTime();

    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(-1);

    //Parse options (read options.xml file)
    options = new Options();
    options->load();

    //Read recent files list
    recent = new Recent;
    recentList = recent->load();

    //Setup UI
    setUpMenu();
    setUpToolBar();

    //Read default folder
    loadFileList();

    //Init objects
    xta = new XTA();
    chords = new Chords();

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseRequest(int)));
    connect(ui->tabWidget,SIGNAL(tabMoved(int,int)),this,SLOT(tabMoved(int,int)));

    connect(ui->actionSearch_lyrics,SIGNAL(triggered()),this,SLOT(pressSearchLyrics()));
    connect(ui->actionSearch_XTA,SIGNAL(triggered()),this,SLOT(pressSearchXTA()));
    connect(ui->actionDownload_XTA,SIGNAL(triggered()),this,SLOT(pressDownloadXTA()));
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(pressPreference()));
    connect(ui->actionClear,SIGNAL(triggered()),this,SLOT(clearRecent()));
    connect(ui->actionOpen_previous_tabs,SIGNAL(triggered()),this,SLOT(pressOpenPrevious()));
    connect(ui->actionChords_manager,SIGNAL(triggered()),this,SLOT(pressChordsManager()));

    connect(ui->actionEdit_mode,SIGNAL(triggered()),this,SLOT(pressEditMode()));
    connect(ui->actionRead_only_mode,SIGNAL(triggered()),this,SLOT(pressReadOnlyMode()));

    connect(ui->actionPreview,SIGNAL(triggered()),this,SLOT(pressPreview()));
    connect(ui->actionPrint,SIGNAL(triggered()),this,SLOT(pressPrint()));

    //Set selected window mode
    if(options->values()->openSizeMode==3){
        Qt::WindowState state = (Qt::WindowState)(options->values()->lastSizeMode);
        if(state==Qt::WindowFullScreen){
            ui->actionFull_Screen->setChecked(true);
        }else{
            this->setWindowState( state );
            this->setGeometry(options->values()->lastGeometry);
        }
    }else{
        switch(options->values()->openSizeMode){
        case 0 : this->setWindowState(Qt::WindowNoState); break;
        case 1 : this->setWindowState(Qt::WindowMaximized); break;
        case 2 : /*this->setWindowState(Qt::WindowFullScreen);*/ ui->actionFull_Screen->setChecked(true); break;
        default : this->setWindowState(Qt::WindowNoState);  break;
        }
    }

    if(options->values()->reOpenPreviousTabs){
        pressOpenPrevious();
    }

    /////////////////////////// TEST /////////////////////////////////////
#if 1
    if(ui->tabWidget->count()==0){
        if(QFile::exists("test.xta")){
            XTAinfo info = xta->load("test.xta");
            int index = addTab(info);
            addRecent(info);

            ui->tabWidget->setCurrentIndex( index );
        }
    }
#endif
    /////////////////////////// TEST /////////////////////////////////////
}

/** Destroy main window
  Save current tab state (open or not) in recent.xml
  Close all tabs
*/
MainWindow::~MainWindow()
{
    QList<QString> paths;
    for(int i=0;i<ui->tabWidget->count();i++){
        Tab *tab = (Tab*)ui->tabWidget->widget(i);
        paths << tab->getXTA().filepath;
    }
    for(int i=0;i<recentList.size();i++){
        recentList[i].wasOpen = paths.contains( recentList[i].path );
    }
    recent->save(recentList);

    options->values()->lastGeometry = this->geometry();
    options->values()->lastSizeMode = this->windowState();
    options->save();

    pressCloseAll();
    delete ui;
}

/** Set up the menu (recent files)
  Sort recent files by artist or by date
  Display in the main menu
*/
void MainWindow::setUpMenu()
{
    //Sort list by date
    qSort(recentList.begin(),recentList.end(),RecentDateComparer());

    //Add all recent files to 're-open/by date' menu
    for(int i=0;i<recentList.size();i++){
        QAction *action = ui->menuBy_date->addAction( recentList.at(i).getName() );
        action->setData( recentList.at(i).path );
        recentAction.push_back( action );
        connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
    }

    //Add 5 first recent file to the 're-open' menu
    for(int i=0;i<qMin(5,recentList.size());i++){
        QAction *action = ui->menuRe_Open->addAction( recentList.at(i).getName() );
        action->setData( recentList.at(i).path );
        recentAction.push_back( action );
        connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
    }

    //Sort list by artist
    qSort(recentList.begin(),recentList.end(),RecentNameComparer());
    //rf_listqDebug() << rf_list;

    QMap<QString,QList<RecentFile> > mapArtist;
    QMap<QString,QMenu*> mapMenu;

    for(int i=0;i<recentList.size();i++){
        RecentFile rf = recentList[i];

        //Create menu with artist name
        if(!rf.artist.isEmpty() && !rf.title.isEmpty()){
            if(!mapMenu.contains(rf.artist)){
                QMenu *menu = new QMenu(rf.artist);
                mapMenu.insert(rf.artist, menu);

                ui->menuBy_artist->addMenu( menu );
            }
            mapArtist[rf.artist].push_back( rf );
        }
    }

    QList<QString> listArtist = mapArtist.keys();
    for(int i=0;i<listArtist.size();i++){
        //Sort by title
        QList<RecentFile> list = mapArtist[listArtist[i]];
        qSort(list.begin(),list.end(),RecentTitleComparer());

        //get artist menu
        QMenu *menu = mapMenu[listArtist[i]];

        for(int j=0;j<list.size();j++){
            QAction *action = menu->addAction( list[j].title );
            action->setData( list[j].path );
            recentAction.push_back( action );
            connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
        }
    }

    //Enable menus
    ui->menuBy_artist->setEnabled(!listArtist.isEmpty());
    ui->menuBy_date->setEnabled(!recentList.isEmpty());
}

/** Set up the tool bar
**/
void MainWindow::setUpToolBar()
{
    ui->mainToolBar->setFloatable(false);

    ui->actionPrevious->setIcon( this->style()->standardIcon(QStyle::SP_ArrowLeft) );
    ui->actionNext->setIcon( this->style()->standardIcon(QStyle::SP_ArrowRight) );

    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionSave_as);
    ui->mainToolBar->addAction(ui->actionExport_PDF);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionUndo);
    ui->mainToolBar->addAction(ui->actionRedo);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionCut);
    ui->mainToolBar->addAction(ui->actionCopy);
    ui->mainToolBar->addAction(ui->actionPaste);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionFull_Screen);
    ui->mainToolBar->addAction(ui->actionEdit_mode);
    ui->mainToolBar->addAction(ui->actionRead_only_mode);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionOpen_Mp3);
    ui->mainToolBar->addAction(ui->actionOpen_Guitar_Pro);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actionClose);


    completer = new MyCompleter(QStringList(),this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);

    smartEdit = new MyCompleterLineEdit;
    smartEdit->setMinimumWidth(250);
    smartEdit->setCompleter(completer);

    connect(smartEdit,SIGNAL(selectionChanged(QString)),this,SLOT(hintSelected(QString)));

    QLabel *label = new QLabel;
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    ui->mainToolBar->addWidget(label);
    ui->mainToolBar->addWidget(smartEdit);

    ui->menuBar->addSeparator();
    exitFullScreenAction = ui->menuBar->addAction(tr("Exit Full Screen"),this,SLOT(pressExitFullScreen()));
    exitFullScreenAction->setShortcut(Qt::Key_Escape);
    exitFullScreenAction->setVisible(false);

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(pressNew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(pressOpen()));
    connect(ui->actionOpen_Folder,SIGNAL(triggered()),this,SLOT(pressOpenFolder()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(pressSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(pressSaveAs()));
    connect(ui->actionExport_PDF,SIGNAL(triggered()),this,SLOT(pressExportPDF()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(pressClose()));
    connect(ui->actionClose_All,SIGNAL(triggered()),this,SLOT(pressCloseAll()));

    connect(ui->actionPrevious,SIGNAL(triggered()),this,SLOT(pressPrevious()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(pressNext()));

    connect(ui->actionFull_Screen,SIGNAL(toggled(bool)),this,SLOT(pressSetFullScreen(bool)));

    connect(ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(pressSelectAll()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(pressCut()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(pressCopy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(pressPaste()));

    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(pressFind()));
    connect(ui->actionReplace,SIGNAL(triggered()),this,SLOT(pressReplace()));

    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(pressUndo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(pressRedo()));

    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(pressAbout()));

    connect(ui->actionOpen_Mp3,SIGNAL(triggered()),this,SLOT(pressOpenMp3()));
    connect(ui->actionOpen_Guitar_Pro,SIGNAL(triggered()),this,SLOT(pressOpenGP()));

    moveToolBar(options->values()->mainToolBarPosition);
}

void MainWindow::moveToolBar(int toolBarPosition)
{
    switch(toolBarPosition){
    case 0: this->addToolBar(Qt::TopToolBarArea,ui->mainToolBar); break;
    case 1: this->addToolBar(Qt::LeftToolBarArea,ui->mainToolBar); break;
    case 2: this->addToolBar(Qt::RightToolBarArea,ui->mainToolBar); break;
    case 3: this->addToolBar(Qt::BottomToolBarArea,ui->mainToolBar); break;
    default: this->addToolBar(Qt::TopToolBarArea,ui->mainToolBar); break;
    }
}

/** Function called when current tab changed
  @param index numero of current tab
**/
void MainWindow::currentTabChanged(int index){
    if(index<0 || index>ui->tabWidget->count()) return;
    Tab* tab = getCurrentTab();
    XTAinfo info = tab->getXTA();

    ui->actionUndo->setEnabled( tab->isUndoAvailable() );
    ui->actionRedo->setEnabled( tab->isRedoAvailable() );

    ui->actionOpen_Mp3->setDisabled( info.file_mp3.isEmpty() );
    ui->actionOpen_Guitar_Pro->setDisabled( info.file_gp.isEmpty() );

    ui->actionRead_only_mode->setVisible(tab->isEditable());
    ui->actionEdit_mode->setVisible(!tab->isEditable());
}

/** Function called when a tab moved
  @param from previous index
  @param to new index
  @warning only automatically moved tab
**/
void MainWindow::tabMoved(int /*from*/, int /*to*/)
{
    /*qDebug() << from << to;
    QAction *action = ((Tab*)ui->tabWidget->widget(from))->getAction();
    if(to+1<ui->tabWidget->count()){
        QAction *actionNext = ((Tab*)ui->tabWidget->widget(to+1))->getAction();
        ui->menuTabs->insertAction(actionNext,action);
        qDebug() << "before" << to+1;
    }else{
        ui->menuTabs->removeAction(action);
        ui->menuTabs->insertAction(0,action);
        qDebug() << "At end";
    }*/
    for(int i=0;i<listTabAction.size();i++){
        ui->menuTabs->removeAction(listTabAction[i]);
    }

    for(int i=0;i<ui->tabWidget->count();i++){
        QAction *action = ((Tab*)ui->tabWidget->widget(i))->getAction();
        action->setShortcut("Ctrl+"+QString::number(i+1));
        ui->menuTabs->insertAction(0,action);
    }
}

/** Enable/Disable undo icon
  @param state new state sended by a tab
**/
void MainWindow::setUndoAvailable(bool state)
{
    //sender() is the tab who send the signal
    if(sender()==ui->tabWidget->currentWidget()){
        ui->actionUndo->setEnabled( state );
    }
}

/** Enable/Disable redo icon
  @param state new state sended by a tab
**/
void MainWindow::setRedoAvailable(bool state)
{
    //sender() is the tab who send the signal
    if(sender()==ui->tabWidget->currentWidget()){
        ui->actionRedo->setEnabled( state );
    }
}

void MainWindow::tabCloseRequest(int index)
{
    QWidget *currentWidget = ui->tabWidget->currentWidget();
    if(index<0 || index>=ui->tabWidget->count()) return;

    ui->tabWidget->setCurrentIndex(index);

    pressClose();

    ui->tabWidget->setCurrentWidget(currentWidget);
}

void MainWindow::changeTabName(QString name)
{
    Tab *tab = (Tab*)sender();
    int index = ui->tabWidget->indexOf(tab);
    if(index<0) return;
    ui->tabWidget->setTabText(index,name);
}

Tab* MainWindow::getCurrentTab()
{
    if(ui->tabWidget->count()==0) return 0;
    else return (Tab*)ui->tabWidget->currentWidget();
}

int MainWindow::addTab(XTAinfo info, bool forceEditable)
{
    QString tabName = info.filename;
    if(!info.artist.isEmpty() && !info.title.isEmpty())
        tabName = info.artist + " - " + info.title;
    if(tabName.isEmpty())
        tabName = tr("New");

    Tab *tab = new Tab(info, chords, options->cloneValues(), ui->tabWidget);

    connect(tab,SIGNAL(setSaveIcon(int,bool)),this,SLOT(displaySaveIcon(int,bool)));
    //connect(this,SIGNAL(setColorsEnabled(bool)),tab,SLOT(enableColors(bool)));
    connect(tab,SIGNAL(undoAvailable(bool)),this,SLOT(setUndoAvailable(bool)));
    connect(tab,SIGNAL(redoAvailable(bool)),this,SLOT(setRedoAvailable(bool)));
    connect(tab,SIGNAL(changeTabName(QString)),this,SLOT(changeTabName(QString)));
    connect(this,SIGNAL(setColors(QList<ColorRegExp>)),tab,SLOT(setColors(QList<ColorRegExp>)));
    connect(this,SIGNAL(optionsChanged(OptionsValues)),tab,SLOT(setOptions(OptionsValues)));

    QAction *action = new QAction(tabName,this);
    action->setIcon( this->style()->standardIcon(QStyle::SP_DialogSaveButton ) );
    action->setShortcut("Ctrl+"+QString::number(ui->tabWidget->count()+1));
    tab->setAction( action );

    listTabAction << action;

    mapTabAction.insert(action,tab);

    ui->menuTabs->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(pressGoTo()));

    int index = ui->tabWidget->addTab(tab,tabName);

    if(options->values()->selectNewTab || ui->tabWidget->count()==1){
        ui->tabWidget->setCurrentIndex( index );
    }

    if(forceEditable){
        pressEditMode();
    }else{
        if(options->values()->openReadOnly){
            pressReadOnlyMode();
        }else{
            pressEditMode();
        }
    }

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
    int index = addTab(info,true);
    ((Tab*)(ui->tabWidget->widget(index)))->toggleInfo();
}

void MainWindow::loadFiles(QStringList files)
{
    foreach(QString filepath, files){
        QFileInfo fi(filepath);

        bool ok;
        XTAinfo info = xta->load(fi.absoluteFilePath(),&ok);

        if(ok){
            addTab( info );
            addRecent( info );
        }else{
            QMessageBox::critical(this,tr("Error"),tr("Can't open file: %1").arg(filepath));
        }
    }
}

void MainWindow::pressOpen()
{
    QStringList filepaths = QFileDialog::getOpenFileNames(this,tr("Open file"),options->values()->defaultPath.isEmpty()?QDir::homePath():options->values()->defaultPath,"*.xta *.txt");

    if(filepaths.isEmpty())
        return;

    loadFiles(filepaths);
}

void MainWindow::pressOpenFolder()
{
    QString folderpath = QFileDialog::getExistingDirectory(this,tr("Open file"),options->values()->defaultPath.isEmpty()?QDir::homePath():options->values()->defaultPath);

    if(folderpath.isEmpty())
        return;

    QStringList filters;
    filters << "*.xta" << "*.XTA" << "*.txt" << "*.TXT";

    QDir dir(folderpath);
    dir.setNameFilters(filters);
    QStringList listFiles = dir.entryList();

    loadFiles(listFiles);
}

void MainWindow::pressOpenPrevious()
{
    QStringList listFiles;
    for(int i=0;i<recentList.size();i++){
        if(recentList[i].wasOpen){
            listFiles << recentList[i].path;
        }
    }
    loadFiles(listFiles);
}

void MainWindow::pressSave()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab *currentTab = getCurrentTab();
    XTAinfo info = currentTab->getXTA();
    if(info.filepath.isEmpty()){
        pressSaveAs();
    }else{
        xta->save(info.filepath,info);
        addRecent(info);
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

    QString sample = (options->values()->defaultPath.isEmpty()?QDir::homePath():options->values()->defaultPath) + QDir::separator();

    QDir defaultFolder(options->values()->defaultPath);
    if(defaultFolder.exists()){
        if(defaultFolder.cd(info.artist)){
            sample = defaultFolder.absolutePath() + QDir::separator();
        }
    }

    sample += info.createFileName() +  ".xta";

    QString filepath = QFileDialog::getSaveFileName(this,tr("Save as"),sample,QString("%1;;%2").arg(tr("XTA files (*.xta)"),tr("TXT files (*.txt)")),&selectedFilter,QFileDialog::DontConfirmOverwrite);

    if(!filepath.isEmpty()){
        QFileInfo fi(filepath);
        if(selectedFilter==tr("XTA files (*.xta)") && !fi.absoluteFilePath().endsWith(".xta",Qt::CaseInsensitive)){
            filepath.push_back(".xta");
        }
        if(selectedFilter==tr("TXT files (*.txt)") && !fi.absoluteFilePath().endsWith(".txt",Qt::CaseInsensitive)){
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
        addRecent(info);
        currentTab->saved(fi.absoluteFilePath());
        displaySaveIcon(ui->tabWidget->indexOf(currentTab),false);
    }
}

void MainWindow::pressClose()
{
    if(ui->tabWidget->count()<=0){
        this->close();
    }else{
        Tab* currentTab = getCurrentTab();
        if(currentTab->isModified()){
            int button = QMessageBox::warning(this,tr("Modified"),tr("Do you want to save : %1").arg(ui->tabWidget->tabText(ui->tabWidget->currentIndex())),QMessageBox::Yes,QMessageBox::No);
            if(button==QMessageBox::Yes){
                pressSave();
            }
        }

        QAction* action = mapTabAction.key(currentTab);

        mapTabAction.remove(action);
        listTabAction.removeAll(action);

        delete action;
        delete ui->tabWidget->currentWidget();

        for(int i=0;i<ui->tabWidget->count();i++){
            ((Tab*)ui->tabWidget->widget(i))->getAction()->setShortcut("Ctrl+"+QString::number(i+1));
        }
    }
}

void MainWindow::pressCloseAll()
{
    while(ui->tabWidget->count()!=0){
        pressClose();
    }
}

void MainWindow::pressSetFullScreen(bool state)
{
    if(state){
        previousState = this->windowState();
        this->setWindowState(Qt::WindowFullScreen);
        ui->mainToolBar->setVisible(false);
        //ui->mainToolBar->setOrientation(Qt::Vertical);
        ui->statusBar->setVisible(false);
        exitFullScreenAction->setVisible(true);
        if(ui->tabWidget->currentIndex()>=0){
            getCurrentTab()->setExpertMode(true);
        }
    }else{
        this->setWindowState(previousState);
        ui->mainToolBar->setVisible(true);
        //ui->mainToolBar->setOrientation(Qt::Horizontal);
        ui->statusBar->setVisible(true);
        exitFullScreenAction->setVisible(false);
        if(ui->tabWidget->currentIndex()>=0){
            getCurrentTab()->setExpertMode(false);
        }
    }
}

void MainWindow::pressExitFullScreen()
{
    ui->actionFull_Screen->setChecked(false);
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

void MainWindow::pressFind()
{
    if(getCurrentTab()!=0){
        FindReplaceDialog *dialog = new FindReplaceDialog;
        dialog->setFindOnly(true);
        dialog->setTextEdit(getCurrentTab()->getTextEdit());
        dialog->exec();
    }
}

void MainWindow::pressReplace()
{
    if(getCurrentTab()!=0){
        FindReplaceDialog *dialog = new FindReplaceDialog;
        dialog->setTextEdit(getCurrentTab()->getTextEdit());
        dialog->exec();
    }
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

void MainWindow::pressDownloadXTA()
{
    DownloadXTA *dXTA = new DownloadXTA(this);
    dXTA->setFolder( options->values()->defaultPath );
    dXTA->exec();
}

void MainWindow::pressPreference()
{
    OptionsForm *opt = new OptionsForm(options->cloneValues(),this);
    if(opt->exec()){
        OptionsValues ov = opt->getOptions();
        options->setValues(ov);
        options->save();
        moveToolBar(options->values()->mainToolBarPosition);
        emit optionsChanged(options->cloneValues());
        loadFileList();
    }
}

void MainWindow::pressChordsManager()
{
    ChordsManager *chordManager = new ChordsManager(chords);
    if(chordManager->exec()){
        chordManager->save();
    }
}

void MainWindow::clearRecent()
{
    for(int i=0;i<recentAction.size();i++){
        if(recentAction.at(i)!=0)
            delete recentAction[i];
    }
    recentAction.clear();
    recentList.clear();
    recent->save(recentList);
    updateRecent();
}

void MainWindow::updateRecent()
{
    for(int i=0;i<recentAction.size();i++){
        if(recentAction.at(i)!=0)
            delete recentAction[i];
    }
    recentAction.clear();
    setUpMenu();
}

void MainWindow::addRecent(XTAinfo &info)
{
    for(int i=0;i<recentList.size();i++){
        if(recentList.at(i).path == info.filepath){
            recentList[i].date = QDateTime::currentDateTime();
            return;
        }
    }
    if(info.filepath.isEmpty())
        return;

    RecentFile rf;
    rf.title = info.title;
    rf.artist = info.artist;
    rf.path = info.filepath;
    rf.date = QDateTime::currentDateTime();

    recentList.push_back(rf);
    updateRecent();
}

void MainWindow::openFile()
{
    QString filepath = ((QAction*)sender())->data().toString();
    loadFiles( QStringList() << filepath);
}

void MainWindow::restart(QString path)
{
    qDebug() << path;
    QProcess process;
    process.startDetached("\""+path+"\"");
    exit(1);
}

void MainWindow::pressEditMode()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab *tab = getCurrentTab();

    ui->actionRead_only_mode->setVisible(true);
    ui->actionEdit_mode->setVisible(false);

    tab->setEditable(true);
}

void MainWindow::pressReadOnlyMode()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab *tab = getCurrentTab();

    ui->actionRead_only_mode->setVisible(false);
    ui->actionEdit_mode->setVisible(true);

    tab->setEditable(false);
}

void MainWindow::pressPreview()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* tab = getCurrentTab();

    QPrintPreviewDialog *pDialog = new QPrintPreviewDialog(this,Qt::Window);
    //pDialog->setWindowFlags(Qt::WindowMaximizeButtonHint);
    connect(pDialog, SIGNAL(paintRequested(QPrinter*)), tab, SLOT(print(QPrinter*)));
    QPrinter *printer = pDialog->printer();
    printer->setPageMargins(options->values()->topMargin,options->values()->leftMargin,options->values()->rightMargin,options->values()->bottomMargin,QPrinter::Millimeter);
#if !defined(__WIN32__)
    QString default_filename = tab->getXTA().createFileName();
    printer->setOutputFileName(default_filename);
#endif
    pDialog->exec();
}

void MainWindow::pressPrint()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* tab = getCurrentTab();

    QPrintPreview *pDialog = new QPrintPreview(this,Qt::Dialog);
    connect(pDialog, SIGNAL(paintRequested(QPrinter*)), tab, SLOT(print(QPrinter*)));
    QPrinter *printer = pDialog->getPrinter();
    printer->setPageMargins(options->values()->topMargin,options->values()->leftMargin,options->values()->rightMargin,options->values()->bottomMargin,QPrinter::Millimeter);
#if !defined(__WIN32__)
    QString default_filename = tab->getXTA().createFileName();
    if(!default_filename.isEmpty()){
        if(!default_filename.endsWith(".pdf",Qt::CaseInsensitive))
            default_filename += ".pdf";
        printer->setOutputFileName(QDir::homePath() + QDir::separator() + default_filename);
    }
#endif

    printer->setOutputFormat(QPrinter::NativeFormat);
    pDialog->pressPrint();
}

void MainWindow::pressExportPDF()
{
    if(ui->tabWidget->currentIndex()<0) return;
    Tab* tab = getCurrentTab();

    QString default_filename = tab->getXTA().createFileName();
    default_filename += ".pdf";

    QString filename = QFileDialog::getSaveFileName(this,tr("Export PDF"), options->values()->defaultOutputFolder + QDir::separator() + default_filename, "PDF (*.pdf)");
    if(filename.isEmpty()) return;

    if(!filename.endsWith(".pdf",Qt::CaseInsensitive))
        filename += ".pdf";

    QPrintPreview *pDialog = new QPrintPreview(this,Qt::Dialog);
    connect(pDialog, SIGNAL(paintRequested(QPrinter*)), tab, SLOT(print(QPrinter*)));
    QPrinter *printer = pDialog->getPrinter();
    printer->setPageMargins(options->values()->topMargin,options->values()->leftMargin,options->values()->rightMargin,options->values()->bottomMargin,QPrinter::Millimeter);
    pDialog->exportPDF(filename);

    int ret = QMessageBox::information(this,tr("Export PDF"),tr("PDF exported : %1").arg(filename),QMessageBox::Close,QMessageBox::Open);
    if(ret==QMessageBox::Open){
#if defined(__WIN32__)
        QUrl url("file:///" + filename, QUrl::TolerantMode);
#else
        QUrl url("file://" + filename, QUrl::TolerantMode);
#endif
        //qDebug() << url;
        QDesktopServices::openUrl(url);
    }
}

void MainWindow::pressOpenMp3()
{
    if(ui->tabWidget->currentIndex()<0) return;
    XTAinfo info = getCurrentTab()->getXTA();
    if(!info.file_mp3.isEmpty()){
        if(QFile::exists(info.file_mp3)){
            QDesktopServices::openUrl(QUrl(info.file_mp3));
        }
    }
}

void MainWindow::pressOpenGP()
{
    if(ui->tabWidget->currentIndex()<0) return;
    XTAinfo info = getCurrentTab()->getXTA();
    if(!info.file_gp.isEmpty()){
        if(QFile::exists(info.file_gp)){
            QDesktopServices::openUrl(QUrl(info.file_gp));
        }
    }
}

void MainWindow::loadFileList()
{    
    if(options->values()->defaultPath.isEmpty()){
        smartEdit->setEnabled(false);
        return;
    }

    mapFiles.clear();

    QDirIterator it(options->values()->defaultPath, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        QString filename = it.fileName();
        QFileInfo info = it.fileInfo();
        if(!QString::compare(info.suffix(),"xta",Qt::CaseInsensitive)){
            filename.chop(4);
            mapFiles[filename] << path;
        }
    }


    listFiles = mapFiles.keys();
    completer->setStringList(listFiles);

    if(listFiles.isEmpty()){
        smartEdit->setEnabled(false);
    }else{
        smartEdit->setEnabled(true);
    }
}

void MainWindow::hintSelected(QString item)
{
    if(mapFiles.contains(item)){
        loadFiles(QStringList() << mapFiles[item].at(0));
    }
}

/*void MainWindow::hintEnterPressed()
{
    hintSelected(smartEdit->text());
}*/

void MainWindow::pressAbout()
{
    QMessageBox::information(this,tr("About"), tr("Written by %1 (%2)\nVersion: %3","author, year, version").arg(QString::fromUtf8("Léo Baudouin"),"2013",version));
}

void MainWindow::handleMessage(const QString& message)
{
    enum Action { Nothing, Open, Print } action;

    action = Nothing;
    QString filename = message;
    if (message.toLower().startsWith("/print ")) {
        filename = filename.mid(7);
        action = Print;
    } else if (!message.isEmpty()) {
        action = Open;
    }
    if (action == Nothing) {
        emit needToShow();
        return;
    }

    switch(action) {
        case Print: break;
        case Open: {
            loadFiles(QStringList() << filename);
            emit needToShow();
            break;
        }
        default: break;
    };
}
