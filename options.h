#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

struct ColorRegExp
{
    ColorRegExp(QString _regExp, QColor _color, int _weight, int _isText = false, int _caseSensitivity = 1){
        regExp = _regExp;
        color = _color;
        weight = _weight;
        isText = _isText;
        caseSensitivity = _caseSensitivity;
    }

    QString regExp;
    QColor color;
    int weight;
    int isText;
    int caseSensitivity;
};

struct OptionsValues
{
    OptionsValues();
    bool enableColors;
    bool selectNewTab;
    bool openReadOnly;
    QString defaultPath;
    int openSizeMode;

    QList<ColorRegExp> colors;

    void setDefaultRegExp();

    void save(QWidget* parent = 0);
    void parse(QWidget* parent = 0);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data);
};

#endif // OPTIONS_H
