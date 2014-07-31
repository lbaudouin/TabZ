#include "recent.h"

Recent::Recent(QWidget *parent) : ReadWriteXML(parent)
{
}

void Recent::read(QDomDocument *dom, QFileInfo)
{
    recentList_.clear();
    QDomElement dom_element = dom->documentElement();
    if(dom_element.tagName()=="recent"){
        QDomNode node = dom_element.firstChild();

        while(!node.isNull())
        {
            QDomElement element = node.toElement();

            if(element.tagName()=="file"){
                RecentFile rf;
                rf.title = element.attribute("title",tr("empty_title"));
                rf.artist = element.attribute("artist",tr("empty_artist"));
                rf.path = element.attribute("path","");
                rf.date = QDateTime::fromString(element.attribute("date",""),"dd MM yy HH mm ss");
                rf.wasOpen = element.attribute("wasOpen","false")=="true";
                recentList_.push_back(rf);
            }
            node = node.nextSibling();
        }
    }
}

void Recent::write(QDomDocument *dom, QFileInfo)
{
    QDomElement mainNode = dom->createElement("recent");
    dom->appendChild(mainNode);

    foreach(RecentFile recentFile, recentList_){
        QDomElement elem = dom->createElement("file");
        elem.setAttribute("title",recentFile.title);
        elem.setAttribute("artist",recentFile.artist);
        elem.setAttribute("path",recentFile.path);
        elem.setAttribute("date",recentFile.date.toString("dd MM yy HH mm ss"));
        if(recentFile.wasOpen)
            elem.setAttribute("wasOpen","true");
        mainNode.appendChild(elem);
    }
}

QList<RecentFile> Recent::load()
{
    recentList_.clear();
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if(QFile::exists(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "recent.xml"))
        parse(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "recent.xml");
#else
    if(QFile::exists(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "recent.xml"))
        parse(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "recent.xml");
#endif
    return recentList_;
}

void Recent::save(QList<RecentFile> &recentList)
{
    recentList_ = recentList;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if(!dir.exists()){
        dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    }
    flush(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + QDir::separator() + "recent.xml");
#else
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if(!dir.exists()){
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    }
    flush(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "recent.xml");
#endif
}
