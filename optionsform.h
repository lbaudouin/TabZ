#ifndef OPTIONSFORM_H
#define OPTIONSFORM_H

#include <QDialog>
#include <QDialogButtonBox>
//#include <QFontDialog>
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
#include <QSpinBox>
#include <QScrollArea>

#include "options.h"
#include "colorregexpform.h"
#include "openeditlayout.h"
#include "fontlabellayout.h"


#include <QColorDialog>

#include <QListWidget>



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
    QCheckBox *autoCreateFolder;
    QComboBox *comboOpenSize;
    QComboBox *comboChordSize;
    QComboBox *comboToolBarPosition;
    FontLabelLayout *mainFontLabel;
    FontLabelLayout *titleFontLabel;
    FontLabelLayout *artistFontLabel;
    FontLabelLayout *otherFontLabel;
    QSpinBox *spinNbSpaceForOneTab;

    //Print
    OpenEditLayout *defaultOutputFolder;
    QCheckBox *checkEnableColorsOnPrinting;
    QCheckBox *checkPrintHeaderOnEachPages;
    QSpinBox *editTop,*editLeft,*editRight,*editBottom;

    //Other
    QDialogButtonBox *buttonBox;

protected:
    void createColorTab(QTabWidget *tab);
    void createGeneralTab(QTabWidget *tab);
    void createPrintTab(QTabWidget *tab);


signals:

public slots:
    void buttonClicked(QAbstractButton *button);

};


#endif // OPTIONSFORM_H
