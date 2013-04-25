#include "tab.h"

Tab::Tab(XTAinfo xta, QWidget *parent) : QWidget(parent), info(xta), modified_info(xta), undoAvailable_(false), redoAvailable_(false), editable_(true), instrument_("Guitar","guitar",6)
{
    //TODO read instrument

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(6);
    //layout->setContentsMargins(11, 11, 11, 11);
    mainLayout->setObjectName(QString::fromUtf8("mainLayout"));

    allInfoWidget = new QWidget;

    QHBoxLayout *infoLayout = new QHBoxLayout;
    allInfoWidget->setLayout(infoLayout);
    mainLayout->addWidget(allInfoWidget);

    QHBoxLayout *i1 = new QHBoxLayout;
    QVBoxLayout *i2 = new QVBoxLayout;

    labelInfo = new QLabel("Info");
    QFont font = labelInfo->font();
    font.setBold(true);
    labelInfo->setFont(font);
    specialInfo = new QLabel("Info");
    //labelInfo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    i1->addWidget(labelInfo);
    i1->addWidget(specialInfo);


    QWidget *infoWidget = new QWidget;
    QFormLayout *formLayout = new QFormLayout;
    editTitle = new QLineEdit;
    formLayout->addRow(tr("Title:"),editTitle);
    editArtist = new QLineEdit;
    formLayout->addRow(tr("Artist:"),editArtist);
    editAlbum = new QLineEdit;
    formLayout->addRow(tr("Album:"),editAlbum);
    editTuning = new QLineEdit;
    formLayout->addRow(tr("Tuning:"),editTuning);
    editCapo = new QSpinBox();

    QSpacerItem *spacerSpinBox = new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Ignored);
    QHBoxLayout *layoutSpinBox = new QHBoxLayout;
    editCapo->setMinimum(0);
    editCapo->setMaximum(30);
    layoutSpinBox->addWidget(editCapo);
    layoutSpinBox->addSpacerItem(spacerSpinBox);
    formLayout->addRow(tr("Capo:"),layoutSpinBox);

    infoWidget->setLayout(formLayout);
    infoWidget->setVisible(false);

    i1->addWidget(infoWidget);


    QToolButton *hideShow = new QToolButton;
    hideShow->setCheckable(true);
    QIcon icon;
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarUnshadeButton).pixmap(16), QIcon::Normal, QIcon::Off );
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton).pixmap(16), QIcon::Normal, QIcon::On );
    hideShow->setIcon(icon);
    i2->addWidget(hideShow);

    //QSpacerItem *buttonSpacer = new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    //i2->addSpacerItem(buttonSpacer);

    infoLayout->addLayout(i1);
    infoLayout->addLayout(i2);

    connect(hideShow,SIGNAL(toggled(bool)),labelInfo,SLOT(setHidden(bool)));
    connect(hideShow,SIGNAL(toggled(bool)),specialInfo,SLOT(setHidden(bool)));
    connect(hideShow,SIGNAL(toggled(bool)),infoWidget,SLOT(setVisible(bool)));
    connect(hideShow,SIGNAL(clicked()),this,SLOT(updateTitle()));


    /*printer = new QPrinter;
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOrientation(QPrinter::Portrait);
    printer->setPaperSize(QPrinter::A4);
    printer->setPrintRange(QPrinter::PageRange);
    printer->setFullPage(true);*/

    printPreviewWidget = new QPrintPreview(/*printer*/);
    printer = printPreviewWidget->getPrinter();
    printer->setPageMargins(10,10,10,10,QPrinter::Millimeter);

    printer->setDocName( info.createFileName() );
    printPreviewWidget->setViewMode(QPrintPreviewWidget::AllPagesView);
    printPreviewWidget->setZoomMode(QPrintPreviewWidget::FitInView);

    connect(printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));

    previewLayout = new QVBoxLayout;
    previewLayout->addWidget(printPreviewWidget);
    printPreviewWidget->setVisible(false);

    edit = new QTextEdit;


    QFont editFont = edit->font();
    editFont.setFamily("DejaVu Sans Mono");
    //editFont.setFamily("Lucida Console");
    //editFont.setFixedPitch(true);

    edit->setFont(editFont);
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    highlighter = new Highlighter(edit->document());


    //edit->document()->setPageSize(QSizeF(10,10));

    //Highlighter *highlighter = new Highlighter(edit->document());

    chordLayout = new QVBoxLayout;
#if 0
    //Buttons
    QToolButton *buttonAdd = new QToolButton();
    buttonAdd->setIcon( QIcon(":images/add") );
    buttonAdd->setToolTip(tr("Add new chord"));
    buttonAdd->setStatusTip(tr("Add new chord"));
    QToolButton *buttonCopy = new QToolButton();
    buttonCopy->setIcon( QIcon(":images/copy") );
    buttonCopy->setToolTip(tr("Import from text"));
    buttonCopy->setStatusTip(tr("Import from text"));
    QToolButton *buttonImport = new QToolButton();
    buttonImport->setIcon( QIcon(":images/import") );
    buttonImport->setToolTip(tr("Import from XTA"));
    buttonImport->setStatusTip(tr("Import from XTA"));
    QToolButton *buttonRead = new QToolButton();
    buttonRead->setIcon( QIcon(":images/search") );
    buttonRead->setToolTip(tr("Manage"));
    buttonRead->setStatusTip(tr("Manage"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonCopy);
    buttonLayout->addWidget(buttonImport);
    buttonLayout->addWidget(buttonRead);

    connect(buttonAdd,SIGNAL(clicked()),this,SLOT(addNewChord()));
    connect(buttonImport,SIGNAL(clicked()),this,SLOT(importFromXTA()));
    connect(buttonCopy,SIGNAL(clicked()),this,SLOT(import()));
    connect(buttonRead,SIGNAL(clicked()),this,SLOT(read()));

    chordLayout->addLayout(buttonLayout);
#else
    QToolBar *chordToolBar = new QToolBar;
    chordToolBar->setMovable(false);
    chordToolBar->addAction(QIcon(":images/add"), tr("Add new chord"), this, SLOT(addNewChord()) );
    chordToolBar->addAction(QIcon(":images/copy"), tr("Import from text"), this, SLOT(import()) );
    chordToolBar->addAction(QIcon(":images/import"), tr("Import from XTA"), this, SLOT(importFromXTA()) );
    chordToolBar->addAction(QIcon(":images/search"), tr("Manage"), this, SLOT(read()) );

    chordLayout->addWidget(chordToolBar);
#endif


    //two columns displayer

    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *vi1 = new QVBoxLayout;
    QVBoxLayout *vi2 = new QVBoxLayout;
    QSpacerItem *s1 = new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    QSpacerItem *s2 = new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);

    chordLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    h->setSizeConstraint(QLayout::SetDefaultConstraint);

    h->addLayout(vi1);
    h->addLayout(vi2);

    v1 = new QVBoxLayout;
    v2 = new QVBoxLayout;

    vi1->addLayout(v1);
    vi2->addLayout(v2);
    vi1->addSpacerItem(s1);
    vi2->addSpacerItem(s2);

    QWidget *w2 = new QWidget;
    w2->setLayout(h);

    scrollArea =new QScrollArea();
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    scrollArea->setWidget(w2);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::Box);
    //scrollArea->raise();

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollArea->setMinimumWidth(0);

    chordLayout->addWidget(scrollArea);

    //QSpacerItem *spacer = new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Maximum);
    //chordsLayout->addSpacerItem(spacer);

    tabToolBar = new QToolBar;
    tabToolBar->setOrientation(Qt::Vertical);
    tabToolBar->setMovable(false);
    tabToolBar->addAction(QIcon(":images/insert-image"),tr("Insert image from file"), this, SLOT(insertImage()) );
    tabToolBar->addAction(QIcon(":images/insert-clipboard"),tr("Insert image from clipboard"), this, SLOT(insertClipboard()) );
    tabToolBar->addAction(QIcon(":images/tab"),tr("Insert tab"), this, SLOT(insertTab()) );
    //tabToolBar->addAction(QIcon("images/tab2.png"),tr("Insert tab"));
    //tabToolBar->addAction(QIcon("images/tab3.png"),tr("Insert tab"));
    //tabToolBar->addAction(QIcon("images/tab4.png"),tr("Insert tab"));
    tabToolBar->addAction(QIcon(":images/La-A"),tr("French to English"), this, SLOT(translateFrEn()) );
    tabToolBar->addAction(QIcon(":images/A-La"),tr("English to French"), this, SLOT(translateEnFr()) );

#if 1
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(tabToolBar,Qt::AlignTrailing);
    hlayout->addWidget(edit);
    //hlayout->addWidget(printPreviewWidget);
    hlayout->addLayout(previewLayout);
    hlayout->addLayout(chordLayout);
    mainLayout->addLayout(hlayout);
#else
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(edit);
    QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    w->setLayout(mainLayout);
    splitter->addWidget(w);
    layout->addWidget(splitter);
#endif


    editTitle->setText( info.title );
    editArtist->setText( info.artist );
    editAlbum->setText( info.album );
    editTuning->setText( info.tuning );
    editCapo->setValue( info.capo );
    connect(edit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(editTitle,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editArtist,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editAlbum,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editTuning,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editCapo,SIGNAL(valueChanged(int)),this,SLOT(capoChanged(int)));

    edit->setText(info.text);

    if(info.images.size()>0){
        QTextCursor cursor = edit->textCursor();
        for(int i=0;i<info.images.size();i++){
            QString url = QString("mydata://image%1.png").arg(i);

            edit->document()->addResource(QTextDocument::ImageResource, QUrl(url), QVariant(info.images[i]));

            QTextImageFormat imgTextFormat;
            imgTextFormat.setName(url);

            cursor =  edit->document()->find(QString(QChar::ObjectReplacementCharacter),cursor);
            cursor.insertText(QString(QChar::ObjectReplacementCharacter), imgTextFormat);
            //cursor.insertText(QString::fromUtf8("@@@image%1@@@").arg(i), imgTextFormat);
            edit->setTextCursor(cursor);
        }
    }

    edit->setTextInteractionFlags(Qt::TextEditorInteraction);
    edit->setAutoFormatting(QTextEdit::AutoNone);
    edit->setAcceptRichText(false);


    addChordsFromText(info.chords);

    updateTitle();

    connect(edit,SIGNAL(cursorPositionChanged()),this,SLOT(updateSelectedNote()));
    connect(edit,SIGNAL(undoAvailable(bool)),this,SLOT(setUndoAvailable(bool)));
    connect(edit,SIGNAL(redoAvailable(bool)),this,SLOT(setRedoAvailable(bool)));

}

void Tab::addChordsFromText(QString text)
{
    if(text.isEmpty())
        return;

    QTextStream stream(&text);

    QStringList list;

    while(!stream.atEnd()){
        QString line = stream.readLine();
        line.replace("\t"," ");
        line.replace(","," ");
        QStringList temp = line.split(" ",QString::SkipEmptyParts);
        if(temp.size()==0)
            continue;
        if(temp.size()==2){
            QString name = temp.at(0);
            QString fingers = temp.at(1);
            temp.clear();
            temp.push_back( name );
            for(int i=0;i<fingers.size();i++){
                temp.push_back( fingers.mid(i,1) );
            }
        }
        QString name = temp.at(0);
        QString fingers = "";
        for(int i=1;i<temp.size();i++)
            fingers += " " + temp.at(i);
        fingers.trimmed();

        list << name;
        addChord(name,fingers);
    }
    resizeLayout();

    highlighter->addPersonalText(list);
    highlighter->rehighlight();
    printPreviewWidget->updatePreview();
}

bool Tab::isUndoAvailable()
{
    return undoAvailable_;
}

bool Tab::isRedoAvailable()
{
    return redoAvailable_;
}

void Tab::updateTitle()
{
    if(modified_info.title.isEmpty()){
        labelInfo->setText( "(" + tr("No title") + ")");
    }else{
        labelInfo->setText( QString(tr("%1 - %2","artist - title")).arg(modified_info.artist,modified_info.title));
    }

    if(modified_info.capo>0 && modified_info.tuning!="EADGBE"){
        specialInfo->setText("\t " + tr("[Capo: %1, Tuning: %2]").arg(QString::number(modified_info.capo),modified_info.tuning));
    }else if(modified_info.capo>0){
        specialInfo->setText("\t " +tr("[Capo: %1]").arg(QString::number(modified_info.capo)));
    }else if(modified_info.tuning!="EADGBE") {
        specialInfo->setText("\t " +tr("[Tuning: %1]").arg(modified_info.tuning));
    }else{
        specialInfo->clear();
    }
}

void Tab::capoChanged(int)
{
    modified_info.capo = editCapo->value();

    if(!info.isEqual(modified_info)){
        emit setSaveIcon(-1,true);
    }else{
        emit setSaveIcon(-1,false);
    }
}

void Tab::infoChanged(QString)
{
    modified_info.title = editTitle->text();
    modified_info.artist = editArtist->text();
    modified_info.album = editAlbum->text();
    modified_info.tuning = editTuning->text();

    QString name;
    if(modified_info.title.isEmpty()){
        name = info.filename;
    }else{
        if(modified_info.artist.isEmpty()){
            name = modified_info.title;
        }else{
            name = modified_info.artist + " - " + modified_info.title;
        }
    }
    emit changeTabName(name);
    menuAction->setText(name);

    if(!info.isEqual(modified_info)){
        emit setSaveIcon(-1,true);
    }else{
        emit setSaveIcon(-1,false);
    }
}

void Tab::textChanged(QString)
{
   modified_info.text = edit->toPlainText();

   printer->setDocName( modified_info.createFileName() );

   if(!info.isEqual(modified_info)){
       emit setSaveIcon(-1,true);
   }else{
       emit setSaveIcon(-1,false);
   }
}

bool Tab::isModified()
{
    //info.diff(modified_info);
    return !info.isEqual(modified_info);
}

void Tab::saved(QString path)
{
    if(!path.isEmpty()){
        QFileInfo fi(path);
        modified_info.filepath = fi.absoluteFilePath();
        modified_info.filename = fi.fileName();
    }
    info = modified_info;
}

XTAinfo Tab::getXTA()
{
    modified_info.chords.clear();
    for(int i=0;i<currentChords.size();i++){
        modified_info.chords += currentChords.at(i).name;
        if(!currentChords[i].fingers.isEmpty()){
            modified_info.chords += " " + currentChords[i].fingers;
        }
        modified_info.chords += "\n";
    }

    QTextCursor cursor = edit->textCursor();
    cursor.setPosition(0);
    modified_info.refImages.clear();
    while(1){
        cursor = edit->document()->find(QString(QChar::ObjectReplacementCharacter),cursor);
        if(cursor.isNull()) break;
        bool ok;
        int index = cursor.charFormat().property(QTextFormat::ImageName).toString().section("image",1).section(".png",0,0).toInt(&ok);
        if(ok){
            modified_info.refImages << index;
        }
    }
    return modified_info;
}

void Tab::updateSelectedNote()
{
    int position =  edit->textCursor().columnNumber();
    QString line = edit->textCursor().block().text();
    QString word;
    for(int i=position-1;i>=0;i--){
        if(line.at(i)!=' ')
            word.push_front(line.at(i));
        else
            break;
    }
    for(int i=position;i<line.size();i++){
        if(line.at(i)!=' ')
            word.push_back(line.at(i));
        else
            break;
    }
    emit setSelected(word);
}


void Tab::deleteGuitar()
{
    Guitar *g = (Guitar*)sender();
    for(int i=0;i<currentChords.size();i++){
        if(currentChords[i].name==g->getName()){
            currentChords.removeAt(i);
            i--;
        }
    }
    chords.removeAll(g->getName());
    v1->removeWidget(g);
    v2->removeWidget(g);
    g->close();
    resizeLayout();
}

void Tab::resizeLayout()
{
    if(v1->count()>0 && v2->count()>0){
        scrollArea->setMinimumWidth(2.0 * 1.4 * optionsValues.chordSize.width());
    }else{
        if(v1->count()>0 || v2->count()>0){
            scrollArea->setMinimumWidth(1.4 * optionsValues.chordSize.width());
        }else{
            scrollArea->setMinimumWidth(0);
        }
    }
    scrollArea->update();
}

void Tab::addNewChord()
{
    ChordsManager *cm = new ChordsManager;
    cm->setChords(chordsList_);
    Chord c = cm->addNewChord();

    if(c.name.isEmpty()){
        return;
    }

    currentChords.push_back(c);

    addChord(c.name,c.fingers);
    resizeLayout();
}

void Tab::addChord(QString name, QString fingers)
{
    if(chords.contains(name)){

    }else{

        if(!fingers.isEmpty()){
            //TODO save if not already exist
        }

        if(fingers.isEmpty()){
            fingers = chordsList_->getFingers(instrument_, name);
            if(fingers.isEmpty()){
                fingers = "X X X X X X";
            }
        }

        Chord c(name,fingers);
        currentChords << c;

        chords << name;
        Guitar *guitar = new Guitar(name,fingers);
        guitar->setChordSize( optionsValues.chordSize );
        if(v1->count() > v2->count())
            v2->addWidget(guitar);
        else
            v1->addWidget(guitar);
        connect(guitar,SIGNAL(closeAndDelete()),this,SLOT(deleteGuitar()));
        connect(this,SIGNAL(removeChord(QString)),guitar,SLOT(remove(QString)));
        connect(this,SIGNAL(setSelected(QString)),guitar,SLOT(setSelected(QString)));
        connect(this,SIGNAL(setChordSize(QSize)),guitar,SLOT(setChordSize(QSize)));
    }

}

void Tab::read()
{
    QStringList list = highlighter->getList(edit->toPlainText());
    list.sort();
    QList<MyQCheckBox*> boxList;

    //TODO, move in a separate file
    QDialog *diag = new QDialog(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    diag->setLayout(vLayout);

    QFrame *frame = new QFrame;
    vLayout->addWidget(frame);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);

    QVBoxLayout *buttonLayout = new QVBoxLayout(frame);

    QPushButton *selectAll = new QPushButton(tr("Select all"));
    selectAll->setDefault(true);
    QPushButton *unselectAll = new QPushButton(tr("Unselect all"));

    foreach(QString item, list){
        MyQCheckBox *box = new MyQCheckBox(item);
        box->setChecked(chords.contains(item));

        connect(selectAll,SIGNAL(clicked()),box,SLOT(check()));
        connect(unselectAll,SIGNAL(clicked()),box,SLOT(uncheck()));

        buttonLayout->addWidget(box);
        boxList.push_back(box);
    }

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    buttonLayout->addWidget(line);

    bool found = false;
    for(int i=0;i<chords.size();i++){
        if(!list.contains(chords.at(i))){
            MyQCheckBox *box = new MyQCheckBox(chords.at(i));
            box->setChecked(true);
            buttonLayout->addWidget(box);
            boxList.push_back(box);
            found = true;
            list.push_back( chords.at(i) );

            connect(selectAll,SIGNAL(clicked()),box,SLOT(check()));
            connect(unselectAll,SIGNAL(clicked()),box,SLOT(uncheck()));
        }
    }
    line->setVisible(found);

    if(boxList.isEmpty()){
        QMessageBox::information(this,tr("Information"),tr("No chord found"));
        return;
    }

    vLayout->addWidget(selectAll);
    vLayout->addWidget(unselectAll);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,SIGNAL(accepted()),diag,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),diag,SLOT(reject()));
    vLayout->addWidget(buttonBox);


    if(diag->exec()){
        for(int i=0;i<list.size();i++){
            QString name = list.at(i);
            if(boxList.at(i)->isChecked())
                addChord(name);
            else
                emit removeChord(name);
        }
        resizeLayout();
    }
}

void Tab::selectAll() { edit->selectAll(); }
void Tab::undo() { edit->undo(); }
void Tab::redo() { edit->redo(); }
void Tab::cut() { edit->cut(); }
void Tab::copy() { edit->copy(); }
void Tab::paste() { edit->paste(); }
void Tab::setUndoAvailable(bool state) { undoAvailable_ = state; emit this->undoAvailable(state); }
void Tab::setRedoAvailable(bool state) { redoAvailable_ = state; emit this->redoAvailable(state);}

void Tab::setColors(QList<ColorRegExp> list)
{
    highlighter->clear();
    foreach(ColorRegExp colorRegExp, list){
        highlighter->addRule(colorRegExp.regExp,colorRegExp.color,colorRegExp.weight,colorRegExp.italic,colorRegExp.isText,colorRegExp.caseSensitivity==0?Qt::CaseInsensitive:Qt::CaseSensitive);
    }
    highlighter->update();
}

void Tab::setOptions(OptionsValues options)
{
    optionsValues = options;

    setColors(options.colors);

    //edit->setReadOnly(options.openReadOnly);
    //setEditable(!options.openReadOnly);
    edit->setFont(options.font);

    emit setChordSize(optionsValues.chordSize);

    resizeLayout();

    updateView();
}

void Tab::print(QPrinter *_printer)
{
    _printer->setDocName( modified_info.createFileName() );

    QPainter painter(_printer);
    painter.setRenderHints(QPainter::Antialiasing |
    QPainter::TextAntialiasing |
    QPainter::SmoothPixmapTransform, true);

    QRect paperRect = _printer->paperRect();
    QRect pageRect = _printer->pageRect();

    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::black);

    /*int leftMargin   = _printer->paperRect().left()   -  _printer->pageRect().left();
    int rightMargin  = _printer->paperRect().right()  -  _printer->pageRect().right();
    int topMargin    = _printer->paperRect().top()    -  _printer->pageRect().top();
    int bottomMargin = _printer->paperRect().bottom() -  _printer->pageRect().bottom();*/

    painter.resetTransform();
    painter.translate(paperRect.left(),paperRect.top());

    int pageHeight = pageRect.height();
    int pageWidth = pageRect.width();


    QSize chordSize(75,100);

    QTextDocument *doc;
#if 0
    doc = new QTextDocument;
    doc->setPlainText( edit->toPlainText() );
    doc->setDefaultFont( optionsValues.font );
#else
    doc = edit->document()->clone(this);
#endif

    if(!optionsValues.enableColorsOnPrinting){
        QTextOption opt;
        opt.setFlags(QTextOption::SuppressColors);
        doc->setDefaultTextOption(opt);
    }

    Highlighter *h = new Highlighter(doc);
    h->setRules( highlighter->getRules() );
    h->addPersonalRegExp( highlighter->getPersonalRegExp() );
    h->rehighlight();

    //TODO (remove space for chords)
    doc->setTextWidth(pageWidth);

    int currentRow = 0;
    int previousH = 0;

    //Compute header height
    int headersHeight = 0;
    if(!modified_info.title.isEmpty()) headersHeight+=20;
    if(!modified_info.artist.isEmpty()) headersHeight+=20;
    /*if(!modified_info.album.isEmpty()) headersHeight+=20; */
    if(modified_info.capo>0 || modified_info.tuning!="EADGBE") headersHeight+=20;
    headersHeight+=35;

    painter.setFont( optionsValues.font );

    //Compute text height
    int textHeight = 0;
    QList<int> textBlocksHeight;
    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    for(QTextBlock line = doc->begin(); line!= doc->end(); line = line.next()){
        QRectF lineRect = layout->blockBoundingRect(line);
        //QRect lineRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::TextWordWrap, line.text().isEmpty()?"|":line.text()  );
        textBlocksHeight << lineRect.height();
        textHeight += lineRect.height();
    }

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont( font );

    //Compute chords height
    int chordsHeight = 0;
    QList<int>chordBlocksHeight;
    for(int i=0;i<chords.size();i++){
        //Strings str(mapChord[chords[i]]);
        //str.setSize(chordSize);
        QRect chordRect = painter.boundingRect(0, 0, chordSize.width(), chordSize.width(), Qt::AlignLeft, chords.at(i)  );
        chordBlocksHeight << chordSize.height() + 10 + chordRect.height();
        chordsHeight += chordSize.height() + 10 + chordRect.height();
    }

    //TODO, make an option
    bool printHeadersOnAllPages = optionsValues.printHearderOnEachPages;

    int nbPages = 0;
    if(textHeight<pageHeight-headersHeight){
        nbPages = 1;
    }else{
        nbPages++;
        int temp = textHeight - (pageHeight-headersHeight);
        while(temp>0){
            if(printHeadersOnAllPages)
                temp -= pageHeight - headersHeight;
            else
                temp -= pageHeight;
            nbPages++;
        }
    }

    int currentChordPrinted = 0;

    for(int page=0;page<nbPages;page++){       
        if(page>0)
            _printer->newPage();
        painter.save();

        int maxHeight;

        //Print header
        if(page==0 || printHeadersOnAllPages){
            maxHeight = pageHeight - headersHeight;

            QFont font = painter.font();
            font.setFamily("Arial");
            font.setPointSize(12);
            font.setBold(true);
            painter.setFont(font);
            if(!modified_info.title.isEmpty()){
                painter.translate(0,20);
                painter.drawText(QPointF(0,0),modified_info.title);
            }
            font.setBold(false);
            painter.setFont(font);
            if(!modified_info.artist.isEmpty()){
                painter.translate(0,20);
                painter.drawText(QPointF(0,0),modified_info.artist);
            }
            /*if(!modified_info.album.isEmpty()){
                painter.drawText(QPointF(0,0),modified_info.album);
                painter.translate(0,20);
            }*/
            font.setPointSize(8);
            painter.setFont(font);
            if(modified_info.capo>0 && modified_info.tuning!="EADGBE"){
                painter.translate(0,20);
                painter.drawText(QPointF(0,0),QString(tr("[Capo: %1, Tuning: %2]")).arg(QString::number(modified_info.capo),modified_info.tuning));
            }else if(modified_info.capo>0){
                painter.translate(0,20);
                painter.drawText(QPointF(0,0),QString("[Capo: %1]").arg(QString::number(modified_info.capo)));
            }else if(modified_info.tuning!="EADGBE") {
                painter.translate(0,20);
                painter.drawText(QPointF(0,0),QString("[Tuning: %1]").arg(modified_info.tuning));
            }
            painter.translate(0,15);
            painter.drawLine(QPointF(0,0),QPointF(pageWidth,0));
            painter.translate(0,20);

        }else{
            maxHeight = pageHeight;
        }

        //Print page number
        if(nbPages>1){
            painter.save();

            QFont font = painter.font();
            font.setPointSize(8);
            font.setItalic(true);
            painter.setFont(font);
            QString pageNumberText =  tr("Page %1 of %2").arg(QString::number(page+1)).arg(QString::number(nbPages));
            QRect rect = painter.boundingRect(0, 0, pageWidth, pageHeight, Qt::TextWordWrap, pageNumberText  );

            //If only on  header, put page number on bottom, else on the top
            if(printHeadersOnAllPages){
                painter.translate(pageWidth-rect.width(),-30);
            }else{
                painter.resetTransform();
                painter.translate(pageWidth/2-rect.width()/2,pageHeight-5);
            }

            painter.drawText(QPointF(0,0),pageNumberText);
            painter.restore();
        }


        //Print chords
        if(page==nbPages-1){

            int hSumChords = 0;
            for(int i=currentChordPrinted;i<chords.size();i++){
                hSumChords += chordBlocksHeight.at(i);
            }

            int nbColumns = 1;

            if(hSumChords>pageHeight){
                nbColumns = 2;
            }

            painter.save();
            painter.translate(pageWidth-chordSize.width(),0);

            if(nbColumns>1){
                painter.translate(-(nbColumns-1)*(chordSize.width()+10),0);
            }

            int currentColumn = 1;

            hSumChords = 0;
            for(int i=currentChordPrinted;i<chords.size();i++){

                hSumChords += chordBlocksHeight.at(i);

                if(hSumChords>(maxHeight)){

                    if(currentColumn<nbColumns){
                        currentColumn++;
                        painter.restore();
                        painter.save();
                        painter.translate(pageWidth-chordSize.width(),0);

                    }else{
                        break;
                    }

                }

                Strings str(currentChords[i].fingers);

                str.setSize(chordSize);

                QRect chordRect = painter.boundingRect(0, 0, str.width(), chordSize.width(), Qt::AlignLeft, chords.at(i)  );
                painter.drawText(chordRect,Qt::AlignHCenter,chords.at(i));
                painter.translate(0,chordRect.height());


                str.paint(&painter,true);

                painter.translate(0,chordSize.height() + 10);

                currentChordPrinted++;
            }

            painter.restore();

        }else{

            painter.save();
            painter.translate(pageWidth-chordSize.width(),0);

            int hSumChords = 0;
            for(int i=currentChordPrinted;i<chords.size();i++){

                hSumChords += chordBlocksHeight.at(i);

                if(hSumChords>(maxHeight)){
                    break;
                }else{
                    Strings str(currentChords[i].fingers);

                    str.setSize(chordSize);

                    QRect chordRect = painter.boundingRect(0, 0, str.width(), chordSize.width(), Qt::AlignLeft, chords.at(i)  );
                    painter.drawText(chordRect,Qt::AlignHCenter,chords.at(i));
                    painter.translate(0,chordRect.height());


                    str.paint(&painter,true);

                    painter.translate(0,str.height()/2 + 10);

                    currentChordPrinted++;
                }
            }
            painter.restore();
        }


        painter.setFont( optionsValues.font );

        int h = 0;
        for(int i=currentRow;i<textBlocksHeight.size();i++){
            if(h+textBlocksHeight.at(i) > maxHeight){
                break;
            }else{
                currentRow++;
                //painter.drawRect(QRectF(0,h,pageWidth,textBlocksHeight.at(i)));
                h += textBlocksHeight.at(i);
            }
        }
        if(h==0){
            painter.restore();
            break;
        }


        //Print text
        painter.translate(0,-previousH);
        QRectF rect(0,previousH,pageWidth,h+3);
        doc->drawContents(&painter,rect);
        previousH = h+3;

        painter.restore();
    }

    //painter.restore();
}

void Tab::setEditable(bool editable)
{
    editable_ = editable;
    if(editable_){
        edit->setVisible(true);
        printPreviewWidget->setVisible(false);
        allInfoWidget->setVisible(true);
        tabToolBar->setVisible(true);
    }else{
        edit->setVisible(false);
        allInfoWidget->setVisible(false);
        tabToolBar->setVisible(false);
        printPreviewWidget->setVisible(true);

        printPreviewWidget->setCurrentPage(1);
        printPreviewWidget->updatePreview();
    }
}

void Tab::updateView()
{
    printPreviewWidget->updatePreview();
}

void Tab::setExpertMode(bool state)
{
    if(state){
        allInfoWidget->setVisible(false);
    }else{
        allInfoWidget->setVisible(true);
    }
}

void Tab::import()
{
    QString selectedText = edit->textCursor().selectedText();

    QDialog *diag = new QDialog(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    diag->setLayout(vLayout);

    QLabel *label = new QLabel(tr("Insert text"));
    vLayout->addWidget(label);

    QTextEdit *inputText = new QTextEdit;
    inputText->setText(selectedText);
    vLayout->addWidget(inputText);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,SIGNAL(accepted()),diag,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),diag,SLOT(reject()));
    vLayout->addWidget(buttonBox);

    if(diag->exec()){
        addChordsFromText(inputText->toPlainText());
    }
}

void Tab::importFromXTA()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Import from file"),optionsValues.defaultPath,tr("XTA files (*.xta)"));
    if(filepath.isEmpty())
        return;
    XTA xta(this);
    XTAinfo imported_info = xta.parse(filepath);
    addChordsFromText(imported_info.text);
}

void Tab::insertTab()
{
    bool ok;
    int length = QInputDialog::getInt(this,tr("Insert length"),tr("Length:"),20,5,100,1,&ok);
    if(!ok) return;

    QStringList keys;
    QString tunning = modified_info.tuning;
    tunning.replace("A",",A");
    tunning.replace("B",",B");
    tunning.replace("C",",C");
    tunning.replace("D",",D");
    tunning.replace("E",",E");
    tunning.replace("F",",F");
    tunning.replace("G",",G");
    keys = tunning.split(",",QString::SkipEmptyParts);

    int maxLength = 0;
    for(int i=0;i<keys.size();i++)
        if(keys.at(i).size()>maxLength)
            maxLength = keys.at(i).size();

    QString text = "\n";
    for(int i=0;i<keys.size();i++){
        text += QString("%1").arg(keys.at(i),maxLength,QChar(' ')) + "|";
        for(int j=0;j<length;j++)
            text += "-";
        text += "|\n";
    }

    QString ins;
    ins.fill('-',length);

    QTextCursor initial_cursor = edit->textCursor();

    bool newTab = true;

    int position =  initial_cursor.positionInBlock();
    QString line = initial_cursor.block().text();

    if(position>0 && position<line.length()){
        if((line[position-1]=='-' || line[position-1]=='|') &&
            (line[position]=='-' || line[position]=='|'))
            newTab = false;
    }

    if(newTab){
        QTextCursor cursor = edit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);
        edit->setTextCursor(cursor);
        edit->insertPlainText(text);
    }else{
        for(int i=0;i<keys.size();i++){

            edit->insertPlainText(ins);
            QTextCursor cursor = edit->textCursor();
            cursor.movePosition(QTextCursor::NextBlock);
            cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,position);
            edit->setTextCursor(cursor);
        }
    }

    edit->setTextCursor(initial_cursor);

}

void Tab::insertImage()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Load image"),QDir::homePath(),tr("Image (*.png)"));
    if(path.isEmpty()) return;

    QImage img(path);
    addImage(img);
}

void Tab::insertClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    QImage img = clipboard->image();
    if(img.isNull()) return;

    addImage(img);
}

void Tab::addImage(QImage &img)
{
    int index = modified_info.images.size();
    QString url = QString("mydata://image%1.png").arg(index);
    modified_info.images << img;

    edit->document()->addResource(QTextDocument::ImageResource,
                                QUrl(url), QVariant(img));

    QTextImageFormat imgTextFormat;
    imgTextFormat.setName(url);

    QTextCursor cursor = edit->textCursor();
    cursor.insertText(QString(QChar::ObjectReplacementCharacter), imgTextFormat);
    edit->setTextCursor(cursor);
}

void Tab::translateFrEn()
{
    QMessageBox::critical(this,"Error","Not available, coming soon");
}

void Tab::translateEnFr()
{
    QMessageBox::critical(this,"Error","Not available, coming soon");
}


void Tab::setAction(QAction *action)
{
    menuAction = action;
}
