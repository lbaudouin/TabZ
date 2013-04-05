#include "tab.h"

Tab::Tab(XTAinfo xta, QWidget *parent) : info(xta), modified_info(xta), undoAvailable_(false), redoAvailable_(false), editable_(true),
    QWidget(parent)
{
    //TODO, move it
    mapChord.insert("A","0 0 2 2 1 0");
    mapChord.insert("B","2 2 4 4 4 2");
    mapChord.insert("C","0 3 2 0 1 0");
    mapChord.insert("D","0 0 0 2 3 2");
    mapChord.insert("E","0 2 2 1 0 0");
    mapChord.insert("F","1 3 3 2 1 1");
    mapChord.insert("G","3 2 0 0 0 3");
    mapChord.insert("Am","0 0 2 2 1 0");
    mapChord.insert("Bm","2 2 4 4 3 2");
    mapChord.insert("Cm","X 3 5 5 4 3");
    mapChord.insert("Dm","0 0 0 2 3 1");
    mapChord.insert("Em","0 2 2 0 0 0");
    mapChord.insert("Fm","1 3 3 1 1 1");
    mapChord.insert("Gm","3 5 5 3 3 3");
    mapChord.insert("A#","X 1 3 3 3 1");
    mapChord.insert("C#","X 4 6 6 6 4");
    mapChord.insert("D#","X 6 8 8 8 6");
    mapChord.insert("F#","2 4 4 3 2 2");
    mapChord.insert("G#","4 6 6 5 4 4");
    mapChord.insert("Bb","X 1 3 3 3 1");
    mapChord.insert("Db","X 4 6 6 6 4");
    mapChord.insert("Eb","X 6 8 8 8 6");
    mapChord.insert("Gb","2 4 4 3 2 2");
    mapChord.insert("Ab","4 6 6 5 4 4");
    mapChord.insert("A#m","X 1 3 3 2 1");
    mapChord.insert("C#m","X 4 6 6 5 4");
    mapChord.insert("D#m","X 6 8 8 7 6");
    mapChord.insert("F#m","2 4 4 2 2 2");
    mapChord.insert("G#m","4 6 6 4 4 4");


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


    printer = new QPrinter;
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOrientation(QPrinter::Portrait);
    printer->setPaperSize(QPrinter::A4);
    printer->setPrintRange(QPrinter::PageRange);
    printer->setFullPage(true);

    printPreviewWidget = new QPrintPreviewDialog(printer);
    //printPreviewWidget->setViewMode(QPrintPreviewWidget::FacingPagesView);
    //printPreviewWidget->setViewMode(QPrintPreviewWidget::AllPagesView);
    //printPreviewWidget->setZoomMode(QPrintPreviewWidget::FitInView);


    connect(printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));

    previewLayout = new QVBoxLayout;
    previewLayout->addWidget(printPreviewWidget);

    edit = new QTextEdit;


    QFont editFont = edit->font();
    editFont.setFamily("DejaVu Sans Mono");
    //editFont.setFamily("Lucida Console");
    //editFont.setFixedPitch(true);

    edit->setFont(editFont);
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //edit->document()->setPageSize(QSizeF(10,10));

    //Highlighter *highlighter = new Highlighter(edit->document());
    highlighter = new Highlighter(edit->document());

    chordLayout = new QVBoxLayout;

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

    chordLayout->addLayout(buttonLayout);

    connect(buttonAdd,SIGNAL(clicked()),this,SLOT(addNewChord()));
    connect(buttonImport,SIGNAL(clicked()),this,SLOT(importFromXTA()));
    connect(buttonCopy,SIGNAL(clicked()),this,SLOT(import()));
    connect(buttonRead,SIGNAL(clicked()),this,SLOT(read()));



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

#if 1
    QHBoxLayout *hlayout = new QHBoxLayout;
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
    connect(editTitle,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(editArtist,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(editAlbum,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(editTuning,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    connect(editCapo,SIGNAL(valueChanged(int)),this,SLOT(capoChanged(int)));

    edit->setText(info.text);

    printPreviewWidget->setVisible(false);

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

        addChord(name,fingers);
    }
    resizeLayout();
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
        labelInfo->setText( tr("(No title)"));
    }else{
        labelInfo->setText( QString(tr("%1 - %2","artist - title")).arg(modified_info.artist,modified_info.title));
    }

    if(modified_info.capo>0 && modified_info.tuning!="EADGBE"){
        specialInfo->setText(QString(tr("\t [Capo: %1, Tuning: %2]")).arg(QString::number(modified_info.capo),modified_info.tuning));
    }else if(modified_info.capo>0){
        specialInfo->setText(QString("\t [Capo: %1]").arg(QString::number(modified_info.capo)));
    }else if(modified_info.tuning!="EADGBE") {
        specialInfo->setText(QString("\t [Tuning: %1]").arg(modified_info.tuning));
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

void Tab::textChanged(QString)
{
   //TODO, separate in several function
   modified_info.text = edit->toPlainText();
   modified_info.title = editTitle->text();
   modified_info.artist = editArtist->text();
   modified_info.album = editAlbum->text();
   modified_info.tuning = editTuning->text();

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

void Tab::saved()
{
    info = modified_info;
}

XTAinfo Tab::getXTA()
{
    modified_info.chords.clear();
    for(int i=0;i<chords.size();i++){
        modified_info.chords += chords.at(i);
        if(mapChord.contains(chords.at(i))){
            modified_info.chords += " " + mapChord[chords.at(i)];
        }
        modified_info.chords += "\n";
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
    chords.removeAll(g->getName());
    v1->removeWidget(g);
    v2->removeWidget(g);
    g->close();
    resizeLayout();
}

void Tab::resizeLayout()
{
    //qDebug() << QSize(v1->count(),v2->count());

    if(v1->count()>0 && v2->count()>0){
        scrollArea->setMinimumWidth(400);
    }else{
        if(v1->count()>0 || v2->count()>0){
            scrollArea->setMinimumWidth(200);
        }else{
            scrollArea->setMinimumWidth(0);
        }
    }
    scrollArea->update();
}

void Tab::addNewChord()
{
    QDialog *diag = new QDialog(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    diag->setLayout(vLayout);

    QComboBox *instrumentCombo = new QComboBox;

    QLineEdit *nameEdit = new QLineEdit;
    QLineEdit *fingersEdit = new QLineEdit;
    QComboBox *fingersCombo = new QComboBox;
    fingersCombo->setEditable(true);
    fingersCombo->addItems( mapChord.values() );
    fingersCombo->setCurrentIndex(-1);


    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Name"),nameEdit);
    formLayout->addRow(tr("Instrument"),instrumentCombo);
    formLayout->addRow(tr("Fingers"),fingersCombo);
    vLayout->addLayout(formLayout);

    Strings *strings = new Strings("");
    vLayout->addWidget(strings,0,Qt::AlignHCenter);

    connect(fingersCombo,SIGNAL(editTextChanged(QString)),strings,SLOT(setFingers(QString)));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,SIGNAL(accepted()),diag,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),diag,SLOT(reject()));
    vLayout->addWidget(buttonBox);

    if(diag->exec()){
        QString name = nameEdit->text();
        if(name.isEmpty())
            name = QInputDialog::getText(this,tr("Note name"),tr("Note:"));
        if(name.isEmpty()) return;

        QString fingers = fingersEdit->text();

        fingers.replace(","," ");
        fingers.replace("\t"," ");

        addChord(name,fingers);
        resizeLayout();
    }
}

void Tab::addChord(QString name, QString fingers)
{
    if(chords.contains(name)){

    }else{

        if(!fingers.isEmpty())
            mapChord.insert(name,fingers);

        if(fingers.isEmpty()){
            if(mapChord.contains(name)){
                fingers = mapChord[name];
            }else{
                fingers = "X X X X X X";
            }
        }

        chords << name;
        Guitar *guitar = new Guitar(name,fingers);
        if(v1->count() > v2->count())
            v2->addWidget(guitar);
        else
            v1->addWidget(guitar);
        connect(guitar,SIGNAL(closeAndDelete()),this,SLOT(deleteGuitar()));
        connect(this,SIGNAL(removeChord(QString)),guitar,SLOT(remove(QString)));
        connect(this,SIGNAL(setSelected(QString)),guitar,SLOT(setSelected(QString)));
    }

}

void Tab::read()
{
    QStringList list = highlighter->getList(edit->toPlainText());
    QList<QQCheckBox*> boxList;

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
        QQCheckBox *box = new QQCheckBox(item);
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
            QQCheckBox *box = new QQCheckBox(chords.at(i));
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

void Tab::enableColors(bool state)
{
    highlighter->setEnabled(state);
}

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
    highlighter->setEnabled(optionsValues.enableColors);

    setColors(options.colors);

    edit->setReadOnly(options.openReadOnly);
    edit->setFont(options.font);
}

void Tab::print(QPrinter *)
{
    //qDebug() << "void Tab::print(QPrinter *)";
    QPainter painter(printer);
    painter.setRenderHints(QPainter::Antialiasing |
    QPainter::TextAntialiasing |
    QPainter::SmoothPixmapTransform, true);

    int margin = 50;
    int pageHeight = painter.window().height() - 2 * margin;
    int pageWidth = painter.window().width() - 2 * margin;

    QTextDocument *doc = edit->document()->clone(this);
    doc->setTextWidth(pageWidth);
    Highlighter *h = new Highlighter(doc);
    h->setRules( highlighter->getRules() );
    h->rehighlight();

    //qDebug() << edit->document()->size();
    //qDebug() << pageWidth;
    //qDebug() << printer->paperSize(QPrinter::Millimeter);

    int currentRow = 0;
    int previousH = 0;

    painter.setFont( optionsValues.font );
    QList<int> hBlocks;
    for(QTextBlock line = doc->begin(); line!= doc->end(); line = line.next()){
        QRect titleRect = painter.boundingRect(0, 0, pageWidth, pageHeight, Qt::TextWordWrap, line.text().isEmpty()?" ":line.text()  );
        hBlocks << titleRect.height();
    }


    painter.setFont( QFont("Ubuntu",12) );

    for(int page=0;;page++){
        painter.save();
        painter.translate(margin,margin);


        int maxHeight = pageHeight;

        if(page==0){
            if(!modified_info.title.isEmpty()){
                painter.drawText(QPointF(0,0),modified_info.title);
                painter.translate(0,20);
                maxHeight-=20;
            }
            if(!modified_info.artist.isEmpty()){
                painter.drawText(QPointF(0,0),modified_info.artist);
                painter.translate(0,20);
                maxHeight-=20;
            }
            if(!modified_info.album.isEmpty()){
                painter.drawText(QPointF(0,0),modified_info.album);
                painter.translate(0,20);
                maxHeight-=20;
            }
            QFont font = painter.font();
            font.setPointSize(8);
            painter.setFont(font);
            if(modified_info.capo>0 && modified_info.tuning!="EADGBE"){
                painter.drawText(QPointF(0,0),QString(tr("[Capo: %1, Tuning: %2]")).arg(QString::number(modified_info.capo),modified_info.tuning));
                painter.translate(0,20);
                maxHeight-=20;
            }else if(modified_info.capo>0){
                painter.drawText(QPointF(0,0),QString("[Capo: %1]").arg(QString::number(modified_info.capo)));
                painter.translate(0,20);
                maxHeight-=20;
            }else if(modified_info.tuning!="EADGBE") {
                painter.drawText(QPointF(0,0),QString("[Tuning: %1]").arg(modified_info.tuning));
                painter.translate(0,20);
                maxHeight-=20;
            }
            painter.drawLine(QPointF(0,0),QPointF(pageWidth,0));
            painter.translate(0,40);
            maxHeight-=40;
        }

        painter.setFont( optionsValues.font );

        int h = 0;
        for(int i=currentRow;i<hBlocks.size();i++){
            h += hBlocks.at(i);
            if(h>maxHeight){
                h -= hBlocks.at(i);
                break;
            }else{
                currentRow++;
            }
        }
        if(h==0)
            break;
        if(page>0)
            printer->newPage();

        painter.translate(0,-previousH);
        QRectF rect(0,previousH,pageWidth,h);

        doc->drawContents(&painter,rect);
        previousH = h;
        painter.restore();
    }

    painter.restore();
}

void Tab::setEditable(bool editable)
{
    editable_ = editable;
    if(editable_){
        edit->setVisible(true);
        printPreviewWidget->setVisible(false);
    }else{
        edit->setVisible(false);

        //TODO, find an other way to create preview (custom QPrintPreviewWidget)
        delete printPreviewWidget;
        printPreviewWidget = new QPrintPreviewDialog(printer);
        connect(printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
        previewLayout->addWidget(printPreviewWidget);
        printPreviewWidget->setVisible(true);
        //printPreviewWidget->updatePreview();
        printPreviewWidget->repaint();
        //printPreviewWidget->
        printPreviewWidget->update();
    }
}

void Tab::updateView()
{
    edit->setVisible(false);
    //edit->document()->setPageSize(QSizeF(1000,1000));
    //printPreviewWidget->updatePreview();
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
