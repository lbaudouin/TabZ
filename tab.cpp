#include "tab.h"

Tab::Tab(XTAinfo xta, Chords* chordsList, OptionsValues optionsValues, QWidget *parent) : QWidget(parent), info(xta), modified_info(xta), chordsList_(chordsList), undoAvailable_(false), redoAvailable_(false), editable_(true), instrument_("Guitar","guitar",6)
{   
    optionsValues_ = optionsValues;

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
    comboInstrument = new QComboBox;
    formLayout->addRow(tr("Instrument:"),comboInstrument);

    QList<Instrument> instruments = chordsList_->getInstruments();
    for(int i=0;i<instruments.size();i++){
        comboInstrument->addItem(instruments[i].name, instruments[i].label);
    }


    editCapo = new QSpinBox();
    QSpacerItem *spacerSpinBox = new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Ignored);
    QHBoxLayout *layoutSpinBox = new QHBoxLayout;
    editCapo->setMinimum(0);
    editCapo->setMaximum(30);
    layoutSpinBox->addWidget(editCapo);
    layoutSpinBox->addSpacerItem(spacerSpinBox);
    formLayout->addRow(tr("Capo:"),layoutSpinBox);

    OpenEditLayout *leditMp3 = new OpenEditLayout(OpenEditLayout::FileMode);
    leditMp3->setFilter(tr("Music files (*.mp3 *.ogg *.wav *.flac)"));
    formLayout->addRow(tr("Mp3 file:"),leditMp3);
    editMp3 = leditMp3->getEditWidget();
    OpenEditLayout *leditGP = new OpenEditLayout(OpenEditLayout::FileMode);
    leditGP->setFilter(tr("GuitarPro files (*.gp *.gp3 *.gp4 *.gp5 *.gp6 *.gpx *.tg)"));
    formLayout->addRow(tr("GuitarPro file:"),leditGP);
    editGP = leditGP->getEditWidget();

    infoWidget->setLayout(formLayout);
    infoWidget->setVisible(false);

    i1->addWidget(infoWidget);

    hideShow = new QToolButton;
    hideShow->setCheckable(true);
    QIcon icon;
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarUnshadeButton).pixmap(16), QIcon::Normal, QIcon::Off );
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton).pixmap(16), QIcon::Normal, QIcon::On );
    hideShow->setIcon(icon);
    i2->addWidget(hideShow);

    infoLayout->addLayout(i1);
    infoLayout->addLayout(i2);

    connect(hideShow,SIGNAL(toggled(bool)),labelInfo,SLOT(setHidden(bool)));
    connect(hideShow,SIGNAL(toggled(bool)),specialInfo,SLOT(setHidden(bool)));
    connect(hideShow,SIGNAL(toggled(bool)),infoWidget,SLOT(setVisible(bool)));
    connect(hideShow,SIGNAL(clicked()),this,SLOT(updateTitle()));

    printPreviewWidget = new QPrintPreview(/*printer*/);
    printer = printPreviewWidget->getPrinter();
    printer->setPageMargins(optionsValues_.topMargin,optionsValues_.leftMargin,
                            optionsValues_.rightMargin,optionsValues_.bottomMargin,QPrinter::Millimeter);

    printer->setDocName( info.createFileName() );
#if !defined(__WIN32__)
    printer->setOutputFileName( optionsValues_.defaultOutputFolder + QDir::separator() + printer->docName() );
#endif
    printPreviewWidget->setViewMode(QPrintPreviewWidget::AllPagesView);
    printPreviewWidget->setZoomMode(QPrintPreviewWidget::FitInView);

    connect(printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));

    previewLayout = new QVBoxLayout;
    previewLayout->addWidget(printPreviewWidget);
    printPreviewWidget->setVisible(false);

    edit = new MyTextEdit;
    edit->setFont(optionsValues_.mainFont);

    connect(edit,SIGNAL(insertNewImage(QImage&)),this,SLOT(addImage(QImage&)));


    //QFont editFont = edit->font();
    //editFont.setFamily("DejaVu Sans Mono");
    //editFont.setFamily("Lucida Console");
    //editFont.setFixedPitch(true);
    //edit->setFont(editFont);

    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    highlighter = new Highlighter(edit->document());
    setColors(optionsValues_.colors);

    chordLayout = new QVBoxLayout;

    chordToolBar = new QToolBar;
    chordToolBar->setOrientation(Qt::Vertical);
    chordToolBar->setMovable(false);
    chordToolBar->addAction(QIcon(":images/add"), tr("Add new chord"), this, SLOT(addNewChord()) );
    chordToolBar->addAction(QIcon(":images/copy"), tr("Import from text"), this, SLOT(import()) );
    chordToolBar->addAction(QIcon(":images/import"), tr("Import from XTA"), this, SLOT(importFromXTA()) );
    chordToolBar->addAction(QIcon(":images/search"), tr("Manage"), this, SLOT(read()) );

    chordLayout->addWidget(chordToolBar);

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
    scrollArea->setVisible(false);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    scrollArea->setWidget(w2);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::Box);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMinimumWidth(0);

    chordLayout->addWidget(scrollArea);


    tabToolBar = new QToolBar;
    tabToolBar->setOrientation(Qt::Vertical);
    tabToolBar->setMovable(false);
    tabToolBar->addAction(QIcon(":images/insert-image"),tr("Insert image from file"), this, SLOT(insertImage()) );
    tabToolBar->addAction(QIcon(":images/insert-clipboard"),tr("Insert image from clipboard"), this, SLOT(insertClipboard()) );
    tabToolBar->addAction(QIcon(":images/tab"),tr("Insert ascii tab"), this, SLOT(insertTab()) );
    tabToolBar->addAction(QIcon(":images/lilypond"),tr("Insert lilypond tab"), this, SLOT(insertLilypond()) );
    tabToolBar->addSeparator();
    tabToolBar->addAction(QIcon(":images/La-A"),tr("French to English"), this, SLOT(translateFrEn()) )->setEnabled(false);
    tabToolBar->addAction(QIcon(":images/A-La"),tr("English to French"), this, SLOT(translateEnFr()) )->setEnabled(false);
    tabToolBar->addSeparator();
    tabToolBar->addAction(QIcon(":images/import"),tr("Import images"), this, SLOT(importImages()));
    tabToolBar->addAction(QIcon(":images/export"),tr("Export images"), this, SLOT(exportImages()));

#if 1
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(tabToolBar,Qt::AlignTrailing);
    hlayout->addWidget(edit);
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
    editMp3->setText( info.file_mp3 );
    editGP->setText( info.file_gp );
    connect(edit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(editTitle,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editArtist,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editAlbum,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editTuning,SIGNAL(textChanged(QString)),this,SLOT(infoChanged(QString)));
    connect(editCapo,SIGNAL(valueChanged(int)),this,SLOT(capoChanged(int)));

    QString instrumentName = chordsList_->getInstrumentName(info.instrument);
    QStringList allInstrument = chordsList_->getInstrumentsNames();

    comboInstrument->setCurrentIndex(allInstrument.indexOf(instrumentName));

    instrument_ = chordsList_->getInstrument(comboInstrument->currentText());

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
            edit->setTextCursor(cursor);
        }
    }

    edit->setTextInteractionFlags(Qt::TextEditorInteraction);
    edit->setAutoFormatting(QTextEdit::AutoNone);
    //edit->setAcceptRichText(false);


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

void Tab::chordsChanged(QString)
{
   modified_info.chords = getChords();

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
#if !defined(__WIN32__)
   printer->setOutputFileName( optionsValues_.defaultOutputFolder + QDir::separator() + printer->docName() );
#endif

   if(!info.isEqual(modified_info)){
       emit setSaveIcon(-1,true);
   }else{
       emit setSaveIcon(-1,false);
   }
}

bool Tab::isModified()
{
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

QString Tab::getChords()
{
    QString text;
    for(int i=0;i<currentChords.size();i++){
        text += currentChords.at(i).name;
        if(!currentChords[i].fingers.isEmpty()){
            text += " " + currentChords[i].fingers;
        }
        text += "\n";
    }
    return text;
}

XTAinfo Tab::getXTA()
{
    modified_info.file_mp3 = editMp3->text();
    modified_info.file_gp = editGP->text();

    modified_info.chords = getChords();

    QTextCursor cursor = edit->textCursor();
    cursor.setPosition(0);
    modified_info.refImages.clear();
    while(1){
        cursor = edit->document()->find(QString(QChar::ObjectReplacementCharacter),cursor);
        if(cursor.isNull()) break;
        bool ok;
        //qDebug() << cursor.charFormat().property(QTextFormat::ImageName).toString();
        int index = cursor.charFormat().property(QTextFormat::ImageName).toString().section("image",1).section(".png",0,0).toInt(&ok);
        if(ok){
            modified_info.refImages << index;
        }
    }

    modified_info.instrument = comboInstrument->itemData(comboInstrument->currentIndex()).toString();

    return modified_info;
}

QString Tab::getHtml()
{
    return edit->document()->toHtml();
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

    if(v1->count()==0 && v2->count()==0){
        chordToolBar->setOrientation(Qt::Vertical);
        scrollArea->setVisible(false);
    }

    resizeLayout();
}

void Tab::resizeLayout()
{
    if(v1->count()>0 && v2->count()>0){
        scrollArea->setMinimumWidth(2.0 * 1.4 * optionsValues_.chordSize.width());
    }else{
        if(v1->count()>0 || v2->count()>0){
            scrollArea->setMinimumWidth(1.4 * optionsValues_.chordSize.width());
        }else{
            scrollArea->setMinimumWidth(0);
        }
    }
    scrollArea->update();
}

void Tab::addNewChord()
{
    ChordsManager *cm = new ChordsManager(chordsList_);
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
        //TODO, display warning/error
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

        chordsChanged();

        Guitar *guitar = new Guitar(name,fingers);
        guitar->setChordSize( optionsValues_.chordSize );

        chordToolBar->setOrientation(Qt::Horizontal);
        scrollArea->setVisible(true);

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

    MyCheckableTree *tree = new MyCheckableTree;
    tree->setRootLabel(tr("All"));

    QDialog *diag = new QDialog(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    diag->setLayout(vLayout);

    QLabel *label = new QLabel(tr("Chords found:"));
    vLayout->addWidget(label);

    foreach(QString item, list){
        tree->addRow(item,item,chords.contains(item));
    }

    for(int i=0;i<chords.size();i++){
        if(!list.contains(chords.at(i))){
            tree->addRow(chords.at(i),chords.at(i),true);
        }
    }

    if(tree->nbChild()==0){
        QMessageBox::information(this,tr("Information"),tr("No chord found"));
        return;
    }

    vLayout->addWidget(tree);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,SIGNAL(accepted()),diag,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),diag,SLOT(reject()));
    vLayout->addWidget(buttonBox);


    if(diag->exec()){
        for(int i=0;i<tree->nbChild();i++){
            QString name = tree->getData(i).toString();
            if(tree->getState(i)==Qt::Checked)
                addChord(name);
            else{
                emit removeChord(name);
            }
        }

        resizeLayout();

        chordsChanged();
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
    optionsValues_ = options;

#if !defined(__WIN32__)
    printer->setOutputFileName( optionsValues_.defaultOutputFolder + QDir::separator() + printer->docName() );
#endif

    setColors(optionsValues_.colors);

    edit->setFont(optionsValues_.mainFont);

    printer->setPageMargins(optionsValues_.topMargin,optionsValues_.leftMargin,
                            optionsValues_.rightMargin,optionsValues_.bottomMargin,QPrinter::Millimeter);

    emit setChordSize(optionsValues_.chordSize);

    resizeLayout();

    updateView();
}

void Tab::print(QPrinter *_printer)
{
    //_printer->setDocName( modified_info.createFileName() );
    //_printer->setOutputFileName( optionsValues_.defaultOutputFolder + QDir::separator() + printer->docName() );

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
    doc->setDefaultFont( optionsValues_.font );
#else
    doc = edit->document()->clone(this);
#endif

    if(!optionsValues_.enableColorsOnPrinting){
        QTextOption opt;
        opt.setFlags(QTextOption::SuppressColors);
        doc->setDefaultTextOption(opt);
    }

    Highlighter *h = new Highlighter(doc);
    h->setRules( highlighter->getRules() );
    h->addPersonalRegExp( highlighter->getPersonalRegExp() );
    h->setDraftMode(false);
    h->rehighlight();

    //TODO (remove space for chords)
    doc->setTextWidth(pageWidth);

    int currentRow = 0;
    int previousH = 0;

    //Compute header height
    int headersHeight = 0;
    if(!modified_info.title.isEmpty()){
        QRect titleRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.title  );
        headersHeight+=titleRect.height();
    }
    if(!modified_info.artist.isEmpty()){
        QRect artistRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.artist  );
        headersHeight+=artistRect.height();
    }
    /*if(!modified_info.album.isEmpty()){
        QRect albumRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.album  );
        headersHeight+=albumRect.height();
    }*/
    if(modified_info.capo>0 || modified_info.tuning!="EADGBE"){
        QRect otherRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, tr("[Capo: %1, Tuning: %2]").arg(QString::number(modified_info.capo),modified_info.tuning)  );
        headersHeight+=otherRect.height();
    }
    headersHeight+=35;

    painter.setFont( optionsValues_.mainFont );

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
    bool printHeadersOnAllPages = optionsValues_.printHearderOnEachPages;

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

            if(!modified_info.title.isEmpty()){
                font = optionsValues_.titleFont;
                painter.setFont(font);
                QRect titleRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.title  );
                painter.translate(0,titleRect.height());
                painter.drawText(QPointF(0,0),modified_info.title);
            }

            if(!modified_info.artist.isEmpty()){
                font = optionsValues_.artistFont;
                painter.setFont(font);
                QRect artistRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.artist  );
                painter.translate(0,artistRect.height());
                painter.drawText(QPointF(0,0),modified_info.artist);
            }
            /*if(!modified_info.album.isEmpty()){
                font = optionsValues_.albumFont;
                painter.setFont(font);
                painter.drawText(QPointF(0,0),modified_info.album);
                QRect albumRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, modified_info.album  );
                painter.translate(0,albumRect.height());
            }*/

            if(modified_info.capo>0 || modified_info.tuning!="EADGBE"){
                font = optionsValues_.otherFont;
                painter.setFont(font);
                QString text;
                if(modified_info.capo>0 && modified_info.tuning!="EADGBE"){
                    text = tr("[Capo: %1, Tuning: %2]").arg(QString::number(modified_info.capo),modified_info.tuning);
                }else if(modified_info.capo>0){
                    text = tr("[Capo: %1]").arg(modified_info.capo);
                }else if(modified_info.tuning!="EADGBE") {
                    text = tr("[Tuning: %1]").arg(modified_info.tuning);
                }
                QRect otherRect = painter.boundingRect(0, 0, pageWidth, -1, Qt::AlignLeft, text );
                painter.translate(0,otherRect.height());
                painter.drawText(QPointF(0,0),text);
            }
            painter.translate(0,15);
            painter.setPen(QPen(Qt::black,1));
            painter.drawLine(QPointF(0,0),QPointF(pageWidth,0));
            painter.setPen(Qt::black);
            painter.translate(0,20);

        }else{
            maxHeight = pageHeight;
        }

        //Print page number
        if(nbPages>1){
            painter.save();

            QFont font = optionsValues_.mainFont;
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

                font = painter.font();
                font.setBold(true);
                painter.setFont(font);

                QRect chordRect = painter.boundingRect(0, 0, str.width(), chordSize.width(), Qt::AlignLeft, chords.at(i)  );
                painter.drawText(chordRect,Qt::AlignHCenter,chords.at(i));
                painter.translate(0,chordRect.height());

                font.setBold(false);
                painter.setFont(font);


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

                    font = painter.font();
                    font.setBold(true);
                    painter.setFont(font);

                    QRect chordRect = painter.boundingRect(0, 0, str.width(), chordSize.width(), Qt::AlignLeft, chords.at(i)  );
                    painter.drawText(chordRect,Qt::AlignHCenter,chords.at(i));
                    painter.translate(0,chordRect.height());

                    font.setBold(false);
                    painter.setFont(font);


                    str.paint(&painter,true);

                    painter.translate(0,chordSize.height() + 10);

                    currentChordPrinted++;
                }
            }
            painter.restore();
        }

        int h = 0;
        for(int i=currentRow;i<textBlocksHeight.size();i++){
            if(h+textBlocksHeight.at(i) > maxHeight){
                break;
            }else{
                currentRow++;
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
    if(editable_){
        if(state){
            allInfoWidget->setVisible(false);
        }else{
            allInfoWidget->setVisible(true);
        }
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
    XTAinfo imported_info = readXTA();
    addChordsFromText(imported_info.text);
}

XTAinfo Tab::readXTA(QString filepath)
{
    if(filepath.isEmpty()){
        filepath = QFileDialog::getOpenFileName(this,tr("Import from file"),optionsValues_.defaultPath,tr("XTA files (*.xta)"));
    }
    if(filepath.isEmpty())
        return XTAinfo();
    XTA *xta = new XTA();
    return xta->load(filepath);
}

void Tab::insertTab()
{
    bool ok;
    int length = QInputDialog::getInt(this,tr("Insert length"),tr("Length:"),50,5,100,1,&ok);
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

void Tab::insertLilypond()
{
#if defined(__WIN32__)
    QMessageBox *mess = new QMessageBox;
    mess->setIconPixmap( QPixmap(":images/ubuntu") );
    mess->setWindowTitle( tr("Ubuntu only function") );
    mess->setText(tr("This function is only available on Ubuntu.\nDownload and install Ubuntu and try again.\n\nA sample will be inserted..."));
    mess->addButton(tr("Download Ubuntu"),QMessageBox::RejectRole);
    mess->addButton(QMessageBox::Ok);
    if(!mess->exec()){
        QDesktopServices::openUrl(QUrl(tr("http://www.ubuntu.com/download/desktop")));
    }
    QImage img(":images/tab_sample");
    addImage(img);
#else
    Lilypond *lilypond = new Lilypond;

    while(!lilypond->isLilypondAvailable()){
        if(!lilypond->downloadLilypond()){
            lilypond->close();
            return;
        }
    }

    if(lilypond->exec()){
        QImage img = lilypond->getImage();
        addImage(img);
    }
#endif
}

void Tab::insertImage()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Load image"),QDir::homePath(),tr("Image (*.png *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
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

void Tab::importImages()
{
    XTAinfo imported_info = readXTA();
    for(int i=0;i<imported_info.images.size();i++){
        addImage( imported_info.images[i] );
    }
}

void Tab::exportImages()
{
    QTextCursor cursor = edit->textCursor();
    cursor.setPosition(0);
    modified_info.refImages.clear();

    QList<QImage> images;
    while(1){
        cursor = edit->document()->find(QString(QChar::ObjectReplacementCharacter),cursor);
        if(cursor.isNull()) break;
        bool ok;
        int index = cursor.charFormat().property(QTextFormat::ImageName).toString().section("image",1).section(".png",0,0).toInt(&ok);
        if(ok){
            images << modified_info.images[index];
        }
    }

    if(images.size()>0){
        QString folder = QFileDialog::getExistingDirectory(this,tr("Select output folder"),optionsValues_.defaultPath);
        if(folder.isEmpty()) return;

        for(int i=0;i<images.size();i++){
            QString filename = folder + QDir::separator() + QString("img%1.png").arg(i);
            images[i].save(filename);
        }
    }
}


void Tab::setAction(QAction *action)
{
    menuAction = action;
}

void Tab::toggleInfo()
{
    hideShow->toggle();
}
