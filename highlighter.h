#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QtGui>
#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

#include <QDebug>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
        bool text;
    };

    Highlighter(QTextDocument *parent = 0);

    QStringList matches();
    void addRule(QStringList list, QColor color, int weight = QFont::Normal, bool italic = false, bool isText = false, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    void addRule(QString text, QColor color, int weight = QFont::Normal, bool italic = false, bool isText = false, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);

    QStringList getList(QString text);

    void clear();

    QVector<HighlightingRule> getRules();
    void setRules(QVector<HighlightingRule> rules);

    void addPersonalText(QStringList list) {
        for(int i=0;i<list.size();i++){
            personal << "\\b" + list[i] + "\\b";
        }
        //personal << list; }
    }
    void addPersonalRegExp(QStringList list) {
        personal << list;
    }
    void resetPersonal() { personal.clear(); }
    QStringList getPersonalRegExp(){
        return personal;
    }

protected:
    void highlightBlock(const QString &text);


private:
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;

    QStringList personal;
    HighlightingRule personalRule;

    bool enabled_;

public slots:
    void enable();
    void disable();
    void setEnabled(bool);

    void update();

};
//! [0]

#endif
