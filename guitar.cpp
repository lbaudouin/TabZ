#include "guitar.h"

Guitar::Guitar(QString name, QString fingers, QWidget *parent) : name_(name), fingers_(fingers),
    QFrame(parent)
{
    //TODO, move it
    QMap<QString,QString> mapChord;
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

    if(fingers_.isEmpty()){
        if(mapChord.contains(name_)){
            fingers_ = mapChord[name_];
        }else{
            fingers_ = "X X X X X X";
        }
    }

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSizeConstraint(QLayout::SetMinimumSize);

    this->setFrameStyle(QFrame::Box);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    //this->setMinimumSize(150,200);
    //this->setMaximumWidth(200);

    QLabel *label = new QLabel(" " + name_ + " ");
    label->setFrameStyle(QFrame::Box);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);


    buttonClose_ = new QToolButton();
    buttonClose_->setIcon( this->style()->standardIcon(QStyle::SP_DialogCloseButton) );
    connect(buttonClose_,SIGNAL(clicked()),this,SLOT(close()));

    buttonReduce_ = new QToolButton();
    buttonReduce_->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton) );
    buttonReduce_->setCheckable(true);

    Neck *neck = new Neck(fingers_,this);

    connect(buttonReduce_,SIGNAL(toggled(bool)),neck,SLOT(setHidden(bool)));
    connect(buttonReduce_,SIGNAL(toggled(bool)),this,SLOT(reduced(bool)));

    hlayout->addWidget(label);
    hlayout->addWidget(buttonReduce_);
    hlayout->addWidget(buttonClose_);

    vlayout->addLayout(hlayout);
    vlayout->addWidget(neck);
}

void Guitar::reduced(bool state)
{
    if(state){
        buttonReduce_->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarUnshadeButton) );
    }else{
        buttonReduce_->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton) );
    }
}

Neck::Neck( QString fingers, QWidget *parent) : fingers_(fingers),
    QWidget(parent)
{
    size_ = QSize(150,200);
    this->resize(size_);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    setMinimumSize( size_ );
}

QSize Neck::sizeHint()
{
    return size_;
}

QSize Neck::minimumSizeHint()
{
    return size_;
}

void Neck::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(0,0,size_.width(),size_.height());

    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);

    painter.drawRect(0,0,size_.width(),size_.height());

    //Base point
    QPoint base(20,25);

    //Rect where strings will be drawn
    QRect strings(base,QSize(size_.width()-base.x()-10,size_.height()-base.y()-10));

    //Main Fret
    painter.setPen(Qt::white);
    painter.drawRect(strings);


    painter.setBrush(Qt::white);
    painter.drawRect(base.x(),base.y()-5,strings.width(),5);


    painter.setBrush(Qt::NoBrush);


    QList<int> poses;
    int min = -1;
    int max = -1;

    //Find min and max, save poses
    QList<QString> fingers = fingers_.split(" ", QString::SkipEmptyParts);
    int nbString = fingers.size();


    if(nbString<2|| nbString>12)
        return;

    foreach(QString finger, fingers){
        if(finger=="X"||finger=="x"){
            poses.push_back(-1);
            continue;
        }
        int pose = finger.toInt();
        poses.push_back(pose);

        if(pose>max) max = pose;
        if(pose<min || min==-1) min = pose;
    }

    int shift = 0;

    if(max>5) shift = min-1;

    //Strings
    double incr_v = (double)(strings.width()) / (double)(nbString-1);
    for(int i=0;i<nbString;i++){
        painter.drawLine(base.x()+incr_v*i,base.y(),
                         base.x()+incr_v*i,base.y()+strings.height());
    }

    //Frets
    double incr_h = (double)(strings.height()) / 5;
    for(int i=0;i<6;i++){
        painter.drawLine(base.x(),base.y()+incr_h*i,
                         base.x()+strings.width(),base.y()+incr_h*i);
    }


    for(int i=0;i<nbString;i++){
        //Draw a cross in case of no playing string
        if(poses[i]<0){
            painter.setPen(QPen(Qt::red,2));
            painter.drawLine(base.x()+incr_v*i-5,5,
                             base.x()+incr_v*i+5,15);
            painter.drawLine(base.x()+incr_v*i+5,5,
                             base.x()+incr_v*i-5,15);
            continue;
        }
        int pose = poses[i] - shift;

        //If pose is out of the drawing
        if(pose>5) continue;

        painter.setPen(Qt::red);

        //Draw a circle in case of empty string
        if(pose==0){
            painter.setPen(QPen(Qt::red,2));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QPointF(base.x()+i*incr_v,10),5,5);
        }else{
            painter.setBrush(Qt::red);
            painter.drawEllipse(QPointF(base.x()+i*incr_v,base.y()+pose*incr_h - (double)incr_h/2.0),7,7);
        }
    }

    //Draw a bar if needed
    if(min>0){
        QList<int> list;
        for(int i=0;i<nbString;i++){
            if(poses[i]==min)
                list.push_back(i);
        }

        //If there are several minima, draw a bar
        if(list.size()>1){
            painter.setBrush(Qt::red);
            painter.drawRect(base.x()+list.first()*incr_v, base.y()+(double)(min-shift)*incr_h-7 - (double)incr_h/2.0,
                             (double)(list.last()-list.first())*incr_v,15);
        }
    }

    //Print first fret
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(0,18,20,10,Qt::AlignCenter,QString::number(1+shift));
}
