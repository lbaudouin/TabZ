#ifndef RECENT_H
#define RECENT_H

#include "readwritexml.h"
#include <QDateTime>

struct RecentFile{
    QString title;
    QString artist;
    QString path;
    QDateTime date;
    bool wasOpen;


    QString getName() const {
        QString name;
        if(title.isEmpty()){
            name = path;
        }else{
            if(artist.isEmpty()){
                name = title;
            }else{
                name = artist + " - " + title;
            }
        }
        return name;
    }

    /*friend QDebug& operator<<( QDebug &stream, const RecentFile &info)
    {
        stream << info.date << info.getName();
        return stream;
    }*/
};

struct RecentDateComparer
{
    bool operator()(const RecentFile & a, const RecentFile & b) const
    {
        return a.date > b.date;
    }
};

struct RecentTitleComparer
{
    bool operator()(const RecentFile & a, const RecentFile & b) const
    {
        return a.title < b.title;
    }
};

struct RecentNameComparer
{
    bool operator()(const RecentFile & a, const RecentFile & b) const
    {
        return a.getName() < b.getName();
    }
};

class Recent : public ReadWriteXML
{
    Q_OBJECT
public:
    Recent(QWidget *parent = 0);

    QList<RecentFile> load(QString filepath);
    void save(QString filepath, QList<RecentFile> &recentList);


protected:
    virtual void read(QDomDocument *dom, QFileInfo fileInfo);
    virtual void write(QDomDocument *dom, QFileInfo fileInfo);

private:
    QList<RecentFile> recentList_;
};

#endif // RECENT_H
