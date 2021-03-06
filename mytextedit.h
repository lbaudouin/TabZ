#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>
#include <QMimeData>
#include <QUrl>
#include <QFile>
#include <QTextStream>

#include <QKeyEvent>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyTextEdit(QWidget *parent = 0);

protected:
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void insertFromMimeData(const QMimeData *source);

    virtual void keyPressEvent(QKeyEvent *event);
    
signals:
    void insertNewImage(QImage &img);
    void insertNewText(QString &text);
    
public slots:
    
};

#endif // MYTEXTEDIT_H
