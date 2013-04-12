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
//#include <QPrintPreviewWidget>
#include "qprintpreview.h"

#include "highlighter.h"
#include "xta.h"
#include "xta_info.h"
#include "guitar.h"
#include "options.h"
#include "chords.h"

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

    bool isEditable() {return editable_; }
    void setEditable(bool editable);

    bool isModified();
    bool isUndoAvailable();
    bool isRedoAvailable();


    void setChords(Chords* chordsList) { chordsList_ = chordsList; }

protected:
    void addChordsFromText(QString text);
    //void setUpToolBar();
    
private:
    QTextEdit *edit;
    QLabel *labelInfo,*specialInfo;
    XTAinfo info,modified_info;

    QVBoxLayout *previewLayout;

    QWidget *allInfoWidget;

    QLineEdit *editTitle,*editArtist,*editAlbum,*editTuning;
    QSpinBox *editCapo;

    QVBoxLayout *chordLayout;
    QVBoxLayout*v1,*v2;
    QScrollArea *scrollArea;

    Highlighter *highlighter;

    QStringList chords;
    QMap<QString,QString> mapChord;

    QList<Chord> currentChords;

    OptionsValues optionsValues;

    bool undoAvailable_, redoAvailable_;

    bool editable_;

    QPrintPreview *printPreviewWidget;
    QPrinter *printer;

    Chords *chordsList_;

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

    void addChord(QString name, QString fingers = QString());

    void print(QPrinter*);

public slots:
    void resizeLayout();
    void addNewChord();

    void read();
    void import();
    void importFromXTA();

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

    void updateView();

    void setExpertMode(bool);


    void setOptions(OptionsValues options);

    void insertTab();
    
};

#endif // TAB_H
