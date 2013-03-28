#ifndef GUITAR_H
#define GUITAR_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QStyle>

#include <QDebug>

class Guitar : public QFrame
{
    Q_OBJECT
public:
    explicit Guitar(QString name, QString fingers = "", QWidget *parent = 0);

private:
    QString name_;
    QString fingers_;
    QSize size_;

    QToolButton *buttonReduce_, *buttonClose_;

signals:

public slots:
    void reduced(bool);
};

class Neck : public QWidget
{
    Q_OBJECT
public:
    explicit Neck(QString fingers, QWidget *parent = 0);
    
protected:
    //void mousePressEvent(QMouseEvent *);
    //void mouseMoveEvent(QMouseEvent *);
    //void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    QSize sizeHint();
    QSize minimumSizeHint();

private:
    QString name_;
    QString fingers_;
    QSize size_;

signals:
    
public slots:
    
};

#endif // GUITAR_H
