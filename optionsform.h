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

#include "options.h"

#include <QDebug>

#include <QColorDialog>

#include <QListWidget>

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
        for(int i=0;i<list.size();i++){
            mainLayout->addWidget( new ColorRegExpEdit(list.at(i).regExp, list.at(i).color, list.at(i).weight, list.at(i).isText ) );
        }

        buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
        connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
        connect(buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));

        mainLayout->addWidget(buttonBox);
    }

private:
    QDialogButtonBox *buttonBox;

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
