#ifndef XTA_H
#define XTA_H

#include <QWidget>
#include <QDomDocument>
#include <QMessageBox>
#include <QFile>

#include <QDebug>

#include "xta_info.h"

class XTA
{
public:
    XTA(QWidget *parent = 0);

    XTAinfo parse(QString filepath);
    void save(QString filepath, XTAinfo xta);

protected:
    void readHeaders(QDomNode &node, XTAinfo &xta);
    void readContent(QDomNode &node, XTAinfo &xta);
    void readSongInfo(QDomNode &node, XTAinfo &xta);

    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data);

private:
    QWidget *parent_;

};

#endif // XTA_H
