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
    Highlighter(QTextDocument *parent = 0);

    QStringList matches();
    void addRule(QStringList list, QColor color, QFont::Weight weight);

    QStringList getList(QString text);

    void clear();
    void reset();

protected:
    void highlightBlock(const QString &text);
    void setDefaultRules();


private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;

    QStringList found;

    bool enabled_;

public slots:
    void enable();
    void disable();
    void setEnabled(bool);
};
//! [0]

#endif
