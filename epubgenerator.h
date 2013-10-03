#ifndef EPUBGENERATOR_H
#define EPUBGENERATOR_H

#include <QFile>
#include <QPixmap>
#include <QProcess>
#include <QDir>

#include <QDebug>

class EpubGenerator
{
public:
    EpubGenerator();

    void generate();

    void setTitle(QString title) {title_ = title;}

protected:

    void generateContent();
    void generateMimetype();
    void generateTitlePage();
    void generateStyleSheet();
    void generateToc();
    void generateContainer();

    void generatePages();

private:
    QString title_;

};

#endif // EPUBGENERATOR_H
