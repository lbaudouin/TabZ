#ifndef CHORDSMANAGER_H
#define CHORDSMANAGER_H

#include <QDialog>

#include "chords.h"

class ChordsManager : public QDialog
{
    Q_OBJECT
public:
    explicit ChordsManager(QWidget *parent = 0);
    void setChords(Chords *chords);

    void updateManager();

    Chord addNewChord(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QTabWidget *tabWidget;
    Chords *chords_;

private slots:
    void buttonClicked(QAbstractButton *button);
    
};

#endif // CHORDSMANAGER_H
