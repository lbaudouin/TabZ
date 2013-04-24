#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    MyTabWidget(QWidget *parent = 0) : QTabWidget(parent)
    {
        connect(tabBar(),SIGNAL(tabMoved(int,int)),this,SIGNAL(tabMoved(int,int)));
    }
signals:
    void tabMoved(int from, int to);

};

#endif // MYTABWIDGET_H
