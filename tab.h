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
    explicit Tab(XTAinfo xta, QWidget *parent = 0);
    XTAinfo getXTA();

    bool isModified();

protected:
    void addChord(QString name, QString fingers = QString());
    
private:
    QTextEdit *edit;
    QLabel *labelInfo,*specialInfo;
    XTAinfo info,modified_info;

    QLineEdit *editTitle,*editArtist,*editAlbum,*editTuning;
    QSpinBox *editCapo;

    QVBoxLayout *chordLayout;
    QVBoxLayout*v1,*v2;
    QScrollArea *scrollArea;

    Highlighter *highlighter;

    QStringList chords;
    QMap<QString,QString> mapChord;

signals:
    void setSaveIcon(int,bool);
    void setSelected(QString name);

private slots:
    void textChanged(QString text = QString());
    void capoChanged(int);
    void deleteGuitar();
    void updateTitle();

    void updateSelectedNote();

public slots:

    void resizeLayout();
    void addNewChord();

    void read();

    void selectAll();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    void saved();
    
};

#endif // TAB_H
