#include "guitar.h"

Guitar::Guitar(QString name, QString fingers, QWidget *parent) : name_(name), fingers_(fingers),
    QFrame(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout();

    this->setFrameStyle(QFrame::Box);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QLabel *label = new QLabel(" " + name_ + " ");
    label->setFrameStyle(QFrame::Box);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);


    QToolButton *buttonClose = new QToolButton();
    connect(buttonClose,SIGNAL(clicked()),this,SLOT(close()));

    QToolButton *button = new QToolButton();
    button->setCheckable(true);

    Neck *neck = new Neck(fingers_,this);

    connect(button,SIGNAL(toggled(bool)),neck,SLOT(setHidden(bool)));

    hlayout->addWidget(label);
    hlayout->addWidget(button);
    hlayout->addWidget(buttonClose);

    vlayout->addLayout(hlayout);
    vlayout->addWidget(neck);
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
    painter.setPen(Qt::red);

    painter.drawRect(0,0,size_.width(),size_.height());

    //Base point
    QPoint base(20,25);

    //Rect where strings will be drawn
    QRect strings(base,QSize(size_.width()-base.x()-10,size_.height()-base.y()-10));

    //Main Fret
    painter.setPen(Qt::white);
    painter.setBrush(Qt::white);
    painter.drawRect(base.x(),base.y()-5,strings.width(),5);


    painter.setBrush(Qt::NoBrush);

    //Strings
    double incr_v = (double)(strings.width()) / 5.0;
    for(int i=0;i<6;i++){
        painter.drawLine(base.x()+incr_v*i,base.y(),
                         base.x()+incr_v*i,base.y()+strings.height());
    }

    //Frets
    double incr_h = (double)(strings.height()) / 5.0;
    for(int i=0;i<6;i++){
        painter.drawLine(base.x(),base.y()+incr_h*i,
                         base.x()+strings.width(),base.y()+incr_h*i);
    }

    QList<int> poses;
    int min = -1;
    int max = -1;

    //Find min and max, save poses
    for(int i=0;i<6;i++){
        if(fingers_.at(i)=='X'){
            poses.push_back(-1);
            continue;
        }
        int pose = QString(fingers_.at(i)).toInt();
        poses.push_back(pose);

        if(pose>max) max = pose;
        if(pose<min || min==-1) min = pose;
    }

    int shift = 0;

    if(max>5) shift = min-1;

    for(int i=0;i<6;i++){
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
        for(int i=0;i<6;i++){
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
    painter.drawText(0,18,10,10,Qt::AlignCenter,QString::number(1+shift));
}
