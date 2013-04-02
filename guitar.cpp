#include "guitar.h"

Guitar::Guitar(QString name, QString fingers, QWidget *parent) : name_(name), fingers_(fingers),
    QFrame(parent)
{
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
    connect(buttonClose_,SIGNAL(clicked()),this,SLOT(pressClose()));

    buttonReduce_ = new QToolButton();
    //buttonReduce_->setIcon( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton) );
    buttonReduce_->setCheckable(true);

    QIcon icon;
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarUnshadeButton).pixmap(16), QIcon::Normal, QIcon::On );
    icon.addPixmap( this->style()->standardIcon(QStyle::SP_TitleBarShadeButton).pixmap(16), QIcon::Normal, QIcon::Off );
    buttonReduce_->setIcon(icon);

    strings = new Strings(fingers_,this);

    connect(buttonReduce_,SIGNAL(toggled(bool)),strings,SLOT(setHidden(bool)));
    //connect(buttonReduce_,SIGNAL(toggled(bool)),this,SLOT(reduced(bool)));

    hlayout->addWidget(label);
    hlayout->addWidget(buttonReduce_);
    hlayout->addWidget(buttonClose_);

    vlayout->addLayout(hlayout);
    vlayout->addWidget(strings);
}

void Guitar::pressReduce()
{
    buttonReduce_->setChecked(true);
}

void Guitar::pressClose()
{
    emit closeAndDelete();
}

void Guitar::remove(QString name)
{
    if(name==name_){
        pressClose();
    }
}

void Guitar::setSelected(QString name)
{
    if(name==name_){
        strings->select(true);
    }else{
        strings->select(false);
    }
}

QString Guitar::getName()
{
    return name_;
}

QString Guitar::getFingers()
{
    return fingers_;
}

Strings::Strings( QString fingers, QWidget *parent) : fingers_(fingers), selected_(false),
    QWidget(parent)
{
    fingers_.replace("\t"," ");
    fingers_.replace(","," ");

    if(!fingers_.contains(" ")){
        QString temp;
        for(int i=0;i<fingers_.size();i++){
            temp += QString(" ") + fingers_.at(i);
        }
        fingers_ = temp;
    }
    fingers_.trimmed();

    size_ = QSize(150,200);
    //size_ = QSize(100,150);
    this->resize(size_);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    setMinimumSize( size_ );
}

QSize Strings::sizeHint()
{
    return size_;
}

QSize Strings::minimumSizeHint()
{
    return size_;
}

void Strings::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(0,0,size_.width(),size_.height());

    if(selected_){
        painter.setBrush(Qt::darkBlue);
        painter.setPen(Qt::darkBlue);
    }else{
        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
    }

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

void Strings::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton){
        QMenu * menu = new QMenu(this);
        QAction* actionModify = menu->addAction(tr("Modify"));
        QAction* actionReduce = menu->addAction(tr("Reduce"));
        QAction* actionClose = menu->addAction(tr("Close"));

        actionModify->setEnabled(false);

        //connect(actionModify,SIGNAL(triggered()),this,SLOT(()));
        connect(actionReduce,SIGNAL(triggered()),(Guitar*)this->parent(),SLOT(pressReduce()));
        connect(actionClose,SIGNAL(triggered()),(Guitar*)this->parent(),SLOT(pressClose()));
        menu->move(event->globalPos());
        menu->show();
    }
}

void Strings::select(bool state)
{
    selected_ = state;
    this->update();
}
