#ifndef GUITAR_H
#define GUITAR_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QStyle>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include <QDebug>

class Strings;

class Guitar : public QFrame
{
    Q_OBJECT
public:
    explicit Guitar(QString name, QString fingers = "", QWidget *parent = 0);

    QString getName();
    QString getFingers();

    void setMenu(bool modify, bool reduce, bool close);

private:
    QString name_;
    QString fingers_;
    QSize size_;
    Strings *strings;

    QToolButton *buttonReduce_, *buttonClose_;

signals:
    void closeAndDelete();

private slots:
    void pressClose();
    void pressReduce();

public slots:
    void setSelected(QString);
    void setChordSize(QSize size);

    void remove(QString name);
};

class Strings : public QWidget
{
    Q_OBJECT
public:
    explicit Strings(QString fingers, QWidget *parent = 0);

    void paint(QPainter*, bool printMode = false);

    void setMenu(bool modify, bool reduce, bool close);
    
protected:
    void mousePressEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    //void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    QSize sizeHint();
    QSize minimumSizeHint();

private:
    QString name_;
    QString fingers_;
    QSize size_;
    bool selected_;
    bool reducable_;
    bool closable_;
    bool modifiable_;

signals:
    
public slots:
    void select(bool);
    void setFingers(QString);

    void setReducable(bool reducable);
    void setClosable(bool closable);
    void setModifiable(bool modifiable);

    void setSize(QSize);
    
};

#endif // GUITAR_H
