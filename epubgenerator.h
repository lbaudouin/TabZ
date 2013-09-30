#ifndef EPUBGENERATOR_H
#define EPUBGENERATOR_H

#include <QFile>

class EpubGenerator
{
public:
    EpubGenerator();

protected:
    void generateContent();
    void generateMimetype();
    void generateTitlePage();
    void generateStyleSheet();
    void generateToc();

    void generatePages();

};

#endif // EPUBGENERATOR_H
