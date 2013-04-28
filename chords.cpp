#include "chords.h"

Chords::Chords(QWidget *parent) : ReadWriteXML(parent)
{
    init();
    load();
}

void Chords::init()
{
    chords.clear();
    Instrument guitar = addInstrument(tr("Guitar"),"guitar",6);
    Instrument bass = addInstrument(tr("Bass"),"bass",4);
    Instrument ukulele = addInstrument(tr("Ukulele"),"ukulele",4);
    Instrument mandolin = addInstrument(tr("Mandolin"),"mandolin",8);
    Instrument banjo = addInstrument(tr("Banjo"),"banjo",5);
    addChord(guitar,"A","0,0,2,2,2,0","",true);
    addChord(guitar,"B","2,2,4,4,4,2","",true);
    addChord(guitar,"C","0,3,2,0,1,0","",true);
    addChord(guitar,"D","0,0,0,2,3,2","",true);
    addChord(guitar,"E","0,2,2,1,0,0","",true);
    addChord(guitar,"F","1,3,3,2,1,1","",true);
    addChord(guitar,"G","3,2,0,0,0,3","",true);
    addChord(guitar,"Am","0,0,2,2,1,0","",true);
    addChord(guitar,"Bm","2,2,4,4,3,2","",true);
    addChord(guitar,"Cm","X,3,5,5,4,3","",true);
    addChord(guitar,"Dm","0,0,0,2,3,1","",true);
    addChord(guitar,"Em","0,2,2,0,0,0","",true);
    addChord(guitar,"Fm","1,3,3,1,1,1","",true);
    addChord(guitar,"Gm","3,5,5,3,3,3","",true);
    addChord(guitar,"A#","X,1,3,3,3,1","",true);
    addChord(guitar,"C#","X,4,6,6,6,4","",true);
    addChord(guitar,"D#","X,6,8,8,8,6","",true);
    addChord(guitar,"F#","2,4,4,3,2,2","",true);
    addChord(guitar,"G#","4,6,6,5,4,4","",true);
    addChord(guitar,"Bb","X,1,3,3,3,1","",true);
    addChord(guitar,"Db","X,4,6,6,6,4","",true);
    addChord(guitar,"Eb","X,6,8,8,8,6","",true);
    addChord(guitar,"Gb","2,4,4,3,2,2","",true);
    addChord(guitar,"Ab","4,6,6,5,4,4","",true);
    addChord(guitar,"A#m","X,1,3,3,2,1","",true);
    addChord(guitar,"C#m","X,4,6,6,5,4","",true);
    addChord(guitar,"D#m","X,6,8,8,7,6","",true);
    addChord(guitar,"F#m","2,4,4,2,2,2","",true);
    addChord(guitar,"G#m","4,6,6,4,4,4","",true);
    addChord(guitar,"A","5,7,7,6,5,5","",true);
    addChord(guitar,"B","7,9,9,8,7,7","",true);
    addChord(guitar,"C","X,3,5,5,5,3","",true);
    addChord(guitar,"C","8,10,10,9,8,8","",true);
    addChord(guitar,"D","X,5,7,7,7,5","",true);
    addChord(guitar,"E","X,7,9,9,9,7","",true);
    addChord(guitar,"F","X,8,10,10,10,8","",true);
    addChord(guitar,"G","3,5,5,4,3,3","",true);
    addChord(guitar,"Am","5,7,7,5,5,5","",true);
    addChord(guitar,"Cm","3,3,5,5,4,3","",true);
    addChord(guitar,"Dm","5,5,7,7,6,5","",true);
    addChord(guitar,"Em","7,7,9,9,8,7","",true);
    addChord(guitar,"Gm","3,5,5,3,3,3","",true);
}

void Chords::read(QDomDocument *dom, QFileInfo)
{
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
}

void Chords::write(QDomDocument *dom, QFileInfo)
{
    QDomElement mainNode = dom->createElement("chords");
    dom->appendChild(mainNode);

    {
        QList<Instrument> instruments = chords.keys();
        for(int i=0;i<instruments.size();i++){
            Instrument instrument = instruments[i];

            QDomElement instrumentNode = dom->createElement("instrument");
            instrumentNode.setAttribute("name",instrument.name);
            instrumentNode.setAttribute("label",instrument.label);
            instrumentNode.setAttribute("strings",instrument.nbStrings);
            mainNode.appendChild(instrumentNode);

            foreach(Chord chord, chords[instrument]){
                if(chord.locked)
                    continue;
                QDomElement elem = dom->createElement("chord");
                elem.setAttribute("name",chord.name);
                if(!chord.fingers.isEmpty())
                    elem.setAttribute("fingers",chord.fingers);
                if(!chord.comment.isEmpty())
                    elem.setAttribute("comment",chord.comment);
                instrumentNode.appendChild(elem);
            }
        }
    }
}

void Chords::addChord(Instrument instrument, QString name, QString fingers, QString comment, bool locked)
{
    Chord chord(name,fingers,comment,locked);
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

Instrument Chords::getInstrument(QString label)
{
    QList<Instrument> instrumentsList = chords.keys();
    for(int i=0;i<instrumentsList.size();i++){
        if(instrumentsList[i].label==label)
            return instrumentsList[i];
    }
    return addInstrument(label,label,6);
}

QString Chords::getInstrumentName(QString label)
{
    return getInstrument(label).name;
}

QStringList Chords::getInstrumentsNames()
{
    QList<Instrument> instrumentsList = chords.keys();
    QStringList instruments;
    for(int i=0;i<instrumentsList.size();i++)
        instruments << instrumentsList[i].name;
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

void Chords::load()
{
    if(QFile::exists("chords.xml"))
        parse("chords.xml");
}

void Chords::save()
{
    flush("chords.xml");
}
