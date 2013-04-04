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

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
}

void OptionsForm::createColorTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    w->setLayout(mainLayout);

    QFormLayout *formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    checkEnableColor = new QCheckBox;
    checkEnableColor->setChecked(options_.enableColors);
    formLayout->addRow(tr("Enable color:"),checkEnableColor);

    cref = new ColorRegExpForm;
    for(int i=0;i<options_.colors.size();i++)
        cref->addColorRegExp(options_.colors.at(i).regExp,
                             options_.colors.at(i).color,
                             options_.colors.at(i).weight,
                             options_.colors.at(i).isText,
                             options_.colors.at(i).caseSensitivity?Qt::CaseSensitive:Qt::CaseInsensitive,
                             options_.colors.at(i).active);


    QScrollArea *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setWidget(cref);

    mainLayout->addWidget(area);

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

    checkreOpenPreviousTabs = new QCheckBox;
    checkreOpenPreviousTabs->setChecked(options_.reOpenPreviousTabs);

    comboOpenSize = new QComboBox;
    comboOpenSize->addItems( QStringList() << tr("Normal") << tr("Maximized") << tr("FullScreen") );
    comboOpenSize->setCurrentIndex(options_.openSizeMode);


    formLayout->addRow(tr("Default folder:"),folderLayout);
    formLayout->addRow(tr("Select new tab:"),checkSelectNewTab);
    formLayout->addRow(tr("Open read only:"),checkOpenReadOnly);
    formLayout->addRow(tr("Re-open previous tabs:"),checkreOpenPreviousTabs);
    formLayout->addRow(tr("Open size mode:"),comboOpenSize);

    tab->addTab(w,tr("Options"));
}

OptionsValues OptionsForm::getOptions()
{
    options_.enableColors = checkEnableColor->isChecked();
    options_.selectNewTab = checkSelectNewTab->isChecked();
    options_.openReadOnly = checkOpenReadOnly->isChecked();
    options_.reOpenPreviousTabs = checkreOpenPreviousTabs->isChecked();
    options_.defaultPath = editDefaultFolder->text();
    options_.openSizeMode = comboOpenSize->currentIndex();

    options_.colors = cref->getListRegExp();

    return options_;
}

void OptionsForm::selectFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this,tr("Select folder"),tr("Path:"));
    if(!folder.isEmpty()){
        editDefaultFolder->setText(folder);
    }
}
