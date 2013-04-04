#ifndef CHORDS_H
#define CHORDS_H

#include <QWidget>
#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDialogButtonBox>

#include <QDebug>

#include "guitar.h"

struct Chord{
    QString name;
    QString fingers;
    QString comment;
};

struct Instrument{
    QString name;
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

class Chords : public QObject
{
    Q_OBJECT
public:
    Chords(QWidget *parent);
    void parse();
    void save();

    QStringList getInstrumentsNames();
    QList<Instrument> getInstruments();
    QList<Chord> getChords(Instrument);

protected:
    void init();

private:
    QWidget* parent_;
    QMap<Instrument,QList<Chord> > chords;
    //QList<Instrument> instruments;

public:
    void addChord(Instrument instrument, QString name, QString fingers, QString comment = "");
    Instrument addInstrument(QString name, int nbStrings);
};

class ChordsManager : public QDialog
{
    Q_OBJECT
public:
    ChordsManager(QWidget *parent = 0);
    void setChords(Chords *chords);

    void updateManager();

protected:
    void resizeEvent(QResizeEvent *);

private:
    QTabWidget *tabWidget;
    Chords *chords_;

private slots:
    void buttonClicked(QAbstractButton *button);
};

#endif // CHORDS_H
