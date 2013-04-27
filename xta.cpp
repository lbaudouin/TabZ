#include "xta.h"

XTA::XTA(QWidget *parent) : ReadWriteXML(parent)
{

}

void XTA::readTXT(QFileInfo fileInfo)
{
    QFile file(fileInfo.absoluteFilePath());
    if(file.open(QFile::ReadOnly)){
        QTextStream in(&file);
        QString text = in.readAll();
        info_.text = text;
    }
    file.close();
}

void XTA::writeTXT(QFileInfo fileInfo)
{
    QFile file(fileInfo.absoluteFilePath());
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    stream << info_.text;
    file.close();
}

void XTA::read(QDomDocument *dom, QFileInfo fileInfo)
{
    XTAinfo xta(fileInfo.absoluteFilePath(),fileInfo.fileName());

    QDomElement dom_element = dom->documentElement();
    QDomNode node = dom_element.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()=="Header"){
            readHeaders(node,xta);
        }

        if(element.tagName()=="Contenu"){
            readContent(node,xta);
        }

        node = node.nextSibling();
    }

    info_ = xta;
}

void XTA::readHeaders(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        if(element.tagName()=="Musique"){
            readSongInfo(child,xta);
        }

        if(element.tagName()=="Version")
            xta.version = element.text();
        if(element.tagName()=="instrument")
            xta.instrument = element.text();
        if(element.tagName()=="Capo")
            xta.capo = element.text().toInt();
        if(element.tagName()=="Accordage"){
            xta.tuning = element.text();
            if(xta.tuning=="EBGDAE")
                xta.tuning = "EADGBE";
        }
        if(element.tagName()=="FichierGTP")
            xta.file_gp = element.text();
        if(element.tagName()=="FichierMP3")
            xta.file_mp3 = element.text();

        child = child.nextSibling();
    }
}

void XTA::readContent(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        if(element.tagName()=="TXT")
            xta.text = element.text();
        if(element.tagName()=="TableAccords")
            xta.chords = element.text();
        if(element.tagName()=="images"){
            readImagesInfo(child,xta);
        }

        child = child.nextSibling();
    }
}

void XTA::readSongInfo(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        if(element.tagName()=="titre")
            xta.title = element.text();
        if(element.tagName()=="artiste")
            xta.artist = element.text();
        if(element.tagName()=="album")
            xta.album = element.text();

        child = child.nextSibling();
    }
}

void XTA::readImagesInfo(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        if(element.tagName()=="image"){
            QString ref = element.attribute("ref");
            QByteArray ba( element.text().toUtf8() );
            QImage img = QImage::fromData( QByteArray::fromBase64(ba) );
            xta.images << img;
        }

        child = child.nextSibling();
    }
}

void XTA::write(QDomDocument *dom, QFileInfo)
{

    QDomElement nodeXTA = dom->createElement("XTA");
    dom->appendChild(nodeXTA);

    {
        QDomElement node1 = dom->createElement("Header");
        nodeXTA.appendChild(node1);

        addNode(dom,&node1,"Version",info_.version);
        addNode(dom,&node1,"instrument",info_.instrument);
        addNode(dom,&node1,"Capo",info_.capo);
        addNode(dom,&node1,"Accordage",info_.tuning);

        QDomElement node2 = dom->createElement("Musique");
        node1.appendChild(node2);

        addNode(dom,&node2,"titre",info_.title);
        addNode(dom,&node2,"artiste",info_.artist);
        addNode(dom,&node2,"album",info_.album);

        addNode(dom,&node1,"FichierGTP",info_.file_gp);
        addNode(dom,&node1,"FichierMP3",info_.file_mp3);

    }

    {
        QDomElement node1 = dom->createElement("Contenu");
        nodeXTA.appendChild(node1);

        addNode(dom,&node1,"TXT",info_.text);
        addNode(dom,&node1,"TableAccords",info_.chords);

        if(info_.images.size()>0){
            QDomElement nodeImages = dom->createElement("images");
            node1.appendChild(nodeImages);

            for(int i=0;i<info_.refImages.size();i++){
                QDomElement node2 = dom->createElement("image");
                node2.setAttribute("ref", QString("image%1").arg(i));
                nodeImages.appendChild(node2);

                QImage image = info_.images[info_.refImages[i]];

                QByteArray ba;
                QBuffer buf(&ba);
                image.save(&buf, "png");

                QDomText textNode = dom->createTextNode(ba.toBase64());
                node2.appendChild(textNode);
            }
        }
    }
}

bool XTA::checkFileName(QFileInfo fileInfo)
{
    if(!fileInfo.fileName().endsWith(".xta",Qt::CaseInsensitive) && !fileInfo.fileName().endsWith(".txt",Qt::CaseInsensitive)){
        QMessageBox::critical(this,tr("Error"),tr("Invalid file suffix, must be 'txt' or 'xta'\nFilename: %1").arg(fileInfo.fileName()));
        return false;
    }
    QFile file(fileInfo.absoluteFilePath());
    if(file.open(QFile::WriteOnly)){
        return true;
    }else{
        QMessageBox::critical(this,tr("Error"),tr("This file is not writable: %1").arg(fileInfo.absoluteFilePath()));
        return false;
    }
}

XTAinfo XTA::load(QString filepath, bool *ok)
{
    parse(filepath);
    return info_;
}

void XTA::save(QString filepath, XTAinfo &info)
{
    info_ = info;
    flush(filepath);
}
