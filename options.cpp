#include "options.h"

Options::Options(QWidget *parent) : ReadWriteXML(parent)
{
    resetDefault();
}

void Options::resetDefault()
{
    optionsValues.openReadOnly = false;
    optionsValues.enableColors = true;
    optionsValues.selectNewTab = true;
    optionsValues.openSizeMode = 0;
    optionsValues.lastSizeMode = 0;
    optionsValues.defaultPath = "";
    optionsValues.reOpenPreviousTabs = false;
    optionsValues.chordSize = QSize(150,200);
    optionsValues.enableColorsOnPrinting = true;
    optionsValues.printHearderOnEachPages = true;
    optionsValues.mainToolBarPosition = 0;
    optionsValues.topMargin = 10;
    optionsValues.leftMargin = 10;
    optionsValues.rightMargin = 10;
    optionsValues.bottomMargin = 10;
    optionsValues.defaultOutputFolder = QDir::homePath();

#if defined(__WIN32__)
    optionsValues.mainFont =  QFont("Lucida Console",12);
    optionsValues.titleFont =  QFont("Arial",14,QFont::Bold);
    optionsValues.artistFont =  QFont("Arial",12,QFont::Bold);
    optionsValues.otherFont =  QFont("Arial",8);
#else
    optionsValues.mainFont = QFont("DejaVu Sans Mono",12);
    optionsValues.titleFont = QFont("Ubuntu",14,QFont::Bold);
    optionsValues.artistFont = QFont("Ubuntu",12,QFont::Bold);
    optionsValues.otherFont = QFont("Ubuntu",8);
#endif

    setDefaultRegExp();
}

void Options::load()
{
    if(QFile::exists(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "options.xml"))
        parse(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "options.xml");
}

void Options::save()
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if(!dir.exists()){
        dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    }
    flush(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "options.xml");
}

void Options::write(QDomDocument *dom, QFileInfo)
{
    QDomElement mainNode = dom->createElement("options");
    dom->appendChild(mainNode);

    {

        addNode(dom,&mainNode,"enableColors",optionsValues.enableColors);
        addNode(dom,&mainNode,"defaultPath",optionsValues.defaultPath);
        addNode(dom,&mainNode,"selectNewTab",optionsValues.selectNewTab);
        addNode(dom,&mainNode,"openReadOnly",optionsValues.openReadOnly);
        addNode(dom,&mainNode,"openSizeMode",optionsValues.openSizeMode);
        addNode(dom,&mainNode,"lastSizeMode",optionsValues.lastSizeMode);
        addNode(dom,&mainNode,"chordSize",fromSize(optionsValues.chordSize));
        addNode(dom,&mainNode,"reOpenPreviousTabs",optionsValues.reOpenPreviousTabs);
        addNode(dom,&mainNode,"mainFont",optionsValues.mainFont.toString());
        addNode(dom,&mainNode,"titleFont",optionsValues.titleFont.toString());
        addNode(dom,&mainNode,"artistFont",optionsValues.artistFont.toString());
        addNode(dom,&mainNode,"otherFont",optionsValues.otherFont.toString());
        addNode(dom,&mainNode,"enableColorsOnPrinting",optionsValues.enableColorsOnPrinting);
        addNode(dom,&mainNode,"printHearderOnEachPages",optionsValues.printHearderOnEachPages);
        addNode(dom,&mainNode,"mainToolBarPosition",optionsValues.mainToolBarPosition);
        addNode(dom,&mainNode,"topMargin",optionsValues.topMargin);
        addNode(dom,&mainNode,"leftMargin",optionsValues.leftMargin);
        addNode(dom,&mainNode,"rightMargin",optionsValues.rightMargin);
        addNode(dom,&mainNode,"bottomMargin",optionsValues.bottomMargin);
        addNode(dom,&mainNode,"defaultOutputFolder",optionsValues.defaultOutputFolder);

        QRect geom = optionsValues.lastGeometry;
        QStringList lastGeomList;
        lastGeomList << QString::number(geom.x()) << QString::number(geom.y()) << QString::number(geom.width()) << QString::number(geom.height());

        addNode(dom,&mainNode,"lastGeometry",lastGeomList.join(","));


        QDomElement colorNode = dom->createElement("colors");
        mainNode.appendChild(colorNode);

        //foreach(const ColorRegExp &colorRegExp, colors){
        foreach(ColorRegExp colorRegExp, optionsValues.colors){
            QDomElement elem = dom->createElement("ColorRegExp");
            elem.setAttribute("regexp",colorRegExp.regExp);
            elem.setAttribute("color",QString("%1,%2,%3").arg(QString::number(colorRegExp.color.red()),QString::number(colorRegExp.color.green()),QString::number(colorRegExp.color.blue())));
            elem.setAttribute("weight",colorRegExp.weight);
            elem.setAttribute("italic",colorRegExp.italic);
            elem.setAttribute("text",colorRegExp.isText);
            elem.setAttribute("caseSensitivity",colorRegExp.caseSensitivity);
            elem.setAttribute("active",colorRegExp.active);
            colorNode.appendChild(elem);
        }

    }
}

void Options::read(QDomDocument *dom, QFileInfo)
{
    QDomElement dom_element = dom->documentElement();
    QDomNode node = dom_element.firstChild();

    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()=="enableColors"){
            optionsValues.enableColors = element.text().toInt();
        }
        if(element.tagName()=="selectNewTab"){
            optionsValues.selectNewTab = element.text().toInt();
        }
        if(element.tagName()=="openReadOnly"){
            optionsValues.openReadOnly = element.text().toInt();
        }
        if(element.tagName()=="reOpenPreviousTabs"){
            optionsValues.reOpenPreviousTabs = element.text().toInt();
        }
        if(element.tagName()=="defaultPath"){
            optionsValues.defaultPath = element.text();
        }
        if(element.tagName()=="defaultOutputFolder"){
            optionsValues.defaultOutputFolder = element.text();
        }
        if(element.tagName()=="openSizeMode"){
            optionsValues.openSizeMode = element.text().toInt();
        }
        if(element.tagName()=="lastSizeMode"){
            optionsValues.lastSizeMode = element.text().toInt();
        }
        if(element.tagName()=="lastGeometry"){
            QStringList list = element.text().split(",");
            if(list.size()==4){
                optionsValues.lastGeometry = QRect(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt(),list.at(3).toInt());
            }
        }
        if(element.tagName()=="openSizeMode"){
            optionsValues.openSizeMode = element.text().toInt();
        }
        if(element.tagName()=="enableColorsOnPrinting"){
            optionsValues.enableColorsOnPrinting = element.text().toInt();
        }
        if(element.tagName()=="printHearderOnEachPages"){
            optionsValues.printHearderOnEachPages = element.text().toInt();
        }
        if(element.tagName()=="mainToolBarPosition"){
            optionsValues.mainToolBarPosition = element.text().toInt();
        }
        if(element.tagName()=="chordSize"){
            optionsValues.chordSize = toSize(element.text());;
        }
        if(element.tagName()=="mainFont"){
            QString fontString = element.text();
            if(!fontString.isEmpty())
                optionsValues.mainFont.fromString(fontString);
        }
        if(element.tagName()=="titleFont"){
            QString fontString = element.text();
            if(!fontString.isEmpty())
                optionsValues.titleFont.fromString(fontString);
        }
        if(element.tagName()=="artistFont"){
            QString fontString = element.text();
            if(!fontString.isEmpty())
                optionsValues.artistFont.fromString(fontString);
        }
        if(element.tagName()=="otherFont"){
            QString fontString = element.text();
            if(!fontString.isEmpty())
                optionsValues.otherFont.fromString(fontString);
        }

        if(element.tagName()=="topMargin"){
            optionsValues.topMargin = element.text().toInt();
        }
        if(element.tagName()=="leftMargin"){
            optionsValues.leftMargin = element.text().toInt();
        }
        if(element.tagName()=="rightMargin"){
            optionsValues.rightMargin = element.text().toInt();
        }
        if(element.tagName()=="bottomMargin"){
            optionsValues.bottomMargin = element.text().toInt();
        }

        if(element.tagName()=="colors"){
            optionsValues.colors.clear();

            QDomNode child = node.firstChild();

            while(!child.isNull()){

                QDomElement colorElement = child.toElement();
                if(colorElement.tagName()=="ColorRegExp"){
                    QString regExp = colorElement.attribute("regexp");
                    QString colorString = colorElement.attribute("color","0,0,0");
                    QString weightText =  colorElement.attribute("weight","50");
                    int isText = colorElement.attribute("text","0").toInt();
                    int isItalic = colorElement.attribute("italic","0").toInt();
                    int caseSensitivity = colorElement.attribute("caseSensitivity","1").toInt();
                    int active = colorElement.attribute("active","1").toInt();

                    QStringList colorList = colorString.split(",");
                    QColor color(0,0,0);
                    if(colorList.size()==3){
                        color.setRgb(colorList[0].toInt(),colorList[1].toInt(),colorList[2].toInt());
                    }

                    ColorRegExp cre(regExp,color,weightText.isEmpty()?50:weightText.toInt(),isItalic,isText,caseSensitivity,active);

                    optionsValues.colors.push_back(cre);
                }

                child =child.nextSibling();
            }
        }

        node = node.nextSibling();
    }

    if(optionsValues.colors.isEmpty())
        setDefaultRegExp();
}

void Options::setDefaultRegExp()
{
    optionsValues.colors.clear();
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]#(?!(m|[1-9]))", Qt::darkRed, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]#[1-9]\\b", Qt::darkRed, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]#m[1-9]\\b", Qt::darkRed, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]#m\\b", Qt::darkRed, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]b\\b", Qt::darkGreen, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]bm\\b", Qt::darkGreen, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G][1-9]\\b", Qt::darkYellow, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]m[1-9]\\b", Qt::darkBlue, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]m\\b", Qt::darkBlue, QFont::Bold)  );
    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G]dim\\b", Qt::darkMagenta, QFont::Bold)  );

    optionsValues.colors.push_back(  ColorRegExp("\\b[A-G](?!(#|'))\\b", Qt::red, QFont::Bold)  );


    optionsValues.colors.push_back(  ColorRegExp("\\bIntro\\b", Qt::darkCyan, QFont::Bold, true, true, Qt::CaseInsensitive)  );
    optionsValues.colors.push_back(  ColorRegExp("\\bChorus\\b", Qt::darkCyan, QFont::Bold, true, true, Qt::CaseInsensitive)  );
    optionsValues.colors.push_back(  ColorRegExp("\\bVerse\\b", Qt::darkCyan, QFont::Bold, true, true, Qt::CaseInsensitive)  );
    optionsValues.colors.push_back(  ColorRegExp("\\bbis\\b", Qt::darkCyan, QFont::Bold, true, true, Qt::CaseInsensitive)  );
    optionsValues.colors.push_back(  ColorRegExp("(arp)", Qt::darkCyan, QFont::Bold, true, true, Qt::CaseInsensitive)  );
}
