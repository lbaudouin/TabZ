#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), enabled_(true)
{

}

void Highlighter::clear()
{
    highlightingRules.clear();
}

void Highlighter::update()
{
    rehighlight();
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
                if(!rule.text){
                    temp << line.mid(index,length);
                    lenght_total += length;
                }
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

        int nbDash = line.count("-");

        int nbChar = chords.size();

        //If there is more than 0.5 of total characters for chords then, paint them
        if(lenght_total>=0.5*nbChar && nbDash<0.5*nbChar){
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
            if(rule.text){
                setFormat(index, length, rule.format);
            }else{
                lenght_total += length;
            }
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

    int nbDash = text.count("-");
    int nbChar = chords.size();

    //If there is more than 0.5 of total characters for chords then, paint them
    if(lenght_total>=0.5*nbChar && nbDash<0.5*nbChar){
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

void Highlighter::addRule(QStringList list, QColor color, int weight, bool italic, bool isText, Qt::CaseSensitivity caseSensitivity)
{
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(weight);
    keywordFormat.setFontItalic(italic);
    rule.text = isText;


    foreach (const QString &pattern, list) {
        if(pattern.isEmpty())
            continue;
        rule.pattern = QRegExp(pattern);
        rule.pattern.setCaseSensitivity(caseSensitivity);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void Highlighter::addRule(QString text, QColor color, int weight, bool italic, bool isText, Qt::CaseSensitivity caseSensitivity)
{
    HighlightingRule rule;
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(color);
    keywordFormat.setFontWeight(weight);
    keywordFormat.setFontItalic(italic);
    rule.text = isText;
    rule.pattern = QRegExp(text);
    rule.pattern.setCaseSensitivity(caseSensitivity);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
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

QVector<Highlighter::HighlightingRule> Highlighter::getRules()
{
    return highlightingRules;
}

void Highlighter::setRules(QVector<Highlighter::HighlightingRule> rules)
{
    highlightingRules = rules;
}
