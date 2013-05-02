#ifndef FONTLABELLAYOUT_H
#define FONTLABELLAYOUT_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QFontDialog>

class FontLabelLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    FontLabelLayout(QWidget *parent = 0) : QHBoxLayout(parent)
    {
        QToolButton *toolFont = new QToolButton;
        toolFont->setIcon(QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fonttruetype-16.png")));
        label = new QLabel;
        label->setFrameStyle(QFrame::Sunken | QFrame::Panel);

        this->addWidget(toolFont);
        this->addWidget(label);

        connect(toolFont,SIGNAL(clicked()),this,SLOT(selectFont()));
    }
    QFont getFont() { return font_; }

    void setFont(QFont font)
    {
        font_ = font;
        label->setText(font_.toString());
        label->setFont(font_);
        emit this->fontChanged( font );
    }

private:
    QLabel *label;
    QFont font_;

signals:
    void fontChanged(QFont font);

public slots:
    void selectFont()
    {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, font_, this->widget());
        if(ok){
            setFont( font );
        }
    }
};

#endif // FONTLABELLAYOUT_H
