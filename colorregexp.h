#ifndef COLORREGEXP_H
#define COLORREGEXP_H

#include <QString>
#include <QColor>

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

#endif // COLORREGEXP_H
