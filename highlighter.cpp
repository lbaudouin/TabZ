#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), enabled_(true)
{
    setDefaultRules();
}

void Highlighter::setDefaultRules()
{
    QStringList keywordPatterns;

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]#(?!m)";
    addRule(keywordPatterns,Qt::red,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]#m[1-9]";
    addRule(keywordPatterns,QColor(255,125,0),QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]#m\\b";
    addRule(keywordPatterns,Qt::red,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]#[1-9]";
    addRule(keywordPatterns,Qt::green,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]b\\b";
    addRule(keywordPatterns,Qt::darkCyan,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G][1-9]\\b";
    addRule(keywordPatterns,Qt::darkYellow,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G](?!(#|'|\\|))\\b";
    addRule(keywordPatterns,Qt::darkBlue,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]m\\b";
    addRule(keywordPatterns,Qt::darkGreen,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]m[1-9]\\b";
    addRule(keywordPatterns,Qt::blue,QFont::Bold);

    keywordPatterns.clear();
    keywordPatterns << "\\b[A-G]dim\\b";
    addRule(keywordPatterns,Qt::magenta,QFont::Bold);
}

void Highlighter::clear()
{
    highlightingRules.clear();
}

void Highlighter::reset()
{
    clear();
    setDefaultRules();
}

QStringList Highlighter::getList(QString text)
{
    QStringList list;

    if(highlightingRules.isEmpty())
        return list;

    QStringList lines = text.split("\n");

    foreach(QString line, lines){
        int lenght_total = 0;

        QStringList temp;

        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(line);
            while (index >= 0) {
                int length = expression.matchedLength();
                temp << line.mid(index,length);
                lenght_total += length;
                index = expression.indexIn(line, index + length);
            }
        }
        if(lenght_total==0)
            continue;

        temp.removeDuplicates();

        QString chords = line;
        chords.remove(QRegExp("[0-9]"));
        chords.remove("x",Qt::CaseInsensitive);
        chords.remove("-");
        chords.remove("|");
        chords.remove(" ");
        chords.remove("\t");

        if(chords.contains("(")){
            chords = chords.remove( QRegExp( "\\(.*\\)" ) );
        }

        int nb_char = chords.size();

        //If there is more than 0.5 of total characters for chords then, paint them
        if(lenght_total>=0.5*nb_char){
            list << temp;
        }
    }
    list.removeDuplicates();
    return list;
}


void Highlighter::highlightBlock(const QString &text)
{
    if(!enabled_)
        return;

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
    chords.remove(QRegExp("[0-9]"));
    chords.remove("x",Qt::CaseInsensitive);
    chords.remove(" ");
    chords.remove("-");
    chords.remove("|");
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

void Highlighter::enable()
{
    enabled_ = true;
    this->rehighlight();
}

void Highlighter::disable()
{
    enabled_ = false;
    this->rehighlight();
}

void Highlighter::setEnabled(bool state)
{
    enabled_ = state;
    this->rehighlight();
}
