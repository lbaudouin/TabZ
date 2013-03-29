#include "tab.h"

Tab::Tab(QWidget *parent) : info("",""),
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    //layout->setContentsMargins(11, 11, 11, 11);
    layout->setObjectName(QString::fromUtf8("layout"));


    labelInfo = new QLabel("Info");
    labelInfo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    layout->addWidget(labelInfo);

    edit = new QTextEdit;
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //Highlighter *highlighter = new Highlighter(edit->document());
    highlighter = new Highlighter(edit->document());

    mainLayout = new QVBoxLayout;

    //Buttons
    QPushButton *buttonAdd = new QPushButton(tr("A"));
    QPushButton *buttonResize = new QPushButton(tr("R"));
    QPushButton *buttonRead = new QPushButton(tr("Read"));
    //buttonAdd->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    //buttonResize->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
    //buttonRead->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonResize);
    buttonLayout->addWidget(buttonRead);

    mainLayout->addLayout(buttonLayout);

    connect(buttonAdd,SIGNAL(clicked()),this,SLOT(addNewChord()));
    connect(buttonResize,SIGNAL(clicked()),this,SLOT(resizeLayout()));
    connect(buttonRead,SIGNAL(clicked()),this,SLOT(read()));



    //twho columns displayer

    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *vi1 = new QVBoxLayout;
    QVBoxLayout *vi2 = new QVBoxLayout;
    QSpacerItem *s1 = new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    QSpacerItem *s2 = new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);

    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
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

    mainLayout->addWidget(scrollArea);

    //QSpacerItem *spacer = new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Maximum);
    //chordsLayout->addSpacerItem(spacer);

    QHBoxLayout *hlayout = new QHBoxLayout;




    //QSplitter *splitter = new QSplitter(this);
    //splitter->setOrientation(Qt::Horizontal);
    //splitter->addWidget(edit);
    //splitter->addWidget(w);

    //layout->addWidget(splitter);

    hlayout->addWidget(edit);
    //hlayout->addWidget(w);
    hlayout->addLayout(mainLayout);
    layout->addLayout(hlayout);

    connect(edit,SIGNAL(textChanged()),this,SLOT(textChanged()));
}

void Tab::textChanged()
{
   emit setSaveIcon(-1,edit->toPlainText()!=info.text);
}

void Tab::setText(QString text)
{
    edit->setText(text);
}

void Tab::setXTA(XTAinfo xta)
{
    info = xta;
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

    labelInfo->setText(tr("Capo: %1 - Tuning: %2\nTitle: %3\nArtist: %4\nAlbum: %5").arg(QString::number(info.capo), info.tuning, info.title, info.artist, info.album));
}

XTAinfo Tab::getXTA()
{
    info.text = edit->toPlainText();
    return info;
}

void Tab::resizeLayout()
{
    if(v1->count()>0 && v2->count()>0){
        scrollArea->setMinimumWidth(400);
        scrollArea->setMaximumWidth(401);
    }else{
        if(v1->count()>0 || v2->count()>0){
            scrollArea->setMinimumWidth(200);
            scrollArea->setMaximumWidth(201);
        }else{
            scrollArea->setMinimumWidth(0);
            scrollArea->setMaximumWidth(1);
        }
    }

    scrollArea->update();
}

void Tab::addNewChord()
{
    addChord("Em","1 2");
    addChord("Em","0 2 2");
    addChord("Em","0 2 2 0 0");
    addChord("Em","0 0 2 2 0 0 0 0 0 0 0 0");
    resizeLayout();
}

void Tab::read()
{
    QStringList list = highlighter->matches();

    foreach(QString name, list){
        name.remove("\\b");
        name.remove("(?!#)");
        name.remove("(?!(#|')");
        addChord(name);
    }
    resizeLayout();
}

void Tab::addChord(QString name, QString fingers)
{
    if(v1->count() > v2->count())
        v2->addWidget(new Guitar(name,fingers));
    else
        v1->addWidget(new Guitar(name,fingers));
}
void Tab::selectAll() { edit->selectAll(); }
void Tab::undo() { edit->undo(); }
void Tab::redo() { edit->redo(); }
void Tab::cut() { edit->cut(); }
void Tab::copy() { edit->copy(); }
void Tab::paste() { edit->paste(); }
