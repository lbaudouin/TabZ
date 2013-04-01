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

        gridLayout->addWidget(new QLabel(tr("Active")),0,0);
        gridLayout->addWidget(new QLabel(tr("Regular Expression")),0,1);
        gridLayout->addWidget(new QLabel(tr("Color")),0,2);
        gridLayout->addWidget(new QLabel(tr("Weight")),0,3);
        gridLayout->addWidget(new QLabel(tr("Text")),0,4);
        gridLayout->addWidget(new QLabel(tr("Case Sensitive")),0,5);

        mainLayout->addLayout(gridLayout);

        edit = new QTextEdit;
        edit->setMaximumHeight(300);

        mainLayout->addWidget(edit);

        edit->setText("Intro\nA Gm F#7\nB Dm Edim\n\nChorus\nAm Fm Dm\nThis is a test");

        highlighter = new Highlighter(edit->document());


    }

    void addColorRegExp(QString _regExp, QColor _color, int _weight, int _isText = false, int caseSensitive = 1)
    {
        int currentRow = gridLayout->rowCount();
        QCheckBox *checkActivate = new QCheckBox;
        QLineEdit *editRegExp = new QLineEdit(_regExp);
        QComboBox *comboColor = new QComboBox;
        QComboBox *comboWeight = new QComboBox;
        QCheckBox *checkText = new QCheckBox;
        QCheckBox *checkCase = new QCheckBox;
        gridLayout->addWidget(checkActivate,currentRow,0);
        gridLayout->addWidget(editRegExp,currentRow,1);
        gridLayout->addWidget(comboColor,currentRow,2);
        gridLayout->addWidget(comboWeight,currentRow,3);
        gridLayout->addWidget(checkText,currentRow,4);
        gridLayout->addWidget(checkCase,currentRow,5);


        QStringList colorNames = QColor::colorNames();
        //foreach(QString colorName, colorNames){
        for(int i=0;i<colorNames.size();i++){
            QColor color(colorNames[i]);
            comboColor->insertItem(i, colorNames[i]);
            comboColor->setItemData(i, color, Qt::DecorationRole);
            if(color==_color)
                comboColor->setCurrentIndex(i);
        }



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

        connect(comboWeight,SIGNAL(currentIndexChanged(int)),this,SLOT(weightChanged(int)));
    }

private:
     QGridLayout *gridLayout;
     QTextEdit *edit;
     Highlighter *highlighter;

public slots:
     void weightChanged(int index){
         QComboBox *box = (QComboBox*)sender();
         box->setFont( ((QListWidget*)box->view())->item(index)->font() );
     }
     void colorChanged(int){
        //TODO
         /*QComboBox *box = (QComboBox*)sender();
         QString colorString = box->currentText();
         QColor color(colorString);

         QListWidgetItem *item = ((QListWidget*)box->view())->item(index);
         item->setBackgroundColor(color);*/
     }
};

class ColorRegExpEdit : public QWidget
{
    Q_OBJECT
public:
    ColorRegExpEdit(QString _regExp, QColor _color, int _weight, int _isText = false){
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        editRegExp = new QLineEdit(_regExp);
        comboColor = new QComboBox;
        comboWeight = new QComboBox;
        checkText = new QCheckBox;

        comboColor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        comboWeight->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

        QStringList colorNames = QColor::colorNames();
        //foreach(QString colorName, colorNames){
        for(int i=0;i<colorNames.size();i++){
        QColor color(colorNames[i]);
        comboColor->insertItem(i, colorNames[i]);
        comboColor->setItemData(i, color, Qt::DecorationRole);
        if(color==_color)
            comboColor->setCurrentIndex(i);
        }

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


        mainLayout->addWidget(editRegExp);
        mainLayout->addWidget(comboColor);
        mainLayout->addWidget(comboWeight);
        mainLayout->addWidget(checkText);
    }
private:
    QLineEdit *editRegExp;
    QComboBox *comboColor;
    QComboBox *comboWeight;
    QCheckBox *checkText;
};

class ColorRegExpDialog : public QDialog
{
    Q_OBJECT
public:
    ColorRegExpDialog(QList<ColorRegExp> list){
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        cref = new ColorRegExpForm;

        mainLayout->addWidget(cref);
        /*for(int i=0;i<list.size();i++){
            mainLayout->addWidget( new ColorRegExpEdit(list.at(i).regExp, list.at(i).color, list.at(i).weight, list.at(i).isText ) );
        }*/

        buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
        connect(buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));

        mainLayout->addWidget(buttonBox);
    }

private:
    QDialogButtonBox *buttonBox;
    ColorRegExpForm *cref;

public slots:
    void buttonClicked(QAbstractButton *button){
        if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){
            qDebug() << "Restore";
        }
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
    QLineEdit *editDefaultFolder;
    QComboBox *comboOpenSize;

    QDialogButtonBox *buttonBox;

protected:
    void createColorTab(QTabWidget *tab);
    void createOptionsTab(QTabWidget *tab);


signals:

public slots:
    void showColorExpReg();
    void buttonClicked(QAbstractButton *button){
        if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){
            qDebug() << "Restore";
        }
    }
    void selectFolder();

};


#endif // OPTIONSFORM_H
