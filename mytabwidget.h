#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QMouseEvent>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    MyTabWidget(QWidget *parent = 0) : QTabWidget(parent)
    {
        connect(tabBar(),SIGNAL(tabMoved(int,int)),this,SIGNAL(tabMoved(int,int)));
    }
protected:
    void mousePressEvent(QMouseEvent *e){
        if(e->button()==Qt::MiddleButton){
            int index = tabBar()->tabAt(e->pos());
            if(index>=0){
                emit this->tabCloseRequested(index);
            }
        }
    }

signals:
    void tabMoved(int from, int to);

};

#endif // MYTABWIDGET_H
