#ifndef OPTIONS_H
#define OPTIONS_H

#include "readwritexml.h"
#include "colorregexp.h"

struct OptionsValues
{
    //Values
    bool enableColors;
    bool selectNewTab;
    bool openReadOnly;
    QString defaultPath;
    int openSizeMode;
    bool reOpenPreviousTabs;
    QFont font;
    QSize chordSize;
    bool enableColorsOnPrinting;
    bool printHearderOnEachPages;
    int mainToolBarPosition;
    int topMargin;
    int leftMargin;
    int rightMargin;
    int bottomMargin;

    QList<ColorRegExp> colors;
};

class Options : public ReadWriteXML
{
public:
    Options(QWidget *parent = 0);
    void resetDefault();

    OptionsValues values() { return optionsValues; }
    void setValues(OptionsValues _optionsValues) { optionsValues = _optionsValues; }

protected:
    virtual void read(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());
    virtual void write(QDomDocument *dom, QFileInfo fileInfo = QFileInfo());

    void setDefaultRegExp();

private:
    OptionsValues optionsValues;

public:
    static QSize toSize(QString str){
        QSize size;
        size.setWidth( str.section("x",0,0).toInt() );
        size.setHeight( str.section("x",1).toInt() );
        return size;
    }
    static QString fromSize(QSize size){
        return QString::number(size.width()) + "x" + QString::number(size.height());
    }

};

#endif // OPTIONS_H
