#ifndef OPTIONSFORM_H
#define OPTIONSFORM_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFontDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QColor>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QLabel>
#include <QTextEdit>

#include "options.h"
#include "highlighter.h"

#include "tools.h"

#include <QDebug>

#include <QColorDialog>

#include <QListWidget>

class ColorRegExpForm : public QWidget
{
    Q_OBJECT
public:
    ColorRegExpForm(){

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        gridLayout = new QGridLayout;
        gridLayout->setAlignment(Qt::AlignHCenter);

        gridLayout->addWidget(new QLabel(tr("Active")),0,0,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Regular Expression")),0,1,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Color")),0,2,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Weight")),0,3,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Italic")),0,4,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Text")),0,5,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Case Sensitive")),0,6,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Delete")),0,7,Qt::AlignHCenter);

        mainLayout->addLayout(gridLayout);

        QToolButton *toolAdd = new QToolButton;
        toolAdd->setIcon( QIcon(":images/add") );
        QToolButton *toolHelp = new QToolButton;
        toolHelp->setIcon( QIcon(":images/help") );
        toolHelp->setEnabled(false);
        QSpacerItem *buttonSpacer = new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Ignored);
        QPushButton *buttonSelect = new QPushButton(tr("Select All"));
        QPushButton *buttonUnselect = new QPushButton(tr("Unselect All"));

        connect(buttonSelect,SIGNAL(clicked()),this,SLOT(selectAll()));
        connect(buttonUnselect,SIGNAL(clicked()),this,SLOT(unselectAll()));
        connect(toolAdd,SIGNAL(clicked()),this,SLOT(addNew()));

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(buttonSelect);
        buttonLayout->addWidget(buttonUnselect);
        buttonLayout->addSpacerItem(buttonSpacer);
        buttonLayout->addWidget(toolHelp);
        buttonLayout->addWidget(toolAdd);

        mainLayout->addLayout(buttonLayout);

        edit = new QTextEdit;
        edit->setMaximumHeight(300);

        mainLayout->addWidget(edit);

        edit->setText("Intro\nA Gm F#7\nB Dm Edim (bis)\n\nChorus\nAm Fm Dm\nThis is a test");

        highlighter = new Highlighter(edit->document());

        QList<QColor> colorsTemp;

        QStringList colorNames = QColor::colorNames();
        for(int i=0;i<colorNames.size();i++){
            QColor c = QColor(colorNames[i]);
            if(!colorsTemp.contains(c)){
                listColors << QPair<QString,QString>(colorNames[i],translateColorName(colorNames[i]));
                colorsTemp << c;
            }
        }
        qSort(listColors.begin(),listColors.end(),QPairSecondStringComparer());

    }

    void addColorRegExp(QString regExp = "", QColor _color = Qt::black, int _weight = QFont::Normal, int italic = false, int isText = false, int caseSensitive = 1, int active = 1)
    {
        int currentRow = gridLayout->rowCount();
        QCheckBox *checkActivate = new QCheckBox;
        QLineEdit *editRegExp = new QLineEdit(regExp);
        QComboBox *comboColor = new QComboBox;
        QComboBox *comboWeight = new QComboBox;
        QCheckBox *checkItalic = new QCheckBox;
        QCheckBox *checkText = new QCheckBox;
        QCheckBox *checkCase = new QCheckBox;
        QToolButton *toolClose = new QToolButton;


        for(int i=0;i<listColors.size();i++){
            QColor color(listColors[i].first);
            QPixmap pixmap(16,16);
            pixmap.fill(color);
            comboColor->addItem(QIcon(pixmap),listColors[i].second,QVariant(listColors[i].first));
            if(color==_color)
                comboColor->setCurrentIndex(i);
        }

        checkActivate->setChecked(active);
        checkText->setChecked(isText);
        checkCase->setChecked(caseSensitive);
        checkItalic->setChecked(italic);


        editRegExp->setMinimumWidth(200);
        editRegExp->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
        comboWeight->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

        QListWidget *m_lw = new QListWidget(this);
        QFont font = comboWeight->font();

        QListWidgetItem *lwi;
        lwi = new QListWidgetItem(tr("Light"));
        font.setWeight(QFont::Light);
        lwi->setFont(font);
        m_lw->addItem(lwi);
        lwi = new QListWidgetItem(tr("Normal"));
        font.setWeight(QFont::Normal);
        lwi->setFont(font);
        m_lw->addItem(lwi);
        lwi = new QListWidgetItem(tr("DemiBold"));
        font.setWeight(QFont::DemiBold);
        lwi->setFont(font);
        m_lw->addItem(lwi);
        lwi = new QListWidgetItem(tr("Bold"));
        font.setWeight(QFont::Bold);
        lwi->setFont(font);
        m_lw->addItem(lwi);
        lwi = new QListWidgetItem(tr("Black"));
        font.setWeight(QFont::Black);
        lwi->setFont(font);
        m_lw->addItem(lwi);

        comboWeight->setModel(m_lw->model());
        comboWeight->setView(m_lw);
        comboWeight->setCurrentIndex(-1);

        toolClose->setIcon( QIcon(":images/close") );

        gridLayout->addWidget(checkActivate,currentRow,0,Qt::AlignHCenter);
        gridLayout->addWidget(editRegExp,currentRow,1);
        gridLayout->addWidget(comboColor,currentRow,2,Qt::AlignHCenter);
        gridLayout->addWidget(comboWeight,currentRow,3,Qt::AlignHCenter);
        gridLayout->addWidget(checkItalic,currentRow,4,Qt::AlignHCenter);
        gridLayout->addWidget(checkText,currentRow,5,Qt::AlignHCenter);
        gridLayout->addWidget(checkCase,currentRow,6,Qt::AlignHCenter);
        gridLayout->addWidget(toolClose,currentRow,7,Qt::AlignHCenter);


        connect(this,SIGNAL(setSelected(bool)),checkActivate,SLOT(setChecked(bool)));
        connect(comboWeight,SIGNAL(currentIndexChanged(int)),this,SLOT(weightChanged(int)));

        switch(_weight){
        case QFont::Light: comboWeight->setCurrentIndex(0); break;
        case QFont::Normal: comboWeight->setCurrentIndex(1); break;
        case QFont::DemiBold: comboWeight->setCurrentIndex(2); break;
        case QFont::Bold: comboWeight->setCurrentIndex(3); break;
        case QFont::Black: comboWeight->setCurrentIndex(4); break;
        default: comboWeight->setCurrentIndex(1); break;
        }

        ColorItems colorItem;
        colorItem.currentRow = currentRow;
        colorItem.checkActivate = checkActivate;
        colorItem.editRegExp = editRegExp;
        colorItem.comboColor = comboColor;
        colorItem.comboWeight = comboWeight;
        colorItem.checkItalic = checkItalic;
        colorItem.checkText = checkText;
        colorItem.checkCase = checkCase;
        colorItem.toolClose = toolClose;
        colors.push_back(colorItem);

        updateHighlight();

        connect(checkActivate,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
        connect(checkText,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
        connect(checkItalic,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
        connect(checkCase,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
        connect(comboColor,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
        connect(comboWeight,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
        connect(editRegExp,SIGNAL(textChanged(QString)),this,SLOT(updateHighlight(QString)));

        connect(toolClose,SIGNAL(clicked()),this,SLOT(deleteLine()));
    }

    QList<ColorRegExp> getListRegExp(){
        QList<ColorRegExp> list;
        for(int i=0;i<colors.size();i++){
           ColorItems c = colors.at(i);
           int weight;
           switch(c.comboWeight->currentIndex()){
           case 0: weight = QFont::Light; break;
           case 1: weight = QFont::Normal; break;
           case 2: weight = QFont::DemiBold; break;
           case 3: weight = QFont::Bold; break;
           case 4: weight = QFont::Black; break;
           default: weight = QFont::Normal; break;
           }

           QColor color = QColor(c.comboColor->itemData(c.comboColor->currentIndex()).toString());
           ColorRegExp colorRegExp(c.editRegExp->text(), color, weight, c.checkItalic->isChecked(), c.checkText->isChecked(), c.checkCase->isChecked(), c.checkActivate->isChecked());
           list.push_back(colorRegExp);
        }
        return list;
    }

protected:
    QString translateColorName(QString colorName){
        if(colorName=="aliceblue") return tr("aliceblue");
        if(colorName=="antiquewhite") return tr("antiquewhite");
        if(colorName=="aqua") return tr("aqua");
        if(colorName=="aquamarine") return tr("aquamarine");
        if(colorName=="azure") return tr("azure");
        if(colorName=="beige") return tr("beige");
        if(colorName=="bisque") return tr("bisque");
        if(colorName=="black") return tr("black");
        if(colorName=="blanchedalmond") return tr("blanchedalmond");
        if(colorName=="blue") return tr("blue");
        if(colorName=="blueviolet") return tr("blueviolet");
        if(colorName=="brown") return tr("brown");
        if(colorName=="burlywood") return tr("burlywood");
        if(colorName=="cadetblue") return tr("cadetblue");
        if(colorName=="chartreuse") return tr("chartreuse");
        if(colorName=="chocolate") return tr("chocolate");
        if(colorName=="coral") return tr("coral");
        if(colorName=="cornflowerblue") return tr("cornflowerblue");
        if(colorName=="cornsilk") return tr("cornsilk");
        if(colorName=="crimson") return tr("crimson");
        if(colorName=="cyan") return tr("cyan");
        if(colorName=="darkblue") return tr("darkblue");
        if(colorName=="darkcyan") return tr("darkcyan");
        if(colorName=="darkgoldenrod") return tr("darkgoldenrod");
        if(colorName=="darkgray") return tr("darkgray");
        if(colorName=="darkgreen") return tr("darkgreen");
        if(colorName=="darkgrey") return tr("darkgrey");
        if(colorName=="darkkhaki") return tr("darkkhaki");
        if(colorName=="darkmagenta") return tr("darkmagenta");
        if(colorName=="darkolivegreen") return tr("darkolivegreen");
        if(colorName=="darkorange") return tr("darkorange");
        if(colorName=="darkorchid") return tr("darkorchid");
        if(colorName=="darkred") return tr("darkred");
        if(colorName=="darksalmon") return tr("darksalmon");
        if(colorName=="darkseagreen") return tr("darkseagreen");
        if(colorName=="darkslateblue") return tr("darkslateblue");
        if(colorName=="darkslategray") return tr("darkslategray");
        if(colorName=="darkslategrey") return tr("darkslategrey");
        if(colorName=="darkturquoise") return tr("darkturquoise");
        if(colorName=="darkviolet") return tr("darkviolet");
        if(colorName=="deeppink") return tr("deeppink");
        if(colorName=="deepskyblue") return tr("deepskyblue");
        if(colorName=="dimgray") return tr("dimgray");
        if(colorName=="dimgrey") return tr("dimgrey");
        if(colorName=="dodgerblue") return tr("dodgerblue");
        if(colorName=="firebrick") return tr("firebrick");
        if(colorName=="floralwhite") return tr("floralwhite");
        if(colorName=="forestgreen") return tr("forestgreen");
        if(colorName=="fuchsia") return tr("fuchsia");
        if(colorName=="gainsboro") return tr("gainsboro");
        if(colorName=="ghostwhite") return tr("ghostwhite");
        if(colorName=="gold") return tr("gold");
        if(colorName=="goldenrod") return tr("goldenrod");
        if(colorName=="gray") return tr("gray");
        if(colorName=="green") return tr("green");
        if(colorName=="greenyellow") return tr("greenyellow");
        if(colorName=="grey") return tr("grey");
        if(colorName=="honeydew") return tr("honeydew");
        if(colorName=="hotpink") return tr("hotpink");
        if(colorName=="indianred") return tr("indianred");
        if(colorName=="indigo") return tr("indigo");
        if(colorName=="ivory") return tr("ivory");
        if(colorName=="khaki") return tr("khaki");
        if(colorName=="lavender") return tr("lavender");
        if(colorName=="lavenderblush") return tr("lavenderblush");
        if(colorName=="lawngreen") return tr("lawngreen");
        if(colorName=="lemonchiffon") return tr("lemonchiffon");
        if(colorName=="lightblue") return tr("lightblue");
        if(colorName=="lightcoral") return tr("lightcoral");
        if(colorName=="lightcyan") return tr("lightcyan");
        if(colorName=="lightgoldenrodyellow") return tr("lightgoldenrodyellow");
        if(colorName=="lightgray") return tr("lightgray");
        if(colorName=="lightgreen") return tr("lightgreen");
        if(colorName=="lightgrey") return tr("lightgrey");
        if(colorName=="lightpink") return tr("lightpink");
        if(colorName=="lightsalmon") return tr("lightsalmon");
        if(colorName=="lightseagreen") return tr("lightseagreen");
        if(colorName=="lightskyblue") return tr("lightskyblue");
        if(colorName=="lightslategray") return tr("lightslategray");
        if(colorName=="lightslategrey") return tr("lightslategrey");
        if(colorName=="lightsteelblue") return tr("lightsteelblue");
        if(colorName=="lightyellow") return tr("lightyellow");
        if(colorName=="lime") return tr("lime");
        if(colorName=="limegreen") return tr("limegreen");
        if(colorName=="linen") return tr("linen");
        if(colorName=="magenta") return tr("magenta");
        if(colorName=="maroon") return tr("maroon");
        if(colorName=="mediumaquamarine") return tr("mediumaquamarine");
        if(colorName=="mediumblue") return tr("mediumblue");
        if(colorName=="mediumorchid") return tr("mediumorchid");
        if(colorName=="mediumpurple") return tr("mediumpurple");
        if(colorName=="mediumseagreen") return tr("mediumseagreen");
        if(colorName=="mediumslateblue") return tr("mediumslateblue");
        if(colorName=="mediumspringgreen") return tr("mediumspringgreen");
        if(colorName=="mediumturquoise") return tr("mediumturquoise");
        if(colorName=="mediumvioletred") return tr("mediumvioletred");
        if(colorName=="midnightblue") return tr("midnightblue");
        if(colorName=="mintcream") return tr("mintcream");
        if(colorName=="mistyrose") return tr("mistyrose");
        if(colorName=="moccasin") return tr("moccasin");
        if(colorName=="navajowhite") return tr("navajowhite");
        if(colorName=="navy") return tr("navy");
        if(colorName=="oldlace") return tr("oldlace");
        if(colorName=="olive") return tr("olive");
        if(colorName=="olivedrab") return tr("olivedrab");
        if(colorName=="orange") return tr("orange");
        if(colorName=="orangered") return tr("orangered");
        if(colorName=="orchid") return tr("orchid");
        if(colorName=="palegoldenrod") return tr("palegoldenrod");
        if(colorName=="palegreen") return tr("palegreen");
        if(colorName=="paleturquoise") return tr("paleturquoise");
        if(colorName=="palevioletred") return tr("palevioletred");
        if(colorName=="papayawhip") return tr("papayawhip");
        if(colorName=="peachpuff") return tr("peachpuff");
        if(colorName=="peru") return tr("peru");
        if(colorName=="pink") return tr("pink");
        if(colorName=="plum") return tr("plum");
        if(colorName=="powderblue") return tr("powderblue");
        if(colorName=="purple") return tr("purple");
        if(colorName=="red") return tr("red");
        if(colorName=="rosybrown") return tr("rosybrown");
        if(colorName=="royalblue") return tr("royalblue");
        if(colorName=="saddlebrown") return tr("saddlebrown");
        if(colorName=="salmon") return tr("salmon");
        if(colorName=="sandybrown") return tr("sandybrown");
        if(colorName=="seagreen") return tr("seagreen");
        if(colorName=="seashell") return tr("seashell");
        if(colorName=="sienna") return tr("sienna");
        if(colorName=="silver") return tr("silver");
        if(colorName=="skyblue") return tr("skyblue");
        if(colorName=="slateblue") return tr("slateblue");
        if(colorName=="slategray") return tr("slategray");
        if(colorName=="slategrey") return tr("slategrey");
        if(colorName=="snow") return tr("snow");
        if(colorName=="springgreen") return tr("springgreen");
        if(colorName=="steelblue") return tr("steelblue");
        if(colorName=="tan") return tr("tan");
        if(colorName=="teal") return tr("teal");
        if(colorName=="thistle") return tr("thistle");
        if(colorName=="tomato") return tr("tomato");
        if(colorName=="transparent") return tr("transparent");
        if(colorName=="turquoise") return tr("turquoise");
        if(colorName=="violet") return tr("violet");
        if(colorName=="wheat") return tr("wheat");
        if(colorName=="white") return tr("white");
        if(colorName=="whitesmoke") return tr("whitesmoke");
        if(colorName=="yellow") return tr("yellow");
        if(colorName=="yellowgreen") return tr("yellowgreen");
        return colorName;
    }

private:

    struct ColorItems{
        int currentRow;
        QCheckBox *checkActivate;
        QLineEdit *editRegExp;
        QComboBox *comboColor;
        QComboBox *comboWeight;
        QCheckBox *checkItalic;
        QCheckBox *checkText;
        QCheckBox *checkCase;
        QToolButton *toolClose;
    };

    QList<ColorItems> colors;

    QGridLayout *gridLayout;
    QTextEdit *edit;
    Highlighter *highlighter;

    QList<QPair<QString,QString> > listColors;

signals:
     void setSelected(bool);

public slots:
     void setEditFont(QFont font){
         edit->setFont(font);
     }

     void weightChanged(int index){
         QComboBox *box = (QComboBox*)sender();
         box->setFont( ((QListWidget*)box->view())->item(index)->font() );
     }
     void deleteLine(){
         QToolButton *tool = (QToolButton*)sender();
         int i = 0;
         for(i=0;i<colors.size();i++){
             if(tool==colors.at(i).toolClose){
                 break;
             }
         }
         deleteLine(i);
     }
     void deleteLine(int index){
         disconnect(colors.at(index).checkActivate,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).checkText,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).checkCase,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).comboColor,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).comboWeight,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).checkItalic,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(index).editRegExp,SIGNAL(textChanged(QString)),this,SLOT(updateHighlight(QString)));
         disconnect(colors.at(index).toolClose,SIGNAL(clicked()),this,SLOT(deleteLine()));

         gridLayout->removeWidget(colors.at(index).checkActivate);
         gridLayout->removeWidget(colors.at(index).editRegExp);
         gridLayout->removeWidget(colors.at(index).comboColor);
         gridLayout->removeWidget(colors.at(index).comboWeight);
         gridLayout->removeWidget(colors.at(index).checkItalic);
         gridLayout->removeWidget(colors.at(index).checkText);
         gridLayout->removeWidget(colors.at(index).checkCase);
         gridLayout->removeWidget(colors.at(index).toolClose);

         delete colors.at(index).checkActivate;
         delete colors.at(index).editRegExp;
         delete colors.at(index).comboColor;
         delete colors.at(index).comboWeight;
         delete colors.at(index).checkItalic;
         delete colors.at(index).checkText;
         delete colors.at(index).checkCase;
         delete colors.at(index).toolClose;

         colors.removeAt(index);
         updateHighlight();

     }
     void clear(){
         while(colors.size()!=0){
             deleteLine(0);
         }
     }
     void selectAll(){
         emit setSelected(true);
     }
     void unselectAll(){
         emit setSelected(false);
     }
     void addNew(){
         addColorRegExp();
     }
     void updateHighlight(QString){
         updateHighlight();
     }
     void updateHighlight(int){
         updateHighlight();
     }

     void updateHighlight(){
         highlighter->clear();

         for(int i=0;i<colors.size();i++){
            ColorItems c = colors.at(i);
            if(c.checkActivate->isChecked() && !c.editRegExp->text().isEmpty()){
                int weight;
                switch(c.comboWeight->currentIndex()){
                case 0: weight = QFont::Light; break;
                case 1: weight = QFont::Normal; break;
                case 2: weight = QFont::DemiBold; break;
                case 3: weight = QFont::Bold; break;
                case 4: weight = QFont::Black; break;
                default: weight = QFont::Normal; break;
                }

                QColor color = QColor(c.comboColor->itemData(c.comboColor->currentIndex()).toString());
                highlighter->addRule(c.editRegExp->text(),color,weight,c.checkItalic->isChecked(), c.checkText->isChecked(),
                                     c.checkCase->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
            }
         }

         highlighter->rehighlight();
     }
};













class OptionsForm : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsForm(OptionsValues options, QWidget *parent = 0);
    OptionsValues getOptions();

private:
    OptionsValues options_;

    //Colors
    QCheckBox *checkEnableColors;
    ColorRegExpForm *cref;

    //General
    QCheckBox *checkSelectNewTab;
    QCheckBox *checkOpenReadOnly;
    QCheckBox *checkreOpenPreviousTabs;
    QLineEdit *editDefaultFolder;
    QComboBox *comboOpenSize;
    QComboBox *comboChordSize;
    QLabel *fontLabel;

    //Print
    QCheckBox *checkEnableColorsOnPrinting;
    QCheckBox *checkPrintHeaderOnEachPages;

    //Other
    QDialogButtonBox *buttonBox;

protected:
    void createColorTab(QTabWidget *tab);
    void createGeneralTab(QTabWidget *tab);
    void createPrintTab(QTabWidget *tab);


signals:

public slots:
    void buttonClicked(QAbstractButton *button);
    void selectFolder();
    void selectFont();

};


#endif // OPTIONSFORM_H
