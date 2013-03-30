#include "options.h"

Options::Options(OptionsValues options, QWidget *parent) : options_(options),
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    createColorTab(tabWidget);
    createOptionsTab(tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));

    mainLayout->addWidget(buttonBox);
}

void Options::createColorTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    checkEnableColor = new QCheckBox;
    checkEnableColor->setChecked(options_.enableColors);
    formLayout->addRow(tr("Enable color:"),checkEnableColor);



    tab->addTab(w,tr("Color"));
}

void Options::createOptionsTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("TEST");

    QLineEdit *lineEdit = new QLineEdit;

    formLayout->addRow(tr("Default folder:"),lineEdit);



    tab->addTab(w,tr("Options"));
}

OptionsValues Options::getOptions()
{
    options_.enableColors = checkEnableColor->isChecked();
    return options_;
}
