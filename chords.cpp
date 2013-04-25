#include "chords.h"

Chords::Chords(QWidget *parent) : parent_(parent)
{
    parse();
}

void Chords::init()
{
    chords.clear();
    Instrument guitar = addInstrument(tr("Guitar"),"guitar",6);
    Instrument bass = addInstrument(tr("Bass"),"bass",4);
    Instrument ukulele = addInstrument(tr("Ukulele"),"ukulele",4);
    Instrument mandolin = addInstrument(tr("Mandolin"),"mandolin",8);
    Instrument banjo = addInstrument(tr("Banjo"),"banjo",5);
    addChord(guitar,"A","0,0,2,2,2,0");
    addChord(guitar,"B","2,2,4,4,4,2");
    addChord(guitar,"C","0,3,2,0,1,0");
    addChord(guitar,"D","0,0,0,2,3,2");
    addChord(guitar,"E","0,2,2,1,0,0");
    addChord(guitar,"F","1,3,3,2,1,1");
    addChord(guitar,"G","3,2,0,0,0,3");
    addChord(guitar,"Am","0,0,2,2,1,0");
    addChord(guitar,"Bm","2,2,4,4,3,2");
    addChord(guitar,"Cm","X,3,5,5,4,3");
    addChord(guitar,"Dm","0,0,0,2,3,1");
    addChord(guitar,"Em","0,2,2,0,0,0");
    addChord(guitar,"Fm","1,3,3,1,1,1");
    addChord(guitar,"Gm","3,5,5,3,3,3");
    addChord(guitar,"A#","X,1,3,3,3,1");
    addChord(guitar,"C#","X,4,6,6,6,4");
    addChord(guitar,"D#","X,6,8,8,8,6");
    addChord(guitar,"F#","2,4,4,3,2,2");
    addChord(guitar,"G#","4,6,6,5,4,4");
    addChord(guitar,"Bb","X,1,3,3,3,1");
    addChord(guitar,"Db","X,4,6,6,6,4");
    addChord(guitar,"Eb","X,6,8,8,8,6");
    addChord(guitar,"Gb","2,4,4,3,2,2");
    addChord(guitar,"Ab","4,6,6,5,4,4");
    addChord(guitar,"A#m","X,1,3,3,2,1");
    addChord(guitar,"C#m","X,4,6,6,5,4");
    addChord(guitar,"D#m","X,6,8,8,7,6");
    addChord(guitar,"F#m","2,4,4,2,2,2");
    addChord(guitar,"G#m","4,6,6,4,4,4");
    addChord(guitar,"A","5,7,7,6,5,5");
    addChord(guitar,"B","7,9,9,8,7,7");
    addChord(guitar,"C","X,3,5,5,5,3");
    addChord(guitar,"C","8,10,10,9,8,8");
    addChord(guitar,"D","X,5,7,7,7,5");
    addChord(guitar,"E","X,7,9,9,9,7");
    addChord(guitar,"F","X,8,10,10,10,8");
    addChord(guitar,"G","3,5,5,4,3,3");
    addChord(guitar,"Am","5,7,7,5,5,5");
    addChord(guitar,"Cm","3,3,5,5,4,3");
    addChord(guitar,"Dm","5,5,7,7,6,5");
    addChord(guitar,"Em","7,7,9,9,8,7");
    addChord(guitar,"Gm","3,5,5,3,3,3");
}

void Chords::parse()
{
    chords.clear();
    QFile xml_doc("chords.xml");
    if(!xml_doc.exists()){
        init();
        save();
        return;
    }

    if(!xml_doc.open(QIODevice::ReadOnly)){
        QMessageBox::warning(parent_,parent_->tr("Failed to open XML document"),parent_->tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(xml_doc.fileName()));
        return ;
    }

    QDomDocument *dom = new QDomDocument("docXML");
    if (!dom->setContent(&xml_doc)){
        xml_doc.close();
        QMessageBox::warning(parent_,parent_->tr("Error opening the XML document"),parent_->tr("The XML document could not be assigned to the object QDomDocument."));
        return ;
    }

    QDomElement dom_element = dom->documentElement();
    if(dom_element.tagName()=="chords"){
        QDomNode node = dom_element.firstChild();
        while(!node.isNull()){
            QDomElement element = node.toElement();
            if(element.tagName()=="instrument"){
                QString instrumentName = element.attribute("name");
                QString instrumentLabel = element.attribute("label");
                int nbStrings = element.attribute("strings").toInt();
                Instrument instrument = addInstrument(instrumentName,instrumentLabel,nbStrings);

                QDomNode instrumentNode = element.firstChild();
                while(!instrumentNode.isNull()){
                    QDomElement instrumentElement = instrumentNode.toElement();
                    if(instrumentElement.tagName()=="chord"){
                        QString name = instrumentElement.attribute("name","");
                        QString fingers = instrumentElement.attribute("fingers","");
                        QString comment = instrumentElement.attribute("comment","");
                        if(!name.isEmpty()){
                            addChord(instrument,name,fingers,comment);
                        }
                    }
                    instrumentNode = instrumentNode.nextSibling();
                }
            }
            node = node.nextSibling();
        }
    }

    xml_doc.close();
}

void Chords::save()
{
    QFile file("chords.xml");
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);

    QDomDocument dom;

    QDomElement mainNode = dom.createElement("chords");
    dom.appendChild(mainNode);

    {
        QList<Instrument> instruments = chords.keys();
        for(int i=0;i<instruments.size();i++){
            Instrument instrument = instruments[i];

            QDomElement instrumentNode = dom.createElement("instrument");
            instrumentNode.setAttribute("name",instrument.name);
            instrumentNode.setAttribute("label",instrument.label);
            instrumentNode.setAttribute("strings",instrument.nbStrings);
            mainNode.appendChild(instrumentNode);

            foreach(Chord chord, chords[instrument]){
                QDomElement elem = dom.createElement("chord");
                elem.setAttribute("name",chord.name);
                if(!chord.fingers.isEmpty())
                    elem.setAttribute("fingers",chord.fingers);
                if(!chord.comment.isEmpty())
                    elem.setAttribute("comment",chord.comment);
                instrumentNode.appendChild(elem);
            }
        }
    }

    stream << dom.toString();

    file.close();
}

void Chords::addChord(Instrument instrument, QString name, QString fingers, QString comment)
{
    Chord chord(name,fingers,comment);
    chords[instrument].push_back(chord);
}

Instrument Chords::addInstrument(QString name, QString label, int nbStrings)
{
    Instrument instrument(name,label,nbStrings);
    if(!chords.contains(instrument)){
        chords.insert(instrument,QList<Chord>());
    }
    return instrument;
}

QStringList Chords::getInstrumentsNames()
{
    QList<Instrument> instumentsList = chords.keys();
    QStringList instruments;
    for(int i=0;i<instumentsList.size();i++)
        instruments << instumentsList[i].name;
    return instruments;
}

QList<Instrument> Chords::getInstruments()
{
    return chords.keys();
}

QList<Chord> Chords::getChords(Instrument instrument)
{
    if(chords.contains(instrument)){
        return chords[instrument];
    }else{
        return QList<Chord>();
    }
}

QList<Chord> Chords::getChords(Instrument instrument, QString name)
{
    if(chords.contains(instrument)){
        QList<Chord> list = chords[instrument];
        QList<Chord> out;
        for(int i=0;i<list.size();i++)
            if(list[i].name==name)
                out << list[i];
        return out;
    }else{
        return QList<Chord>();
    }
}

QString Chords::getFingers(Instrument instrument, QString name)
{
    QList<Chord> list = getChords(instrument,name);
    if(list.isEmpty()) return "";
    return list[0].fingers;
}
