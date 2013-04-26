#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

struct ColorRegExp
{
    ColorRegExp(QString _regExp, QColor _color, int _weight, int _italic = false, int _isText = false, int _caseSensitivity = true, int _active = true){
        regExp = _regExp;
        color = _color;
        weight = _weight;
        isText = _isText;
        italic = _italic;
        caseSensitivity = _caseSensitivity;
        active = _active;
    }

    QString regExp;
    QColor color;
    int weight;
    int isText;
    int caseSensitivity;
    int active;
    int italic;
};

struct OptionsValues
{
    OptionsValues();

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

    //Functions
    void setDefaultRegExp();
    void save(QWidget* parent = 0);
    void parse(QWidget* parent = 0);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data);

    QSize toSize(QString str){
        QSize size;
        size.setWidth( str.section("x",0,0).toInt() );
        size.setHeight( str.section("x",1).toInt() );
        return size;
    }
    QString fromSize(QSize size){
        return QString::number(size.width()) + "x" + QString::number(size.height());
    }
};

#endif // OPTIONS_H
