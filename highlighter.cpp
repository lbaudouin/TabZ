#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QStringList keywordPatterns;

    keywordPatterns.clear();
    keywordPatterns << "\\bA#" << "\\bB#" << "\\bC#" << "\\bD#" << "\\bE#" << "\\bF#" << "\\bG#";
    addRule(keywordPatterns,Qt::darkMagenta,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bA#m" << "\\bB#m" << "\\bC#m" << "\\bD#m" << "\\bE#m" << "\\bF#m" << "\\bG#m";
    addRule(keywordPatterns,Qt::red,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bA#7" << "\\bB#7" << "\\bC#7" << "\\bD#7" << "\\bE#7" << "\\bF#7" << "\\bG#7";
    addRule(keywordPatterns,Qt::green,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bAb\\b" << "\\bBb\\b" << "\\bCb\\b" << "\\bDb\\b" << "\\bEb\\b" << "\\bFb\\b" << "\\bGb\\b";
    addRule(keywordPatterns,Qt::darkCyan,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bA7\\b" << "\\bB7\\b" << "\\bC7\\b" << "\\bD7\\b" << "\\bE7\\b" << "\\bF7\\b" << "\\bG7\\b";
    addRule(keywordPatterns,Qt::darkYellow,QFont::Bold);


    keywordPatterns.clear();
    keywordPatterns << "\\bA(?!#)\\b" << "\\bB(?!#)\\b" << "\\bC(?!(#|'))\\b" << "\\bD(?!(#|'))\\b" << "\\bE(?!#)\\b" << "\\bF(?!#)\\b" << "\\bG(?!#)\\b";
    addRule(keywordPatterns,Qt::darkBlue,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bAm\\b" << "\\bBm\\b" << "\\bCm\\b" << "\\bDm\\b" << "\\bEm\\b" << "\\bFm\\b" << "\\bGm\\b";
    addRule(keywordPatterns,Qt::darkGreen,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\bAdim\\b" << "\\bBdim\\b" << "\\bCdim\\b" << "\\bDdim\\b" << "\\bEdim\\b" << "\\bFdim\\b" << "\\bGdim\\b";
    addRule(keywordPatterns,Qt::magenta,QFont::Bold);
}

void Highlighter::highlightBlock(const QString &text)
{
    int lenght_total = 0;
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            lenght_total += length;
            index = expression.indexIn(text, index + length);
        }
    }
    if(lenght_total==0)
        return;

    QString chords = text;
    chords.remove("0");
    chords.remove("1");
    chords.remove("2");
    chords.remove("3");
    chords.remove("4");
    chords.remove("5");
    chords.remove("6");
    chords.remove("7");
    chords.remove("8");
    chords.remove("9");
    chords.remove("x");
    chords.remove("X");
    chords.remove(" ");
    chords.remove("\t");

    if(chords.contains("(")){
        chords = chords.remove( QRegExp( "\\(.*\\)" ) );
    }

    int nb_char = chords.size();

    //If there is more than 0.5 of total characters for chords then, paint them
    if(lenght_total>=0.5*nb_char){
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            if(index>=0 && !found.contains(rule.pattern.pattern()))
                found << rule.pattern.pattern();
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
        }
    }

    setCurrentBlockState(0);
}

void Highlighter::addRule(QStringList list, QColor color, QFont::Weight weight)
{
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(weight);

    foreach (const QString &pattern, list) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

QStringList Highlighter::matches()
{
    return found;
}
