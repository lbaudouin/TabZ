#include "optionsform.h"

OptionsForm::OptionsForm(OptionsValues options, QWidget *parent) : options_(options),
    QDialog(parent)
{
    this->setMinimumSize(640,480);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    createColorTab(tabWidget);
    createOptionsTab(tabWidget);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
    connect(buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));

    mainLayout->addWidget(buttonBox);
}

void OptionsForm::createColorTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    checkEnableColor = new QCheckBox;
    checkEnableColor->setChecked(options_.enableColors);
    formLayout->addRow(tr("Enable color:"),checkEnableColor);

    QPushButton *buttonRegExp = new QPushButton(tr("Edit"));
    buttonRegExp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    formLayout->addRow(tr("Regular expressions:"),buttonRegExp);
    connect(buttonRegExp,SIGNAL(clicked()),this,SLOT(showColorExpReg()));


    //ColorRegExpEdit *editColor = new ColorRegExpEdit("test",Qt::red,75);
    //formLayout->addWidget(editColor);

    tab->addTab(w,tr("Color"));
}

void OptionsForm::createOptionsTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("TEST");

    editDefaultFolder = new QLineEdit;
    editDefaultFolder->setText(options_.defaultPath);
    editDefaultFolder->setReadOnly(true);

    QToolButton *toolFolder = new QToolButton;
    toolFolder->setIcon(QIcon(":images/open"));
    connect(toolFolder,SIGNAL(clicked()),this,SLOT(selectFolder()));

    QHBoxLayout *folderLayout = new QHBoxLayout;
    folderLayout->addWidget(toolFolder);
    folderLayout->addWidget(editDefaultFolder);

    checkSelectNewTab = new QCheckBox;
    checkSelectNewTab->setChecked(options_.selectNewTab);

    checkOpenReadOnly = new QCheckBox;
    checkOpenReadOnly->setChecked(options_.openReadOnly);

    comboOpenSize = new QComboBox;
    comboOpenSize->addItems( QStringList() << tr("Normal") << tr("Maximized") << tr("FullScreen") );
    comboOpenSize->setCurrentIndex(options_.openSizeMode);


    formLayout->addRow(tr("Default folder:"),folderLayout);
    formLayout->addRow(tr("Select new tab:"),checkSelectNewTab);
    formLayout->addRow(tr("Open read only:"),checkOpenReadOnly);
    formLayout->addRow(tr("Open size mode:"),comboOpenSize);



    tab->addTab(w,tr("Options"));
}

OptionsValues OptionsForm::getOptions()
{
    options_.enableColors = checkEnableColor->isChecked();
    options_.selectNewTab = checkSelectNewTab->isChecked();
    options_.openReadOnly = checkOpenReadOnly->isChecked();
    options_.defaultPath = editDefaultFolder->text();
    options_.openSizeMode = comboOpenSize->currentIndex();
    return options_;
}

void OptionsForm::showColorExpReg()
{
    ColorRegExpDialog *cred = new ColorRegExpDialog(options_.colors);
    cred->exec();
}

void OptionsForm::selectFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this,tr("Select folder"),tr("Path:"));
    if(!folder.isEmpty()){
        editDefaultFolder->setText(folder);
    }
}
