#include "chordsmanager.h"

ChordsManager::ChordsManager(Chords* chords, QWidget *parent) : QDialog(parent), chords_(chords)
{
    this->setWindowTitle(tr("Chords Manager"));
    this->setMinimumSize(QSize(800,600));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
    connect(buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonClicked(QAbstractButton*)));
    mainLayout->addWidget(buttonBox);
}

void ChordsManager::buttonClicked(QAbstractButton *button){
    QDialogButtonBox* buttonBox = (QDialogButtonBox*)sender();
    if(buttonBox->buttonRole(button)==QDialogButtonBox::ResetRole){
        qDebug() << "Restore";
    }
}

void ChordsManager::save()
{
    chords_->save();
}

void ChordsManager::updateManager()
{
    if(chords_==0){
        return;
    }

    tabWidget->clear();
    QList<Instrument> instruments = chords_->getInstruments();
    foreach(Instrument instrument, instruments){
        QScrollArea *area = new QScrollArea();
        QList<Chord> list = chords_->getChords(instrument);

        QGridLayout *gridLayout = new QGridLayout;

        area->setWidgetResizable(true);
        QWidget *w = new QWidget;
        w->setLayout(gridLayout);

        area->setWidget(w);

        int nb = this->width() / 200;

        for(int i=0;i<list.size();i++){
            Guitar *guitar = new Guitar(list[i].getName(), list[i].getFingers());
            guitar->setMenu(!list[i].isLocked(),false,false);
            gridLayout->addWidget( guitar , i/nb, i%nb);
        }

        int index =  tabWidget->addTab(area, instrument.name );

        if(instrument.label=="guitar"){
            tabWidget->setCurrentIndex(index);
        }

    }
}

void ChordsManager::resizeEvent(QResizeEvent *)
{
    updateManager();
}

Chord ChordsManager::addNewChord(QWidget *parent)
{
    if(parent==0)
        parent = this;

    QDialog *diag = new QDialog(parent);
    diag->setWindowTitle(tr("Add new chord"));
    QVBoxLayout *vLayout = new QVBoxLayout;
    diag->setLayout(vLayout);
    //diag->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    diag->setFixedSize(250,375);

    QComboBox *instrumentCombo = new QComboBox;
    instrumentCombo->addItems(chords_->getInstrumentsNames());
    instrumentCombo->setCurrentIndex(chords_->getInstrumentsNames().indexOf(tr("Guitar")));

    QLineEdit *nameEdit = new QLineEdit;
    QComboBox *fingersCombo = new QComboBox;
    fingersCombo->setEditable(true);
    //fingersCombo->addItems( mapChord.values() );
    fingersCombo->setCurrentIndex(-1);


    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Instrument"),instrumentCombo);
    formLayout->addRow(tr("Name"),nameEdit);
    formLayout->addRow(tr("Fingers"),fingersCombo);
    vLayout->addLayout(formLayout);

    Strings *strings = new Strings("");
    strings->setSize(QSize(150,200));
    vLayout->addWidget(strings,0,Qt::AlignHCenter);

    QSpacerItem *spacer = new QSpacerItem(0,10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);

    connect(fingersCombo,SIGNAL(editTextChanged(QString)),strings,SLOT(setFingers(QString)));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox,SIGNAL(accepted()),diag,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),diag,SLOT(reject()));
    vLayout->addWidget(buttonBox);

    ChordUnlocked chord;

    while(1){
        if(!diag->exec()){
            break;
        }
        QString name = nameEdit->text();
        if(name.isEmpty()){
            QMessageBox::warning(parent,tr("Warning"),tr("Invalid name"));
            continue;
        }

        QString fingers = fingersCombo->currentText();

        fingers.replace(","," ");
        fingers.replace("\t"," ");

        chord.setName(name);
        chord.setFingers(fingers);
        chord.setComment("");

        break;
    }

    return chord;
}
