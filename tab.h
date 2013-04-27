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
#include "chordsmanager.h"
#include "mycheckbox.h"

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(XTAinfo xta, Chords* chordsList, OptionsValues optionsValues, QWidget *parent = 0);
    XTAinfo getXTA();

    bool isEditable() {return editable_; }
    void setEditable(bool editable);

    bool isModified();
    bool isUndoAvailable();
    bool isRedoAvailable();

    QAction* getAction() { return menuAction; }

protected:
    void addChordsFromText(QString text);
    //void setUpToolBar();
    void addImage(QImage &img);

    XTAinfo readXTA(QString filepath = QString());
    
private:
    QTextEdit *edit;
    QLabel *labelInfo,*specialInfo;
    XTAinfo info,modified_info;

    QToolBar *tabToolBar;
    QToolBar *chordToolBar;

    QVBoxLayout *previewLayout;

    QWidget *allInfoWidget;

    QLineEdit *editTitle,*editArtist,*editAlbum,*editTuning;
    QComboBox *comboInstrument;
    QSpinBox *editCapo;

    QVBoxLayout *chordLayout;
    QVBoxLayout*v1,*v2;
    QScrollArea *scrollArea;

    Highlighter *highlighter;

    OptionsValues optionsValues_;

    QStringList chords;
    QList<Chord> currentChords;

    QPrintPreview *printPreviewWidget;
    QPrinter *printer;

    Chords *chordsList_;

    QAction *menuAction;

    bool undoAvailable_, redoAvailable_;
    bool editable_;

    Instrument instrument_;

signals:
    void setSaveIcon(int,bool);
    void setSelected(QString name);
    void removeChord(QString name);

    void undoAvailable(bool);
    void redoAvailable(bool);

    void setChordSize(QSize);

    void changeTabName(QString name);

private slots:
    void infoChanged(QString text = QString());
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

    void saved(QString path = QString());

    void setUndoAvailable(bool);
    void setRedoAvailable(bool);

    void setColors(QList<ColorRegExp> list);

    void updateView();

    void setExpertMode(bool);


    void setOptions(OptionsValues options);

    void insertTab();
    void insertImage();
    void insertClipboard();

    void setAction(QAction*);

    void translateFrEn();
    void translateEnFr();

    void importImages();
    void exportImages();
    
};

#endif // TAB_H
