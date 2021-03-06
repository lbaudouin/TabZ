#include "lilypond.h"

Lilypond::Lilypond(QWidget *parent) :
    QDialog(parent)
{
    this->setMinimumSize(800,600);
    this->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->setWindowTitle(tr("Insert Lilypond tab"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *secondLayout = new QHBoxLayout;
    edit = new QTextEdit;
    edit->setMinimumWidth(250);
    label = new QLabel;
    checkBox = new QCheckBox(tr("Auto-generate"));
    QPushButton *button = new QPushButton(tr("Generate"));
    button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    QVBoxLayout *thirdLayout = new QVBoxLayout;
    QDialogButtonBox *dialogButton = new QDialogButtonBox(QDialogButtonBox::Cancel);
    okButton = dialogButton->addButton(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    thirdLayout->addWidget(label);
    thirdLayout->addWidget(checkBox,0,Qt::AlignHCenter);
    thirdLayout->addWidget(button,0,Qt::AlignHCenter);
    secondLayout->addWidget(edit);
    secondLayout->addLayout(thirdLayout);
    mainLayout->addLayout(secondLayout);
    mainLayout->addWidget(dialogButton);

    connect(dialogButton,SIGNAL(accepted()),this,SLOT(accept()));
    connect(dialogButton,SIGNAL(rejected()),this,SLOT(reject()));
    connect(edit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    connect(button,SIGNAL(clicked()),this,SLOT(generate()));
    connect(checkBox,SIGNAL(clicked()),SLOT(generate()));

    QString sample = "\\version \"2.14.2\"\n"
            "\\paper{\n"
            "  indent=0\\mm\n"
            "  line-width=120\\mm\n"
            "  oddFooterMarkup=##f\n"
            "  oddHeaderMarkup=##f\n"
            "  bookTitleMarkup = ##f\n"
            "  scoreTitleMarkup = ##f\n"
            "}\n"
            "\\new TabStaff \\relative c' {\n"
            "  a,8 a' <c e> a\n"
            "  d,8 a' <d f> a\n"
            "}\n"
            "\n"
            "symbols = {\n"
            "  \\time 3/4\n"
            "  c4-.^\"Allegro\" d( e)\n"
            "  f4-.\\f g a^\\fermata\n"
            "  \\mark \\default\n"
            "  c8_.\\<\\( c16 c ~ c2\\!\n"
            "  c'2.\\prall\\)\n"
            "}\n"
            "\n"
            "\\score {\n"
            "  \\new TabStaff {\n"
            "    \\tabFullNotation\n"
            "    \\symbols\n"
            "  }\n"
            "}\n";
    edit->setPlainText(sample);

    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(generate()));

    timer->start(25);
}

bool Lilypond::isLilypondAvailable()
{
    QProcess process;
    process.start("which lilypond");
    process.waitForFinished();
    QString path(process.readAllStandardOutput());

    return !path.isEmpty();
}

bool Lilypond::downloadLilypond()
{
    int ret = QMessageBox::question(this,tr("Download Lilypond"),tr("Do you wan't to download Lilypond software?\nCommand: 'gksudo apt-get -y install lilypond'"),QMessageBox::No,QMessageBox::Yes);
    if(ret==QMessageBox::Yes){
        this->setCursor(Qt::WaitCursor);

        installProcess = new QProcess;

        connect(installProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished(int,QProcess::ExitStatus)));

        installProcess->start("gksudo \"apt-get -y install lilypond\"");

        //bool success = installProcess->waitForFinished(-1);

        QMessageBox::information(this,tr("Information"),tr("Lilypond installion is currently working, a message will inform you when the process is complete."));

        this->setCursor(Qt::ArrowCursor);

        return true;
    }
    return false;
}

void Lilypond::textChanged()
{
    okButton->setEnabled(false);
    if(checkBox->isChecked()){
        timer->start(500);
    }
}

void Lilypond::generate()
{
    QFile::remove("/tmp/TabZ.png");

    QFile file("/tmp/TabZ.ly");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);
    stream << edit->toPlainText();
    file.close();

    this->setCursor(Qt::WaitCursor);
    QProcess process;
    process.start("lilypond -dbackend=eps -dno-gs-load-fonts -dinclude-eps-fonts --output=/tmp/TabZ --png /tmp/TabZ.ly");
    process.waitForFinished();

    //QString outS(process.readAllStandardOutput());
    //qDebug() << outS;
    //QString outE(process.readAllStandardError());
    //qDebug() << outE;

    if(QFile::exists("/tmp/TabZ.png")){
        QPixmap pix("/tmp/TabZ.png");
        label->setPixmap(pix);

        image.load("/tmp/TabZ.png");

        label->setMinimumSize( pix.size() );
        okButton->setEnabled(true);
    }

    this->setCursor(Qt::ArrowCursor);
}

void Lilypond::finished(int,QProcess::ExitStatus status)
{
    if(status==QProcess::NormalExit){
        QMessageBox::information(this,tr("Information"),tr("Lilypond was installed successfully"));
    }else{
        QMessageBox::warning(this,tr("Warning"),tr("The Lilypond installation has failed"));
    }
}
