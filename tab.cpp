#include "tab.h"

Tab::Tab(QWidget *parent) : info(""),
    QWidget(parent)
{
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


    mainLayout = new QVBoxLayout;

    QPushButton *buttonAdd = new QPushButton(tr("A"));
    QPushButton *buttonResize = new QPushButton(tr("R"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonAdd);
    buttonLayout->addWidget(buttonResize);

    mainLayout->addLayout(buttonLayout);

    connect(buttonAdd,SIGNAL(clicked()),this,SLOT(addChord()));
    connect(buttonResize,SIGNAL(clicked()),this,SLOT(resizeLayout()));

   /* QGridLayout *grid = new QGridLayout();
    grid->setObjectName(QString::fromUtf8("grid"));

    grid->addWidget(new Guitar("F","133211"));
    grid->addWidget(new Guitar("D","XX2320"));
    grid->addWidget(new Guitar("E","022100"));
    grid->addWidget(new Guitar("D#m","X68876"),0,1);
    grid->addWidget(new Guitar("A","577655"),1,1);
    grid->addWidget(new Guitar("Em","022000"),2,1);
    grid->addWidget(new Guitar("D#add9-\\A#","X1234X"));*/

    scrollArea =new QScrollArea();
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QWidget *w2 = new QWidget;

    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *vi1 = new QVBoxLayout;
    QVBoxLayout *vi2 = new QVBoxLayout;

    h->addLayout(vi1);
    h->addLayout(vi2);

    v1 = new QVBoxLayout;
    v2 = new QVBoxLayout;


    vi1->addLayout(v1);
    vi2->addLayout(v2);

    QSpacerItem *s1 = new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding);
    QSpacerItem *s2 = new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding);

    vi1->addSpacerItem(s1);
    vi2->addSpacerItem(s2);



    v1->addWidget(new Guitar("F","1 3 3 2 1 1"));
    v1->addWidget(new Guitar("D","X X 2 3 2 0"));
    v1->addWidget(new Guitar("X","10 12 12 11 10 10"));
    v1->addWidget(new Guitar("D#m","X 6 8 8 7 6"));
    v2->addWidget(new Guitar("A","5 7 7 6 5 5"));
    v2->addWidget(new Guitar("Em","0 2 2 0 0 0"));
    v2->addWidget(new Guitar("D#add9-\\A#","X 1 2 3 4 X"));



    w2->setLayout(h);
    scrollArea->setWidget(w2);
    //scrollArea->setWidgetResizable(true);
    scrollArea->raise();

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
    info.text = edit->toPlainText();
    return info;
}

void Tab::resizeLayout()
{
    qDebug() << "resizeLayout";
    scrollArea->resize(0,0);
    mainLayout->update();
    v1->update();
    v2->update();
    parentWidget()->update();


}

void Tab::addChord()
{
    v2->addWidget(new Guitar("Em","0 2 2 0 0 0"));
}
