#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    options.parse(this);

    ui->actionFull_Screen->setChecked(options.openSizeMode==2);

    switch(options.openSizeMode){
    case 0 : this->setWindowState(Qt::WindowNoState); break;
    case 1 : this->setWindowState(Qt::WindowMaximized); break;
    case 2 : this->setWindowState(Qt::WindowFullScreen); break;
    default : this->setWindowState(Qt::WindowNoState); break;
    }

    readRecent();

    setUpMenu();

    setUpToolBar();

    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));

    xta = new XTA(this->centralWidget());
    chords = new Chords(this->centralWidget());

    /////////////////////////// TEST /////////////////////////////////////

    //QFontDatabase db;
    //qDebug() << db.families();

    QString testFile = "test.xta";

    if(qApp->argc()==2){
        testFile = qApp->arguments().at(1);
    }

    if(QFile::exists(testFile)){
        XTAinfo info = xta->parse(testFile);
        int index = addTab(info);
        addRecent(info);

        ui->tabWidget->setCurrentIndex( index );
    }

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


    /////////////////////////// TEST /////////////////////////////////////

    if(options.reOpenPreviousTabs){
        pressOpenPrevious();
    }

    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseRequest(int)));
    connect(ui->actionSearch_lyrics,SIGNAL(triggered()),this,SLOT(pressSearchLyrics()));
    connect(ui->actionSearch_XTA,SIGNAL(triggered()),this,SLOT(pressSearchXTA()));
    connect(ui->actionPreferences,SIGNAL(triggered()),this,SLOT(pressPreference()));
    connect(ui->actionClear,SIGNAL(triggered()),this,SLOT(clearRecent()));
    connect(ui->actionOpen_previous_tabs,SIGNAL(triggered()),this,SLOT(pressOpenPrevious()));
    connect(ui->actionChords_manager,SIGNAL(triggered()),this,SLOT(pressChordsManager()));

    connect(ui->actionEdit_mode,SIGNAL(triggered()),this,SLOT(pressEditMode()));
    connect(ui->actionRead_only_mode,SIGNAL(triggered()),this,SLOT(pressReadOnlyMode()));
}

MainWindow::~MainWindow()
{
    QList<QString> paths;
    for(int i=0;i<ui->tabWidget->count();i++){
        Tab *tab = (Tab*)ui->tabWidget->widget(i);
        paths << tab->getXTA().filepath;
    }
    for(int i=0;i<recent.size();i++){
        recent[i].wasOpen = paths.contains( recent[i].path );
    }
    saveRecent();
    pressCloseAll();
    delete ui;
}

void MainWindow::setUpMenu()
{
    QMap<QString,QMenu*> mapMenu;   //QMap< artsit, QMenu* >
    QMap<QString,QList<QPair<QString,QString> > > mapTitle; //QMap< artist, QList< QPair < title, path > > >
    QList< QPair<QDateTime,QPair<QString,QString> > > listDate; //QList < QPair< date, QPair< "artiste - title", path > > >

    for(int i=0;i<recent.size();i++){
        QString text;
        if(recent[i].title.isEmpty())
            text = recent[i].path;
        else
            text = recent[i].artist + " - " + recent[i].title;

        listDate.push_back( QPair<QDateTime,QPair<QString,QString> >( recent[i].date, QPair<QString,QString>(text,recent[i].path) ) );

        if(!recent[i].artist.isEmpty() && !recent[i].title.isEmpty()){
            if(!mapMenu.contains(recent[i].artist)){
                mapMenu.insert(recent[i].artist,new QMenu(recent[i].artist));
            }
            mapTitle[recent[i].artist].push_back( QPair<QString,QString>(recent[i].title,recent[i].path) );
        }
    }

    qSort(listDate.begin(),listDate.end(),QPairFirstComparerInverse());

    for(int i=0;i<listDate.size();i++){
        QAction *action = ui->menuBy_date->addAction( listDate.at(i).second.first );
        action->setData( listDate.at(i).second.second );
        recentAction.push_back( action );
        connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
    }

    for(int i=0;i<qMin(5,listDate.size());i++){
        QAction *action = ui->menuRe_Open->addAction( listDate.at(i).second.first );
        action->setData( listDate.at(i).second.second );
        recentAction.push_back( action );
        connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
    }


    QList<QString> listMenu = mapMenu.keys();
    for(int i=0;i<listMenu.size();i++){
        QMenu *menu = mapMenu[listMenu[i]];
        ui->menuBy_artist->addMenu( menu );
        recentAction.push_back( menu );

        QList<QPair<QString,QString> > list = mapTitle[listMenu[i]];
        qSort(list.begin(),list.end(),QPairFirstComparer());

        for(int j=0;j<list.size();j++){
            QAction *action = menu->addAction( list[j].first );
            action->setData( list[j].second );
            //recentAction.push_back( action );
            connect(action,SIGNAL(triggered()),this,SLOT(openFile()));
        }
    }

    ui->menuBy_artist->setEnabled(!listMenu.isEmpty());
    ui->menuBy_date->setEnabled(!listDate.isEmpty());
}

void MainWindow::setUpToolBar()
{
    ui->actionPrevious->setIcon( this->style()->standardIcon(QStyle::SP_ArrowLeft) );
    ui->actionNext->setIcon( this->style()->standardIcon(QStyle::SP_ArrowRight) );

    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionSave_as);
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
    ui->mainToolBar->addAction(ui->actionClose);

    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(pressNew()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(pressOpen()));
    connect(ui->actionOpen_Folder,SIGNAL(triggered()),this,SLOT(pressOpenFolder()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(pressSave()));
    connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(pressSaveAs()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(pressClose()));
    connect(ui->actionClose_All,SIGNAL(triggered()),this,SLOT(pressCloseAll()));

    connect(ui->actionPrevious,SIGNAL(triggered()),this,SLOT(pressPrevious()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(pressNext()));

    connect(ui->actionFull_Screen,SIGNAL(triggered()),this,SLOT(pressSetFullScreen()));

    connect(ui->actionSelect_All,SIGNAL(triggered()),this,SLOT(pressSelectAll()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(pressCut()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(pressCopy()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(pressPaste()));

    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(pressUndo()));
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(pressRedo()));
}

void MainWindow::currentTabChanged(int index){
    if(index<0 || index>=ui->tabWidget->count()) return;
    ui->actionUndo->setEnabled( getCurrentTab()->isUndoAvailable() );
    ui->actionRedo->setEnabled( getCurrentTab()->isRedoAvailable() );
}

void MainWindow::setUndoAvailable(bool state)
{
    if(sender()==ui->tabWidget->currentWidget()){
        ui->actionUndo->setEnabled( state );
    }
}

void MainWindow::setRedoAvailable(bool state)
{
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
    connect(tab,SIGNAL(undoAvailable(bool)),this,SLOT(setUndoAvailable(bool)));
    connect(tab,SIGNAL(redoAvailable(bool)),this,SLOT(setRedoAvailable(bool)));
    connect(this,SIGNAL(setColors(QList<ColorRegExp>)),tab,SLOT(setColors(QList<ColorRegExp>)));

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

        XTAinfo info(fi.absoluteFilePath(), fi.fileName());
        info = xta->parse(filepath);

        addTab( info );
        addRecent(info);
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
        XTAinfo info(fi.absoluteFilePath(), fi.fileName());
        info = xta->parse(filepath);
        addTab(info);
        addRecent(info);
    }
}

void MainWindow::pressOpenPrevious()
{
    for(int i=0;i<recent.size();i++){
        if(recent[i].wasOpen){
            QFileInfo fi(recent[i].path);

            XTAinfo info(fi.absoluteFilePath(), fi.fileName());
            info = xta->parse(fi.absoluteFilePath());

            addTab( info );
        }
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
        addRecent(info);
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

void MainWindow::pressSetFullScreen()
{
    if(this->windowState()!=Qt::WindowFullScreen){
        previousState = this->windowState();
        this->setWindowState(Qt::WindowFullScreen);
    }else{
        this->setWindowState(previousState);
    }
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
    OptionsForm *opt = new OptionsForm(options,this);
    if(opt->exec()){
        OptionsValues o = opt->getOptions();

        if(o.enableColors!=options.enableColors)
            emit setColorsEnabled(o.enableColors);

        emit setColors(o.colors);

        options = o;

        options.save(this);
    }
}

void MainWindow::pressChordsManager()
{
    ChordsManager *chordManager = new ChordsManager;
    chordManager->setChords( chords );
    chordManager->exec();
}

void MainWindow::readRecent()
{
    recent.clear();

    QFile xml_doc("recent.xml");
    if(!xml_doc.exists()){
        saveRecent();
        return;
    }

    if(!xml_doc.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,this->tr("Failed to open XML document"),tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(xml_doc.fileName()));
        return ;
    }

    QDomDocument *dom = new QDomDocument("docXML");
    if (!dom->setContent(&xml_doc)){
        xml_doc.close();
        QMessageBox::warning(this,this->tr("Error opening the XML document"),tr("The XML document could not be assigned to the object QDomDocument."));
        return ;
    }

    QDomElement dom_element = dom->documentElement();
    if(dom_element.tagName()=="recent"){
        QDomNode node = dom_element.firstChild();

        while(!node.isNull())
        {
            QDomElement element = node.toElement();

            if(element.tagName()=="file"){
                RecentFile rf;
                rf.title = element.attribute("title",tr("empty_title"));
                rf.artist = element.attribute("artist",tr("empty_artist"));
                rf.path = element.attribute("path","");
                rf.date = QDateTime::fromString(element.attribute("date",""),"dd MM yy HH mm ss");
                rf.wasOpen = element.attribute("wasOpen","false")=="true";
                recent.push_back(rf);
            }
            node = node.nextSibling();
        }
    }

    xml_doc.close();

}

void MainWindow::saveRecent()
{
    QFile file("recent.xml");
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);

    QDomDocument dom;

    QDomElement mainNode = dom.createElement("recent");
    dom.appendChild(mainNode);

    {
        foreach(RecentFile recentFile, recent){
            QDomElement elem = dom.createElement("file");
            elem.setAttribute("title",recentFile.title);
            elem.setAttribute("artist",recentFile.artist);
            elem.setAttribute("path",recentFile.path);
            elem.setAttribute("date",recentFile.date.toString("dd MM yy HH mm ss"));
            if(recentFile.wasOpen)
                elem.setAttribute("wasOpen","true");
            mainNode.appendChild(elem);
        }

    }

    stream << dom.toString();

    file.close();
}

void MainWindow::clearRecent()
{
    for(int i=0;i<recentAction.size();i++){
        if(recentAction.at(i)!=0)
            delete recentAction[i];
    }
    recentAction.clear();
    recent.clear();
    saveRecent();
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
    for(int i=0;i<recent.size();i++){
        if(recent.at(i).path == info.filepath){
            recent[i].date = QDateTime::currentDateTime();
            return;
        }
    }
    RecentFile rf;
    rf.title = info.title;
    rf.artist = info.artist;
    rf.path = info.filepath;
    rf.date = QDateTime::currentDateTime();
    recent.push_back(rf);
    updateRecent();
}

void MainWindow::openFile()
{
    QString filepath = ((QAction*)sender())->data().toString();
    QFileInfo fi(filepath);

    XTAinfo info(fi.absoluteFilePath(), fi.fileName());
    info = xta->parse(fi.absoluteFilePath());

    addTab( info );
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
