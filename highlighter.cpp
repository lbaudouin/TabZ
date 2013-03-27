/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

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
    keywordPatterns << "\\bA(?!#)\\b" << "\\bB(?!#)\\b" << "\\bC(?!(#|'))\\b" << "\\bD(?!#)\\b" << "\\bE(?!#)\\b" << "\\bF(?!#)\\b" << "\\bG(?!#)\\b";
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
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
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
