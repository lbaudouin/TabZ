#include "xta.h"

XTA::XTA(QWidget *parent) : parent_(parent)
{

}

XTAinfo XTA::parse(QString filepath, bool *ok)
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
                QMessageBox::warning(parent_,tr("Failed to open XML document"),tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(filepath));
            if(ok!=0)
                *ok = false;
            return XTAinfo("","");
        }
        if (!dom->setContent(&xml_doc)){
            xml_doc.close();
            if(parent_)
                QMessageBox::warning(parent_,tr("Error opening the XML document"),tr("The XML document could not be assigned to the object QDomDocument."));
            if(ok!=0)
                *ok = false;
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

        if(ok!=0)
            *ok = true;
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

void XTA::save(QString filepath, XTAinfo xta)
{
    if(!filepath.endsWith("txt",Qt::CaseInsensitive) && !filepath.endsWith("xta",Qt::CaseInsensitive)){
        QMessageBox::critical(this,tr("Error"),tr("Invalid file suffix, must be 'txt' or 'xta'\nFilename: %1").arg(filepath));
        return;
    }

    QFile file(filepath);
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    if(filepath.endsWith("txt",Qt::CaseInsensitive)){
        qDebug() << "Save txt";
        stream << xta.text;
    }

    if(filepath.endsWith("xta",Qt::CaseInsensitive)){
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

            if(xta.images.size()>0){
                QDomElement nodeImages = dom.createElement("images");
                node1.appendChild(nodeImages);

                for(int i=0;i<xta.refImages.size();i++){
                    QDomElement node2 = dom.createElement("image");
                    node2.setAttribute("ref", QString("image%1").arg(i));
                    nodeImages.appendChild(node2);

                    QImage image = xta.images[xta.refImages[i]];

                    QByteArray ba;
                    QBuffer buf(&ba);
                    image.save(&buf, "png");

                    QDomText textNode = dom.createTextNode(ba.toBase64());
                    node2.appendChild(textNode);
                }
            }
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
