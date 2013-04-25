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

void Guitar::setMenu(bool modify, bool reduce, bool close)
{
    buttonClose_->setVisible(close);
    buttonReduce_->setVisible(reduce);
    strings->setMenu(modify,reduce,close);
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

void Guitar::setChordSize(QSize size)
{
    strings->setSize(size);
    this->repaint();
}

Strings::Strings( QString fingers, QWidget *parent) : selected_(false),
    QWidget(parent)
{
    reducable_ = true;
    closable_ = true;
    modifiable_ = true;

    setFingers(fingers);

    //size_ = QSize(150,200);
    size_ = QSize(100,150);
    this->resize(size_);
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    setMinimumSize( size_ );
}

void Strings::setMenu(bool modify, bool reduce, bool close)
{
    modifiable_ = modify;
    reducable_ = reduce;
    closable_ = close;
}

void Strings::setSize(QSize size)
{
    size_ = size;
    setMinimumSize( size_ );
    this->update();
}

void Strings::setFingers(QString fingers)
{
    fingers_ = fingers;
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
    this->update();
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
    paint(&painter);
}

void Strings::paint(QPainter *painter, bool printMode)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    if(!printMode){
        if(selected_){
            painter->setBrush(Qt::darkBlue);
            painter->setPen(Qt::darkBlue);
        }else{
            painter->setBrush(Qt::black);
            painter->setPen(Qt::black);
        }
    }else{
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::NoBrush);
    }

    painter->drawRect(0,0,size_.width(),size_.height());

    //Base point
    QPoint base(20,25);

    //Rect where strings will be drawn
    QRect strings(base,QSize(size_.width()-base.x()-10,size_.height()-base.y()-10));

    //Main Fret
    if(printMode){
        painter->setPen(Qt::darkGray);
        painter->setBrush(Qt::darkGray);
    }else{
        painter->setPen(Qt::white);
        painter->setBrush(Qt::white);
    }

    painter->drawRect(base.x(),base.y()-5,strings.width(),5);

    painter->setBrush(Qt::NoBrush);

    painter->drawRect(strings);





    QList<int> poses;
    int min = -1;
    int max = -1;

    //Find min and max, save poses
    QList<QString> fingers = fingers_.split(" ", QString::SkipEmptyParts);
    int nbString = fingers.size();

    if(nbString<2|| nbString>12){
        painter->restore();
        return;
    }

    bool containEmpty = false;

    foreach(QString finger, fingers){
        if(finger=="X"||finger=="x"){
            poses.push_back(-1);
            continue;
        }
        bool ok;
        int pose = finger.toInt(&ok);
        if(!ok){
            poses.push_back(-1);
            continue;
        }else{
            poses.push_back(pose);
        }

        if(pose==0){
            containEmpty = true;
            continue;
        }

        if(pose>max) max = pose;
        if(pose<min || min==-1) min = pose;
    }

    int shift = 0;

    if(max>5) shift = min-1;

    //Strings
    double incr_v = (double)(strings.width()) / (double)(nbString-1);
    for(int i=0;i<nbString;i++){
        painter->drawLine(base.x()+incr_v*i,base.y(),
                         base.x()+incr_v*i,base.y()+strings.height());
    }

    //Frets
    double incr_h = (double)(strings.height()) / 5;
    for(int i=0;i<6;i++){
        painter->drawLine(base.x(),base.y()+incr_h*i,
                         base.x()+strings.width(),base.y()+incr_h*i);
    }


    int s = 5.0 * size_.width() / 150.0;


    for(int i=0;i<nbString;i++){
        //Draw a cross in case of no playing string
        if(poses[i]<0){
            if(printMode)
                painter->setPen(Qt::black);
            else
                painter->setPen(QPen(Qt::red,2));
            painter->drawLine(base.x()+incr_v*i-s,10-s,
                             base.x()+incr_v*i+s,10+s);
            painter->drawLine(base.x()+incr_v*i+s,10-s,
                             base.x()+incr_v*i-s,10+s);
            continue;
        }
        int pose = poses[i];

        //Draw a circle in case of empty string
        if(pose==0){
            if(printMode)
                painter->setPen(Qt::black);
            else
                painter->setPen(QPen(Qt::red,2));

            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(QPointF(base.x()+i*incr_v,10),s,s);
            continue;
        }

        pose -= shift;

        //If pose is out of the drawing
        if(pose>5) continue;


        if(printMode){
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
        }else{
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
        }
        painter->drawEllipse(QPointF(base.x()+i*incr_v,base.y()+pose*incr_h - (double)incr_h/2.0),s+2,s+2);
    }

    //Draw a bar if needed
    if(min>0 && !containEmpty){
        QList<int> list;
        for(int i=0;i<nbString;i++){
            if(poses[i]==min)
                list.push_back(i);
        }       

        //If there are several minima, draw a bar
        if(list.size()>1){
            if(printMode){
                painter->setPen(Qt::black);
                painter->setBrush(Qt::black);
            }else{
                painter->setPen(Qt::red);
                painter->setBrush(Qt::red);
            }
            painter->drawRect(base.x()+list.first()*incr_v, base.y()+(double)(min-shift)*incr_h-(s+2) - (double)incr_h/2.0,
                             (double)(list.last()-list.first())*incr_v,2*s+5);
        }
    }

    //Print first fret
    QFont font  = painter->font();

    if(printMode){
        painter->setPen(Qt::black);
        font.setPointSize(8);
    }else{
        painter->setPen(Qt::white);
        font.setPointSize(12);
    }
    painter->setFont(font);

    painter->setBrush(Qt::NoBrush);
    painter->drawText(0,16,20,16,Qt::AlignCenter,QString::number(1+shift));

    painter->restore();
}

void Strings::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton){
        if(reducable_ || closable_ || modifiable_){
            QMenu * menu = new QMenu(this);

            if(modifiable_){
                QAction* actionModify = menu->addAction(tr("Modify"));
                actionModify->setEnabled(false);
                //connect(actionModify,SIGNAL(triggered()),this,SLOT(()));
            }

            if(reducable_){
                QAction* actionReduce = menu->addAction(tr("Reduce"));
                connect(actionReduce,SIGNAL(triggered()),(Guitar*)this->parent(),SLOT(pressReduce()));
            }

            if(closable_){
                QAction* actionClose = menu->addAction(tr("Close"));
                connect(actionClose,SIGNAL(triggered()),(Guitar*)this->parent(),SLOT(pressClose()));
            }

            menu->move(event->globalPos());
            menu->show();
        }
    }
}

void Strings::select(bool state)
{
    selected_ = state;
    this->update();
}

void Strings::setReducable(bool reducable)
{
    reducable_ = reducable;
}

void Strings::setClosable(bool closable)
{
    closable_ = closable;
}

void Strings::setModifiable(bool modifiable)
{
    modifiable_ = modifiable;
}
