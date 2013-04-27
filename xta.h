#ifndef XTA_H
#define XTA_H

#include "readwritexml.h"

#include "xta_info.h"

#include <QBuffer>

class XTA : public ReadWriteXML
{
    Q_OBJECT
public:
    XTA(QWidget *parent = 0);

    XTAinfo load(QString filepath, bool *ok = 0);
    void save(QString filepath, XTAinfo &info);


protected:
    virtual void read(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());
    virtual void write(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());
    virtual bool checkFileName(QFileInfo fileInfo);

    void readTXT(QFileInfo fileInfo);
    void writeTXT(QFileInfo fileInfo);

    void readHeaders(QDomNode &node, XTAinfo &xta);
    void readContent(QDomNode &node, XTAinfo &xta);
    void readSongInfo(QDomNode &node, XTAinfo &xta);
    void readImagesInfo(QDomNode &node, XTAinfo &xta);

private:
    XTAinfo info_;

};

#endif // XTA_H
