#include "options.h"

OptionsValues::OptionsValues()
{
    openReadOnly = false;
    enableColors = true;
    selectNewTab = true;
    openSizeMode = 0;
    defaultPath = "";
    reOpenPreviousTabs = false;
}

void OptionsValues::addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data)
{
    QDomElement node = dom.createElement(tag);
    parent.appendChild(node);

    QDomText textNode = dom.createTextNode(data);
    node.appendChild(textNode);
}

void OptionsValues::addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data)
{
    addNode(dom,parent,tag,QString::number(data));
}

void OptionsValues::save(QWidget *parent)
{
    QFile file("options.xml");
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);

    QDomDocument dom;

    QDomElement mainNode = dom.createElement("options");
    dom.appendChild(mainNode);

    {

        addNode(dom,mainNode,"enableColors",enableColors);
        addNode(dom,mainNode,"defaultPath",defaultPath);
        addNode(dom,mainNode,"selectNewTab",selectNewTab);
        addNode(dom,mainNode,"openReadOnly",openReadOnly);
        addNode(dom,mainNode,"openSizeMode",openSizeMode);
        addNode(dom,mainNode,"reOpenPreviousTabs",reOpenPreviousTabs);



        QDomElement colorNode = dom.createElement("colors");
        mainNode.appendChild(colorNode);

        //foreach(const ColorRegExp &colorRegExp, colors){
        foreach(ColorRegExp colorRegExp, colors){
            QDomElement elem = dom.createElement("ColorRegExp");
            elem.setAttribute("regexp",colorRegExp.regExp);
            elem.setAttribute("color",QString("%1,%2,%3").arg(QString::number(colorRegExp.color.red()),QString::number(colorRegExp.color.green()),QString::number(colorRegExp.color.blue())));
            elem.setAttribute("weight",colorRegExp.weight);
            elem.setAttribute("text",colorRegExp.isText);
            elem.setAttribute("caseSensitivity",colorRegExp.caseSensitivity);
            elem.setAttribute("active",colorRegExp.active);
            colorNode.appendChild(elem);
        }

    }

    stream << dom.toString();

    file.close();
}

void OptionsValues::parse(QWidget *parent)
{
    QDomDocument *dom = new QDomDocument("docXML");

    if(!QFile::exists("options.xml")){
        save(parent);
    }

    QFile xml_doc("options.xml");

    if(!xml_doc.open(QIODevice::ReadOnly)){
        if(parent)
            QMessageBox::warning(parent,parent->tr("Failed to open XML document"),parent->tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(xml_doc.fileName()));
        return;
    }
    if(!dom->setContent(&xml_doc)){
        xml_doc.close();
        if(parent)
            QMessageBox::warning(parent,parent->tr("Error opening the XML document"),parent->tr("The XML document could not be assigned to the object QDomDocument."));
        return;
    }

    QDomElement dom_element = dom->documentElement();
    QDomNode node = dom_element.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()=="enableColors"){
            enableColors = element.text().toInt();
        }
        if(element.tagName()=="selectNewTab"){
            selectNewTab = element.text().toInt();
        }
        if(element.tagName()=="openReadOnly"){
            openReadOnly = element.text().toInt();
        }
        if(element.tagName()=="reOpenPreviousTabs"){
            reOpenPreviousTabs = element.text().toInt();
        }
        if(element.tagName()=="defaultPath"){
            defaultPath = element.text();
        }
        if(element.tagName()=="openSizeMode"){
            openSizeMode = element.text().toInt();
        }
        if(element.tagName()=="openSizeMode"){
            openSizeMode = element.text().toInt();
        }

        if(element.tagName()=="colors"){
            colors.clear();

            QDomNode child = node.firstChild();

            while(!child.isNull()){

                QDomElement colorElement = child.toElement();
                if(colorElement.tagName()=="ColorRegExp"){
                    QString regExp = colorElement.attribute("regexp");
                    QString colorString = colorElement.attribute("color","0,0,0");
                    QString weightText =  colorElement.attribute("weight","50");
                    int isText = colorElement.attribute("text","0").toInt();
                    int caseSensitivity = colorElement.attribute("caseSensitivity","1").toInt();
                    int active = colorElement.attribute("active","1").toInt();

                    QStringList colorList = colorString.split(",");
                    QColor color(0,0,0);
                    if(colorList.size()==3){
                        color.setRgb(colorList[0].toInt(),colorList[1].toInt(),colorList[2].toInt());
                    }

                    ColorRegExp cre(regExp,color,weightText.isEmpty()?50:weightText.toInt(),isText,caseSensitivity,active);

                    colors.push_back(cre);
                }

                child =child.nextSibling();
            }
        }

        node = node.nextSibling();
    }

    xml_doc.close();

    if(colors.isEmpty())
        setDefaultRegExp();
}

void OptionsValues::setDefaultRegExp()
{
    colors.clear();
    colors.push_back(  ColorRegExp("\\b[A-G]#(?!(m|[1-9]))", Qt::darkRed, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]#[1-9]\\b", Qt::darkRed, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]#m[1-9]\\b", Qt::darkRed, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]#m\\b", Qt::darkRed, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]b\\b", Qt::darkGreen, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]bm\\b", Qt::darkGreen, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G][1-9]\\b", Qt::darkYellow, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]m[1-9]\\b", Qt::darkBlue, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]m\\b", Qt::darkBlue, QFont::Bold)  );
    colors.push_back(  ColorRegExp("\\b[A-G]dim\\b", Qt::darkMagenta, QFont::Bold)  );

    colors.push_back(  ColorRegExp("\\b[A-G](?!(#|'|\\|))\\b", Qt::red, QFont::Bold)  );


    colors.push_back(  ColorRegExp("\\bIntro\\b", Qt::darkCyan, QFont::Bold, true, Qt::CaseInsensitive)  );
    colors.push_back(  ColorRegExp("\\bChorus\\b", Qt::darkCyan, QFont::Bold, true, Qt::CaseInsensitive)  );
    colors.push_back(  ColorRegExp("\\bbis\\b", Qt::darkCyan, QFont::Bold, true, Qt::CaseInsensitive)  );
}
