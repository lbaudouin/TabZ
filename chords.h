#ifndef CHORDS_H
#define CHORDS_H

#include "readwritexml.h"
#include <QTabWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QInputDialog>

#include <QDebug>

#include "guitar.h"

struct Chord{
    Chord() {}
    Chord(QString _name, QString _fingers, QString _comment = "", bool _locked = false) {name=_name; fingers=_fingers; comment=_comment; locked=_locked; }
    QString name;
    QString fingers;
    QString comment;
    bool locked;
};

struct Instrument{
    Instrument() { nbStrings = 6; }
    Instrument(QString _name, QString _label, int _nbStrings = 6) {name=_name; label=_label; nbStrings=_nbStrings; }
    QString name;
    QString label;
    int nbStrings;
};

inline bool operator<(const Instrument &i1, const Instrument &i2){
    if(i1.name < i2.name) return true;
    if(i1.nbStrings < i2.nbStrings) return true;
    return false;
}
inline bool operator==(const Instrument &i1, const Instrument &i2){
    return (i1.name == i2.name) && (i1.nbStrings == i2.nbStrings);
}

class Chords : public ReadWriteXML
{
    Q_OBJECT
public:
    Chords(QWidget *parent = 0);

    void load();
    void save();

    Instrument getInstrument(QString label);
    QString getInstrumentName(QString label);
    QStringList getInstrumentsNames();
    QList<Instrument> getInstruments();
    QList<Chord> getChords(Instrument);
    QList<Chord> getChords(Instrument, QString name);
    QString getFingers(Instrument, QString name);

protected:
    void init();
    virtual void read(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());
    virtual void write(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());

private:
    QWidget* parent_;
    QMap<Instrument,QList<Chord> > chords;
    //QList<Instrument> instruments;

public:
    void addChord(Instrument instrument, QString name, QString fingers, QString comment = "", bool locked = false);
    Instrument addInstrument(QString name, QString label, int nbStrings);
};


#endif // CHORDS_H
