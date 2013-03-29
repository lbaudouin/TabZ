#include "tab.h"

Tab::Tab(XTAinfo xta, QWidget *parent) : info(xta), modified_info(xta),
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


    QHBoxLayout *infoLayout = new QHBoxLayout;
    mainLayout->addLayout(infoLayout);

    QVBoxLayout *i1 = new QVBoxLayout;
    QVBoxLayout *i2 = new QVBoxLayout;

    labelInfo = new QLabel("Info");
    labelInfo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    i1->addWidget(labelInfo);


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
    editCapo->setMinimum(0);
    editCapo->setMaximum(30);
    formLayout->addRow(tr("Capo:"),editCapo);

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
    connect(hideShow,SIGNAL(toggled(bool)),infoWidget,SLOT(setVisible(bool)));


    edit = new QTextEdit;
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //Highlighter *highlighter = new Highlighter(edit->document());
    highlighter = new Highlighter(edit->document());

    chordLayout = new QVBoxLayout;

    //Buttons
    QToolButton *buttonAdd = new QToolButton();
    buttonAdd->setIcon( QIcon(":images/add") );
    //QToolButton *buttonResize = new QToolButton();
    QToolButton *buttonRead = new QToolButton();
    buttonRead->setIcon( QIcon(":images/search") );
    //buttonAdd->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    //buttonResize->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    //buttonRead->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

    //QSpacerItem *buttonSpacer = new QSpacerItem(10,10,QSizePolicy::Preferred,QSizePolicy::Ignored);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonAdd);
    //buttonLayout->addWidget(buttonResize);
    buttonLayout->addWidget(buttonRead);
    //buttonLayout->addSpacerItem(buttonSpacer);

    chordLayout->addLayout(buttonLayout);

    connect(buttonAdd,SIGNAL(clicked()),this,SLOT(addNewChord()));
    //connect(buttonResize,SIGNAL(clicked()),this,SLOT(resizeLayout()));
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

    if(!info.chords.isEmpty()){


        QTextStream stream(&info.chords);

        while(!stream.atEnd()){
            QString line = stream.readLine();
            QStringList temp = line.split(" ",QString::SkipEmptyParts);
            if(temp.size()==0)
                continue;
            QString name = temp.at(0);
            QString fingers = "";
            for(int i=1;i<temp.size();i++)
                fingers += " " + temp.at(i);
            fingers.trimmed();

            addChord(name,fingers);
        }
        resizeLayout();
    }

    labelInfo->setText(tr("%1 - %2 %3").arg(info.artist,info.title,
                                                 (info.capo>0 && info.tuning!="EADGBE")?QString(tr("\t [Capo: %1, Tuning: %2]")).arg(QString::number(info.capo),info.tuning):
                                                 (info.capo>0)?QString("\t [Capo: %1]").arg(QString::number(info.capo)):
                                                 (info.tuning!="EADGBE")?QString("\t [Tuning: %1]").arg(info.tuning):""));
}

void Tab::capoChanged(int)
{
    modified_info.capo = editCapo->value();

    if(!info.isEqual(modified_info))
     emit setSaveIcon(-1,edit->toPlainText()!=info.text);
}

void Tab::textChanged(QString)
{
   modified_info.text = edit->toPlainText();
   modified_info.title = editTitle->text();
   modified_info.artist = editArtist->text();
   modified_info.album = editAlbum->text();
   modified_info.tuning = editTuning->text();

   if(!info.isEqual(modified_info)){
    emit setSaveIcon(-1,true);
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
    return modified_info;
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
    QString name = QInputDialog::getText(this,tr("Note name"),tr("Note:"));
    if(name.isEmpty()) return;
    QString fingers = QInputDialog::getText(this,tr("Note fingers"),tr("Fingers:"));
    if(fingers.isEmpty()) return;

    fingers.replace(","," ");

    addChord(name,fingers);
    resizeLayout();
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
    }

}

void Tab::read()
{
    QStringList list = highlighter->getList(edit->toPlainText());

    foreach(QString name, list){
        name.remove("\\b");
        name.remove("(?!#)");
        name.remove("(?!(#|')");
        name.remove("(?|m)");
        addChord(name);
    }
    resizeLayout();
}

void Tab::selectAll() { edit->selectAll(); }
void Tab::undo() { edit->undo(); }
void Tab::redo() { edit->redo(); }
void Tab::cut() { edit->cut(); }
void Tab::copy() { edit->copy(); }
void Tab::paste() { edit->paste(); }
