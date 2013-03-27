#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

#include "highlighter.h"
#include "xta_info.h"
#include "guitar.h"

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = 0);
    XTAinfo getXTA();
    
private:
    QTextEdit *edit;
    XTAinfo info;

    QVBoxLayout *mainLayout;
    QVBoxLayout*v1,*v2;
    QScrollArea *scrollArea;

signals:
    
public slots:
    void setText(QString text);
    void setXTA(XTAinfo xta);

    void resizeLayout();
    void addChord();
    
};

#endif // TAB_H
