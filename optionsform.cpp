#include "optionsform.h"

OptionsForm::OptionsForm(OptionsValues options, QWidget *parent) : QDialog(parent), options_(options)
{
    this->setMinimumSize(640,480);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    createGeneralTab(tabWidget);
    createColorTab(tabWidget);
    createPrintTab(tabWidget);

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

    checkEnableColors = new QCheckBox;
    checkEnableColors->setChecked(options_.enableColors);
    formLayout->addRow(tr("Enable color:"),checkEnableColors);

    cref = new ColorRegExpForm;
    for(int i=0;i<options_.colors.size();i++)
        cref->addColorRegExp(options_.colors.at(i).regExp,
                             options_.colors.at(i).color,
                             options_.colors.at(i).weight,
                             options_.colors.at(i).italic,
                             options_.colors.at(i).isText,
                             options_.colors.at(i).caseSensitivity?Qt::CaseSensitive:Qt::CaseInsensitive,
                             options_.colors.at(i).active);

    cref->setEditFont(options_.font);

    QScrollArea *area = new QScrollArea;
    area->setWidgetResizable(true);
    area->setWidget(cref);

    mainLayout->addWidget(area);

    tab->addTab(w,tr("Colors"));
}

void OptionsForm::createGeneralTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

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

    QStringList sizes;
    sizes << "100x150" << "125x175" << "150x200";
    int sizeIndex = sizes.indexOf(options_.fromSize(options_.chordSize));
    if(sizeIndex<0) sizeIndex = 1;
    comboChordSize = new QComboBox;
    comboChordSize->addItems( sizes );
    comboChordSize->setCurrentIndex(sizeIndex);

    comboToolBarPosition = new QComboBox;
    comboToolBarPosition->addItems(QStringList() << tr("Top") << tr("Left") << tr("Right") << tr("Bottom"));
    comboToolBarPosition->setCurrentIndex(options_.mainToolBarPosition);

    fontLabel = new QLabel;
    fontLabel->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    fontLabel->setText(options_.font.toString());
    fontLabel->setFont(options_.font);

    QToolButton *toolFont = new QToolButton;
    //toolFont->setIcon(QIcon(":images/open"));
    toolFont->setIcon(QIcon(QLatin1String(":/trolltech/styles/commonstyle/images/fonttruetype-16.png")));
    connect(toolFont,SIGNAL(clicked()),this,SLOT(selectFont()));

    QHBoxLayout *fontLayout = new QHBoxLayout;
    fontLayout->addWidget(toolFont);
    fontLayout->addWidget(fontLabel);

    formLayout->addRow(tr("Default font:"),fontLayout);
    formLayout->addRow(tr("Default folder:"),folderLayout);
    formLayout->addRow(tr("Select new tab:"),checkSelectNewTab);
    formLayout->addRow(tr("Open read only:"),checkOpenReadOnly);
    formLayout->addRow(tr("Re-open previous tabs:"),checkreOpenPreviousTabs);
    formLayout->addRow(tr("Open size mode:"),comboOpenSize);
    formLayout->addRow(tr("Chord size:"),comboChordSize);
    formLayout->addRow(tr("ToolBar position:"),comboToolBarPosition);

    tab->addTab(w,tr("General"));
}

void OptionsForm::createPrintTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    //Enable colors on printig
    checkEnableColorsOnPrinting = new QCheckBox;
    checkEnableColorsOnPrinting->setChecked(options_.enableColorsOnPrinting);

    //Print headers on each pages
    checkPrintHeaderOnEachPages = new QCheckBox;
    checkPrintHeaderOnEachPages->setChecked(options_.printHearderOnEachPages);

    //Default margins
    editTop = new QSpinBox;
    editTop->setMaximumWidth(150);
    editTop->setSuffix(" " + tr("mm"));
    editLeft = new QSpinBox;
    editLeft->setMaximumWidth(150);
    editLeft->setSuffix(" " + tr("mm"));
    editRight = new QSpinBox;
    editRight->setMaximumWidth(150);
    editRight->setSuffix(" " + tr("mm"));
    editBottom = new QSpinBox;
    editBottom->setMaximumWidth(150);
    editBottom->setSuffix(" " + tr("mm"));

    editTop->setValue(options_.topMargin);
    editLeft->setValue(options_.leftMargin);
    editRight->setValue(options_.rightMargin);
    editBottom->setValue(options_.bottomMargin);

    QVBoxLayout *marginVBox = new QVBoxLayout;
    QHBoxLayout *marginHBox = new QHBoxLayout;
    QHBoxLayout *marginMainBox = new QHBoxLayout;

    QSpacerItem *s1 = new QSpacerItem(5,0,QSizePolicy::Expanding);
    QSpacerItem *s2 = new QSpacerItem(5,0,QSizePolicy::Expanding);

    marginVBox->addWidget(editTop,0,Qt::AlignCenter);
    marginHBox->addWidget(editLeft);
    marginHBox->addWidget(editRight);
    marginVBox->addLayout(marginHBox);
    marginVBox->addWidget(editBottom,0,Qt::AlignCenter);

    marginMainBox->addLayout(marginVBox);
    marginMainBox->addSpacerItem(s2);

    //Chord size


    //Create form
    formLayout->addRow(tr("Enable colors on printing:"),checkEnableColorsOnPrinting);
    formLayout->addRow(tr("Print header on each pages:"),checkPrintHeaderOnEachPages);
    formLayout->addRow(tr("Margins:"),marginMainBox);

    tab->addTab(w,tr("Print"));
}

OptionsValues OptionsForm::getOptions()
{
    options_.enableColors = checkEnableColors->isChecked();
    options_.selectNewTab = checkSelectNewTab->isChecked();
    options_.openReadOnly = checkOpenReadOnly->isChecked();
    options_.reOpenPreviousTabs = checkreOpenPreviousTabs->isChecked();
    options_.defaultPath = editDefaultFolder->text();
    options_.openSizeMode = comboOpenSize->currentIndex();
    options_.font.fromString( fontLabel->text() );
    options_.chordSize = options_.toSize( comboChordSize->currentText() );
    options_.printHearderOnEachPages = checkPrintHeaderOnEachPages->isChecked();
    options_.enableColorsOnPrinting = checkEnableColorsOnPrinting->isChecked();
    options_.mainToolBarPosition = comboToolBarPosition->currentIndex();
    options_.topMargin = editTop->value();
    options_.leftMargin = editLeft->value();
    options_.rightMargin = editRight->value();
    options_.bottomMargin = editBottom->value();

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

void OptionsForm::selectFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, options_.font, this);
    if(ok){
        fontLabel->setText(font.toString());
        fontLabel->setFont(font);
        cref->setEditFont(font);
    }
}

void OptionsForm::buttonClicked(QAbstractButton *button){
    if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){
        options_ = OptionsValues();
        options_.setDefaultRegExp();

        checkEnableColors->setChecked(options_.enableColors);
        checkSelectNewTab->setChecked(options_.selectNewTab);
        checkOpenReadOnly->setChecked(options_.openReadOnly);
        checkreOpenPreviousTabs->setChecked(options_.reOpenPreviousTabs);
        editDefaultFolder->setText(options_.defaultPath);
        comboOpenSize->setCurrentIndex(options_.openSizeMode);
        fontLabel->setText(options_.font.toString());
        checkPrintHeaderOnEachPages->setChecked(options_.printHearderOnEachPages);
        checkEnableColorsOnPrinting->setChecked(options_.enableColorsOnPrinting);

        cref->clear();
        for(int i=0;i<options_.colors.size();i++)
            cref->addColorRegExp(options_.colors.at(i).regExp,
                                 options_.colors.at(i).color,
                                 options_.colors.at(i).weight,
                                 options_.colors.at(i).italic,
                                 options_.colors.at(i).isText,
                                 options_.colors.at(i).caseSensitivity?Qt::CaseSensitive:Qt::CaseInsensitive,
                                 options_.colors.at(i).active);

        cref->setEditFont(options_.font);
    }
}
