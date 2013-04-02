#include "xta.h"

XTA::XTA(QWidget *parent) : parent_(parent)
{

}

XTAinfo XTA::parse(QString filepath)
{
    QDomDocument *dom = new QDomDocument("docXML");
    QFile xml_doc(filepath);

    QFileInfo fi(filepath);
    XTAinfo xta(fi.absoluteFilePath(),fi.fileName());

    if(fi.suffix()=="txt" || fi.suffix()=="TXT"){
        QFile file(filepath);
        if(file.open(QFile::ReadOnly)){
            QTextStream in(&file);
            QString text = in.readAll();
            xta.text = text;
        }
        file.close();
    }

    if(fi.suffix()=="xta" || fi.suffix()=="XTA"){

        if(!xml_doc.open(QIODevice::ReadOnly)){
            if(parent_)
                QMessageBox::warning(parent_,parent_->tr("Failed to open XML document"),parent_->tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(filepath));
            return XTAinfo("","");
        }
        if (!dom->setContent(&xml_doc)){
            xml_doc.close();
            if(parent_)
                QMessageBox::warning(parent_,parent_->tr("Error opening the XML document"),parent_->tr("The XML document could not be assigned to the object QDomDocument."));
            return XTAinfo("","");
        }

        QDomElement dom_element = dom->documentElement();
        QDomNode node = dom_element.firstChild();


        while(!node.isNull())
        {
            QDomElement element = node.toElement();

            //qDebug() << element.tagName();

            if(element.tagName()=="Header"){
                readHeaders(node,xta);
            }

            if(element.tagName()=="Contenu"){
                readContent(node,xta);
            }

            node = node.nextSibling();
        }

        xml_doc.close();
    }

    return xta;
}

void XTA::readHeaders(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        //qDebug() << element.tagName();

        if(element.tagName()=="Musique"){
            readSongInfo(child,xta);
        }

        if(element.tagName()=="Version")
            xta.version = element.text();
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

        //qDebug() << element.tagName();

        if(element.tagName()=="TXT")
            xta.text = element.text();
        if(element.tagName()=="TableAccords")
            xta.chords = element.text();

        child = child.nextSibling();
    }
}

void XTA::readSongInfo(QDomNode &node, XTAinfo &xta)
{
    QDomNode child = node.firstChild();

    while(!child.isNull())
    {
        QDomElement element = child.toElement();

        //qDebug() << element.tagName();

        if(element.tagName()=="titre")
            xta.title = element.text();
        if(element.tagName()=="artiste")
            xta.artist = element.text();
        if(element.tagName()=="album")
            xta.album = element.text();

        child = child.nextSibling();
    }
}

void XTA::save(QString filepath, XTAinfo xta)
{
    QFile file(filepath);
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);

    QFileInfo fi(filepath);
    if(fi.suffix()=="txt" || fi.suffix()=="TXT"){
        qDebug() << "Save txt";
        stream << xta.text;
    }

    if(fi.suffix()=="xta" || fi.suffix()=="XTA"){
        QDomDocument dom;

        QDomElement nodeXTA = dom.createElement("XTA");
        dom.appendChild(nodeXTA);

        {
            QDomElement node1 = dom.createElement("Header");
            nodeXTA.appendChild(node1);

            addNode(dom,node1,"Version",xta.version);
            addNode(dom,node1,"Capo",xta.capo);
            addNode(dom,node1,"Accordage",xta.tuning);

            QDomElement node2 = dom.createElement("Musique");
            node1.appendChild(node2);

            addNode(dom,node2,"titre",xta.title);
            addNode(dom,node2,"artiste",xta.artist);
            addNode(dom,node2,"album",xta.album);


            addNode(dom,node1,"FichierGTP",xta.file_gp);
            addNode(dom,node1,"FichierMP3",xta.file_mp3);

        }

        {
            QDomElement node1 = dom.createElement("Contenu");
            nodeXTA.appendChild(node1);

            addNode(dom,node1,"TXT",xta.text);
            addNode(dom,node1,"TableAccords",xta.chords);
        }

        stream << dom.toString();
    }

    file.close();

}


void XTA::addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data)
{
    QDomElement node = dom.createElement(tag);
    parent.appendChild(node);

    QDomText textNode = dom.createTextNode(data);
    node.appendChild(textNode);
}

void XTA::addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data)
{
    addNode(dom,parent,tag,QString::number(data));
}
