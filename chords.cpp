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
    addLockedChord(guitar,"A","0,0,2,2,2,0");
    addLockedChord(guitar,"B","2,2,4,4,4,2");
    addLockedChord(guitar,"C","0,3,2,0,1,0");
    addLockedChord(guitar,"D","0,0,0,2,3,2");
    addLockedChord(guitar,"E","0,2,2,1,0,0");
    addLockedChord(guitar,"F","1,3,3,2,1,1");
    addLockedChord(guitar,"G","3,2,0,0,0,3");
    addLockedChord(guitar,"Am","0,0,2,2,1,0");
    addLockedChord(guitar,"Bm","2,2,4,4,3,2");
    addLockedChord(guitar,"Cm","X,3,5,5,4,3");
    addLockedChord(guitar,"Dm","0,0,0,2,3,1");
    addLockedChord(guitar,"Em","0,2,2,0,0,0");
    addLockedChord(guitar,"Fm","1,3,3,1,1,1");
    addLockedChord(guitar,"Gm","3,5,5,3,3,3");
    addLockedChord(guitar,"A#","X,1,3,3,3,1");
    addLockedChord(guitar,"C#","X,4,6,6,6,4");
    addLockedChord(guitar,"D#","X,6,8,8,8,6");
    addLockedChord(guitar,"F#","2,4,4,3,2,2");
    addLockedChord(guitar,"G#","4,6,6,5,4,4");
    addLockedChord(guitar,"Bb","X,1,3,3,3,1");
    addLockedChord(guitar,"Db","X,4,6,6,6,4");
    addLockedChord(guitar,"Eb","X,6,8,8,8,6");
    addLockedChord(guitar,"Gb","2,4,4,3,2,2");
    addLockedChord(guitar,"Ab","4,6,6,5,4,4");
    addLockedChord(guitar,"A#m","X,1,3,3,2,1");
    addLockedChord(guitar,"C#m","X,4,6,6,5,4");
    addLockedChord(guitar,"D#m","X,6,8,8,7,6");
    addLockedChord(guitar,"F#m","2,4,4,2,2,2");
    addLockedChord(guitar,"G#m","4,6,6,4,4,4");
    addLockedChord(guitar,"A","5,7,7,6,5,5");
    addLockedChord(guitar,"B","7,9,9,8,7,7");
    addLockedChord(guitar,"C","X,3,5,5,5,3");
    addLockedChord(guitar,"C","8,10,10,9,8,8");
    addLockedChord(guitar,"D","X,5,7,7,7,5");
    addLockedChord(guitar,"E","X,7,9,9,9,7");
    addLockedChord(guitar,"F","X,8,10,10,10,8");
    addLockedChord(guitar,"G","3,5,5,4,3,3");
    addLockedChord(guitar,"Am","5,7,7,5,5,5");
    addLockedChord(guitar,"Cm","3,3,5,5,4,3");
    addLockedChord(guitar,"Dm","5,5,7,7,6,5");
    addLockedChord(guitar,"Em","7,7,9,9,8,7");
    addLockedChord(guitar,"Gm","3,5,5,3,3,3");

    Instrument ukulele = addInstrument(tr("Ukulele"),"ukulele",4);
    addLockedChord(ukulele,"A","2,1,0,0");
    addLockedChord(ukulele,"B","4,3,2,2");
    addLockedChord(ukulele,"C","0,0,0,3");
    addLockedChord(ukulele,"D","2,2,2,0");
    addLockedChord(ukulele,"E","1,4,0,2");
    addLockedChord(ukulele,"F","2,0,1,0");
    addLockedChord(ukulele,"G","0,2,3,2");
    addLockedChord(ukulele,"Am","2,0,0,0");
    addLockedChord(ukulele,"Bm","4,2,2,2");
    addLockedChord(ukulele,"Cm","0,3,3,3");
    addLockedChord(ukulele,"Dm","2,2,1,0");
    addLockedChord(ukulele,"Em","0,4,3,2");
    addLockedChord(ukulele,"Fm","1,0,1,3");
    addLockedChord(ukulele,"Gm","0,2,3,1");
    addLockedChord(ukulele,"A#","3,2,1,1");
    addLockedChord(ukulele,"C#","1,1,1,4");
    addLockedChord(ukulele,"D#","0,3,3,1");
    addLockedChord(ukulele,"F#","3,1,2,1");
    addLockedChord(ukulele,"G#","1,3,4,3");
    addLockedChord(ukulele,"Bb","3,2,1,1");
    addLockedChord(ukulele,"Db","1,1,1,4");
    addLockedChord(ukulele,"Eb","0,3,3,1");
    addLockedChord(ukulele,"Gb","3,1,2,1");
    addLockedChord(ukulele,"Ab","1,3,4,3");
    addLockedChord(ukulele,"A#m","3,1,1,1");
    addLockedChord(ukulele,"C#m","1,1,0,4");
    addLockedChord(ukulele,"D#m","3,3,2,1");
    addLockedChord(ukulele,"F#m","2,1,2,0");
    addLockedChord(ukulele,"G#m","1,3,4,2");
    addLockedChord(ukulele,"Bbm","3,1,1,1");
    addLockedChord(ukulele,"Dbm","1,1,0,4");
    addLockedChord(ukulele,"Ebm","3,3,2,1");
    addLockedChord(ukulele,"Gbm","2,1,2,0");
    addLockedChord(ukulele,"Abm","1,3,4,2");

    Instrument mandolin = addInstrument(tr("Mandolin"),"mandolin",4);
    addLockedChord(mandolin,"A","9,7,4,5");
    addLockedChord(mandolin,"A","2,2,4,5");
    addLockedChord(mandolin,"Am","5,7,7,0");
    addLockedChord(mandolin,"Am","2,2,3,5");
    addLockedChord(mandolin,"Am","9,7,3,5");
    addLockedChord(mandolin,"A7","6,5,7,5");
    addLockedChord(mandolin,"Bb","11,9,6,7");

    Instrument banjo = addInstrument(tr("Banjo"),"banjo",5);
    addLockedChord(banjo,"A","0,2,2,2,2");
    addLockedChord(banjo,"B","0,4,4,4,4");
    addLockedChord(banjo,"C","0,2,0,1,3");
    addLockedChord(banjo,"D","0,4,2,3,4");
    addLockedChord(banjo,"E","0,2,1,0,2");
    addLockedChord(banjo,"F","0,3,2,1,3");
    addLockedChord(banjo,"G","0,0,0,0,0");

    //Instrument bass = addInstrument(tr("Bass"),"bass",4);
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
                if(chord.isLocked())
                    continue;
                QDomElement elem = dom->createElement("chord");
                elem.setAttribute("name",chord.getName());
                if(!chord.getFingers().isEmpty())
                    elem.setAttribute("fingers",chord.getFingers());
                if(!chord.getComment().isEmpty())
                    elem.setAttribute("comment",chord.getComment());
                instrumentNode.appendChild(elem);
            }
        }
    }
}

void Chords::addLockedChord(Instrument instrument, QString name, QString fingers, QString comment)
{
    chords[instrument].push_back(Chord(name,fingers,comment));
}

void Chords::addChord(Instrument instrument, QString name, QString fingers, QString comment, bool locked)
{
    if(locked)
        chords[instrument].push_back(Chord(name,fingers,comment));
    else
        chords[instrument].push_back(ChordUnlocked(name,fingers,comment));
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
            if(list[i].getName()==name)
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
    return list[0].getFingers();
}

void Chords::load()
{
    if(QFile::exists(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "chords.xml"))
        parse(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "chords.xml");
}

void Chords::save()
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if(!dir.exists()){
        dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    }
    flush(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "chords.xml");
}
