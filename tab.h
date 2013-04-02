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
#include "options.h"

//TODO, move in a separate file
class QQCheckBox : public QCheckBox{
    Q_OBJECT
public:
    QQCheckBox(QString text) : QCheckBox(text) {}
public slots:
    void uncheck(){
        setChecked(false);
    }
    void check(){
        setChecked(true);
    }
};

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(XTAinfo xta, QWidget *parent = 0);
    XTAinfo getXTA();


    void setOptions(OptionsValues options);
    bool isModified();
    bool isUndoAvailable();
    bool isRedoAvailable();

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

    OptionsValues optionsValues;

    bool undoAvailable_, redoAvailable_;

signals:
    void setSaveIcon(int,bool);
    void setSelected(QString name);
    void removeChord(QString name);

    void undoAvailable(bool);
    void redoAvailable(bool);

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

    void enableColors(bool);

    void setUndoAvailable(bool);
    void setRedoAvailable(bool);

    void setColors(QList<ColorRegExp> list);
    
};

#endif // TAB_H
