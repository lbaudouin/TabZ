#ifndef EPUBGENERATOR_H
#define EPUBGENERATOR_H

#include <QFile>
#include <QPixmap>
#include <QProcess>
#include <QDir>

#include <xta_info.h>

#include <QDesktopServices>
#include <QTextDocument>
#include <QDebug>

class EpubGenerator
{
public:
    EpubGenerator();

    void generate(const QList<XTAinfo> &list);

    void setTitle(const QString &title) {title_ = title;}
    void setOutput(const QString &output) {outputEpub_ = output;}
    void setOutputFolder(const QString &outputFolder) {outputFolder_ = outputFolder + QDir::separator();}

protected:

    void generateContent(const QStringList &list);
    void generateMimetype();
    void generateTitlePage();
    void generateStyleSheet();
    void generateToc(const QStringList &list);
    void generateContainer();

    QStringList generatePages(const QList<XTAinfo> &list);

private:
    QString title_;
    QString outputFolder_;
    QString outputEpub_;
    QString tmpFolder_;
    QString epubFolder_;

};

#endif // EPUBGENERATOR_H
