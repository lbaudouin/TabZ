#include "optionsform.h"

OptionsForm::OptionsForm(OptionsValues options, QWidget *parent) : QDialog(parent), options_(options)
{
    this->setWindowTitle(tr("Setting"));
    this->setMinimumSize(800,600);
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

    cref->setEditFont(options_.mainFont);

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

    OpenEditLayout *leditDefaultFolder = new OpenEditLayout(OpenEditLayout::FolderMode,true);
    editDefaultFolder = leditDefaultFolder->getEditWidget();
    editDefaultFolder->setText(options_.defaultPath);

    autoCreateFolder = new QCheckBox;
    autoCreateFolder->setChecked(options_.autoCreateFolder);

    checkSelectNewTab = new QCheckBox;
    checkSelectNewTab->setChecked(options_.selectNewTab);

    checkOpenReadOnly = new QCheckBox;
    checkOpenReadOnly->setChecked(options_.openReadOnly);

    checkreOpenPreviousTabs = new QCheckBox;
    checkreOpenPreviousTabs->setChecked(options_.reOpenPreviousTabs);

    comboOpenSize = new QComboBox;
    comboOpenSize->addItems( QStringList() << tr("Normal") << tr("Maximized") << tr("FullScreen") << tr("Last used") );
    comboOpenSize->setCurrentIndex(options_.openSizeMode);

    QStringList sizes;
    sizes << "100x150" << "125x175" << "150x200";
    int sizeIndex = sizes.indexOf(Options::fromSize(options_.chordSize));
    if(sizeIndex<0) sizeIndex = 1;
    comboChordSize = new QComboBox;
    comboChordSize->addItems( sizes );
    comboChordSize->setCurrentIndex(sizeIndex);

    comboToolBarPosition = new QComboBox;
    comboToolBarPosition->addItems(QStringList() << tr("Top") << tr("Left") << tr("Right") << tr("Bottom"));
    comboToolBarPosition->setCurrentIndex(options_.mainToolBarPosition);

    spinNbSpaceForOneTab = new QSpinBox;
    spinNbSpaceForOneTab->setValue(options_.nbSpaceForOneTab);

    formLayout->addRow(tr("Default folder:"),leditDefaultFolder);
    formLayout->addRow(tr("Auto-create artist folder:"),autoCreateFolder);
    formLayout->addRow(tr("Select new tab:"),checkSelectNewTab);
    formLayout->addRow(tr("Open read only:"),checkOpenReadOnly);
    formLayout->addRow(tr("Re-open previous tabs:"),checkreOpenPreviousTabs);
    formLayout->addRow(tr("Open size mode:"),comboOpenSize);
    formLayout->addRow(tr("Chord size:"),comboChordSize);
    formLayout->addRow(tr("ToolBar position:"),comboToolBarPosition);
    formLayout->addRow(tr("Spaces for one tabulation:"),spinNbSpaceForOneTab);

    tab->addTab(w,tr("General"));
}

void OptionsForm::createPrintTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    //Default output folder
    defaultOutputFolder = new OpenEditLayout(OpenEditLayout::FolderMode,true);
    defaultOutputFolder->setText(options_.defaultOutputFolder);

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

    QSpacerItem *spacer = new QSpacerItem(5,0,QSizePolicy::Expanding);

    marginVBox->addWidget(editTop,0,Qt::AlignCenter);
    marginHBox->addWidget(editLeft);
    marginHBox->addWidget(editRight);
    marginVBox->addLayout(marginHBox);
    marginVBox->addWidget(editBottom,0,Qt::AlignCenter);

    marginMainBox->addLayout(marginVBox);
    marginMainBox->addSpacerItem(spacer);

    //Chord size

    //Print font
    mainFontLabel = new FontLabelLayout;
    mainFontLabel->setFont(options_.mainFont);
    titleFontLabel = new FontLabelLayout;
    titleFontLabel->setFont(options_.titleFont);
    artistFontLabel = new FontLabelLayout;
    artistFontLabel->setFont(options_.artistFont);
    otherFontLabel = new FontLabelLayout;
    otherFontLabel->setFont(options_.otherFont);

    //Create form
    formLayout->addRow(tr("Default output folder:"),defaultOutputFolder);
    formLayout->addRow(tr("Enable colors on printing:"),checkEnableColorsOnPrinting);
    formLayout->addRow(tr("Print header on each pages:"),checkPrintHeaderOnEachPages);
    formLayout->addRow(tr("Title font:"),titleFontLabel);
    formLayout->addRow(tr("Artist font:"),artistFontLabel);
    formLayout->addRow(tr("Other font:"),otherFontLabel);
    formLayout->addRow(tr("Text font:"),mainFontLabel);
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
    options_.autoCreateFolder = autoCreateFolder->isChecked();
    options_.defaultOutputFolder = defaultOutputFolder->getText();
    options_.openSizeMode = comboOpenSize->currentIndex();
    options_.mainFont = mainFontLabel->getFont();
    options_.titleFont = titleFontLabel->getFont();
    options_.artistFont = artistFontLabel->getFont();
    options_.otherFont = otherFontLabel->getFont();
    options_.chordSize = Options::toSize( comboChordSize->currentText() );
    options_.printHearderOnEachPages = checkPrintHeaderOnEachPages->isChecked();
    options_.enableColorsOnPrinting = checkEnableColorsOnPrinting->isChecked();
    options_.mainToolBarPosition = comboToolBarPosition->currentIndex();
    options_.topMargin = editTop->value();
    options_.leftMargin = editLeft->value();
    options_.rightMargin = editRight->value();
    options_.bottomMargin = editBottom->value();
    options_.nbSpaceForOneTab = spinNbSpaceForOneTab->value();

    options_.colors = cref->getListRegExp();

    return options_;
}

void OptionsForm::buttonClicked(QAbstractButton *button){
    if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){

        //Create a static function
        Options *opt = new Options;
        options_ = opt->cloneValues();

        checkEnableColors->setChecked(options_.enableColors);
        checkSelectNewTab->setChecked(options_.selectNewTab);
        checkOpenReadOnly->setChecked(options_.openReadOnly);
        checkreOpenPreviousTabs->setChecked(options_.reOpenPreviousTabs);
        editDefaultFolder->setText(options_.defaultPath);
        autoCreateFolder->setChecked(options_.autoCreateFolder);
        defaultOutputFolder->setText(options_.defaultOutputFolder);
        comboOpenSize->setCurrentIndex(options_.openSizeMode);
        mainFontLabel->setFont(options_.mainFont);
        titleFontLabel->setFont(options_.titleFont);
        artistFontLabel->setFont(options_.artistFont);
        otherFontLabel->setFont(options_.otherFont);
        checkPrintHeaderOnEachPages->setChecked(options_.printHearderOnEachPages);
        checkEnableColorsOnPrinting->setChecked(options_.enableColorsOnPrinting);
        spinNbSpaceForOneTab->setValue(options_.nbSpaceForOneTab);

        editTop->setValue( options_.topMargin );
        editLeft->setValue( options_.leftMargin );
        editRight->setValue( options_.rightMargin );
        editBottom->setValue( options_.bottomMargin );

        cref->clear();
        for(int i=0;i<options_.colors.size();i++)
            cref->addColorRegExp(options_.colors.at(i).regExp,
                                 options_.colors.at(i).color,
                                 options_.colors.at(i).weight,
                                 options_.colors.at(i).italic,
                                 options_.colors.at(i).isText,
                                 options_.colors.at(i).caseSensitivity?Qt::CaseSensitive:Qt::CaseInsensitive,
                                 options_.colors.at(i).active);

        cref->setEditFont(options_.mainFont);
    }
}
