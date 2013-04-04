#ifndef OPTIONSFORM_H
#define OPTIONSFORM_H

#include <QDialog>
#include <QDialogButtonBox>
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
        gridLayout->addWidget(new QLabel(tr("Text")),0,4,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Case Sensitive")),0,5,Qt::AlignHCenter);
        gridLayout->addWidget(new QLabel(tr("Delete")),0,6,Qt::AlignHCenter);

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


    }

    void addColorRegExp(QString regExp = "", QColor _color = Qt::black, int _weight = QFont::Normal, int isText = false, int caseSensitive = 1, int active = 1)
    {
        int currentRow = gridLayout->rowCount();
        QCheckBox *checkActivate = new QCheckBox;
        QLineEdit *editRegExp = new QLineEdit(regExp);
        QComboBox *comboColor = new QComboBox;
        QComboBox *comboWeight = new QComboBox;
        QCheckBox *checkText = new QCheckBox;
        QCheckBox *checkCase = new QCheckBox;
        QToolButton *toolClose = new QToolButton;


        QStringList colorNames = QColor::colorNames();
        for(int i=0;i<colorNames.size();i++){
            QColor color(colorNames[i]);
            QPixmap pixmap(16,16);
            pixmap.fill(color);
            comboColor->addItem(QIcon(pixmap),colorNames[i]);
            if(color==_color)
                comboColor->setCurrentIndex(i);
        }

        checkActivate->setChecked(active);
        checkText->setChecked(isText);
        checkCase->setChecked(caseSensitive);


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
        gridLayout->addWidget(checkText,currentRow,4,Qt::AlignHCenter);
        gridLayout->addWidget(checkCase,currentRow,5,Qt::AlignHCenter);
        gridLayout->addWidget(toolClose,currentRow,6,Qt::AlignHCenter);


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
        colorItem.checkText = checkText;
        colorItem.checkCase = checkCase;
        colorItem.toolClose = toolClose;
        colors.push_back(colorItem);

        updateHighlight();

        connect(checkActivate,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
        connect(checkText,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
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

           ColorRegExp colorRegExp(c.editRegExp->text(),QColor(c.comboColor->currentText()),weight,c.checkText->isChecked(), c.checkCase->isChecked(), c.checkActivate->isChecked());
           list.push_back(colorRegExp);
        }
        return list;
    }

private:

    struct ColorItems{
        int currentRow;
        QCheckBox *checkActivate;
        QLineEdit *editRegExp;
        QComboBox *comboColor;
        QComboBox *comboWeight;
        QCheckBox *checkText;
        QCheckBox *checkCase;
        QToolButton *toolClose;
    };

    QList<ColorItems> colors;

    QGridLayout *gridLayout;
    QTextEdit *edit;
    Highlighter *highlighter;

signals:
     void setSelected(bool);

public slots:
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

         disconnect(colors.at(i).checkActivate,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(i).checkText,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(i).checkCase,SIGNAL(stateChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(i).comboColor,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(i).comboWeight,SIGNAL(currentIndexChanged(int)),this,SLOT(updateHighlight(int)));
         disconnect(colors.at(i).editRegExp,SIGNAL(textChanged(QString)),this,SLOT(updateHighlight(QString)));
         disconnect(colors.at(i).toolClose,SIGNAL(clicked()),this,SLOT(deleteLine()));

         gridLayout->removeWidget(colors.at(i).checkActivate);
         gridLayout->removeWidget(colors.at(i).editRegExp);
         gridLayout->removeWidget(colors.at(i).comboColor);
         gridLayout->removeWidget(colors.at(i).comboWeight);
         gridLayout->removeWidget(colors.at(i).checkText);
         gridLayout->removeWidget(colors.at(i).checkCase);
         gridLayout->removeWidget(colors.at(i).toolClose);

         delete colors.at(i).checkActivate;
         delete colors.at(i).editRegExp;
         delete colors.at(i).comboColor;
         delete colors.at(i).comboWeight;
         delete colors.at(i).checkText;
         delete colors.at(i).checkCase;
         delete colors.at(i).toolClose;

         colors.removeAt(i);
         updateHighlight();

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

                highlighter->addRule(c.editRegExp->text(),QColor(c.comboColor->currentText()),weight,c.checkText->isChecked(),
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

    QCheckBox *checkEnableColor;
    QCheckBox *checkSelectNewTab;
    QCheckBox *checkOpenReadOnly;
    QCheckBox *checkreOpenPreviousTabs;
    QLineEdit *editDefaultFolder;
    QComboBox *comboOpenSize;
    ColorRegExpForm *cref;

    QDialogButtonBox *buttonBox;

protected:
    void createColorTab(QTabWidget *tab);
    void createOptionsTab(QTabWidget *tab);


signals:

public slots:
    void buttonClicked(QAbstractButton *button){
        if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){
            qDebug() << "Restore";
        }
    }
    void selectFolder();

};


#endif // OPTIONSFORM_H
