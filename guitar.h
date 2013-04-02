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

    void remove(QString name);
};

class Strings : public QWidget
{
    Q_OBJECT
public:
    explicit Strings(QString fingers, QWidget *parent = 0);
    
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

signals:
    
public slots:
    void select(bool);
    
};

#endif // GUITAR_H
