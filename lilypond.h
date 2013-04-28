#ifndef LILYPOND_H
#define LILYPOND_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QPicture>
#include <QMessageBox>

#include <QProcess>

#include <QDebug>

class Lilypond : public QDialog
{
    Q_OBJECT
public:
    explicit Lilypond(QWidget *parent = 0);

    bool isLilypondAvailable();
    bool downloadLilypond();

    QPicture getPicture() { return picture; }
    QImage getImage() { return image; }
    
private:
    QTextEdit *edit;
    QLabel *label;
    QPicture picture;
    QImage image;
    QPushButton *okButton;

signals:
    
public slots:
    void generate();
    void textChanged();
    
};

#endif // LILYPOND_H