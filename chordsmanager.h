#ifndef CHORDSMANAGER_H
#define CHORDSMANAGER_H

#include <QDialog>

#include "chords.h"

#include <QDebug>

class ChordsManager : public QDialog
{
    Q_OBJECT
public:
    explicit ChordsManager(Chords* chords, QWidget *parent = 0);

    Chord addNewChord(QWidget *parent = 0);

    void save();

protected:
    void resizeEvent(QResizeEvent *);

private:
    QTabWidget *tabWidget;
    Chords *chords_;

private slots:
    void buttonClicked(QAbstractButton *button);
    void updateManager();

    
};

#endif // CHORDSMANAGER_H
