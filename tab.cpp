#include "tab.h"

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    /*if(parent->layout()){
        delete parent->layout();
    }

    if(!parent->layout()){
        QVBoxLayout* layout = new QVBoxLayout(parent);
        layout->setSpacing(6);
        layout->setContentsMargins(11, 11, 11, 11);
        layout->setObjectName(QString::fromUtf8("layout"));
    }

    edit = new QTextEdit;
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    Highlighter *highlighter = new Highlighter(edit->document());
    QLabel *label =new QLabel("test");
    QSplitter *splitter = new QSplitter(parent);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(edit);
    splitter->addWidget(label);

    QLabel *info = new QLabel("Info");
    info->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    parent->layout()->addWidget(info);

    parent->layout()->addWidget(splitter);

    edit->setText(QString::fromUtf8("A Am A# B# C# A\n"
                  "Avec A Amour"));*/


    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    //layout->setContentsMargins(11, 11, 11, 11);
    layout->setObjectName(QString::fromUtf8("layout"));


    QLabel *info = new QLabel("Info");
    info->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    layout->addWidget(info);

    edit = new QTextEdit;
    edit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //Highlighter *highlighter = new Highlighter(edit->document());
    new Highlighter(edit->document());


    QVBoxLayout *chordsLayout = new QVBoxLayout;

    QGridLayout *grid = new QGridLayout();
    grid->setObjectName(QString::fromUtf8("grid"));

    grid->addWidget(new Guitar("F","133211"));
    grid->addWidget(new Guitar("D","XX2320"));
    grid->addWidget(new Guitar("E","022100"));
    grid->addWidget(new Guitar("D#m","X68876"),0,1);
    grid->addWidget(new Guitar("A","577655"),1,1);
    grid->addWidget(new Guitar("Em","022000"),2,1);
    grid->addWidget(new Guitar("D#add9-\\A#","X1234X"));

#if 1
    QScrollArea *s =new QScrollArea();
    s->setAlignment(Qt::AlignCenter);
    s->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QWidget *w2 = new QWidget;
    w2->setLayout(grid);
    s->setWidget(w2);
    s->setWidgetResizable(true);
    s->raise();

    s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    s->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    chordsLayout->addWidget(s);

#else
    chordsLayout->addLayout(grid);
#endif

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
    hlayout->addLayout(chordsLayout);
    layout->addLayout(hlayout);


    edit->setText(QString::fromUtf8("A Am A# B# C# A\n"
                  "C'est Avec le A de Amour"));
}

void Tab::setText(QString text)
{
    edit->setText(text);
}

void Tab::setXTA(XTAinfo xta)
{
    info = xta;
    edit->setText(info.text);
}

XTAinfo Tab::getXTA()
{
    return info;
}
