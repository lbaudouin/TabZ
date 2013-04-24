#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QCheckBox>

class MyQCheckBox : public QCheckBox{
    Q_OBJECT
public:
    MyQCheckBox(QString text) : QCheckBox(text) {}
public slots:
    void uncheck(){
        setChecked(false);
    }
    void check(){
        setChecked(true);
    }
};

#endif // MYCHECKBOX_H
