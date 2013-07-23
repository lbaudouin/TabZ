#include "findreplacedialog.h"

FindReplaceDialog::FindReplaceDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("Find/Replace"));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *secondLayout = new QVBoxLayout();

    QFormLayout *findReplaceLayout = new QFormLayout;
    findEdit = new QLineEdit;
    replaceEdit = new QLineEdit;
    replaceLabel = new QLabel(tr("Replace with:"));
    findReplaceLayout->addRow(tr("Find:"),findEdit);
    findReplaceLayout->addRow(replaceLabel,replaceEdit);
    secondLayout->addLayout(findReplaceLayout);

    errorLabel = new QLabel;
    errorLabel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    secondLayout->addWidget(errorLabel);

    optionsGroupBox = new QGroupBox(this);
    optionsGroupBox->setTitle(tr("Options"));
    caseCheckBox = new QCheckBox(tr("Case sensitive"),optionsGroupBox);
    wordCheckBox = new QCheckBox(tr("Entire word only"),optionsGroupBox);
    regexpCheckBox = new QCheckBox(tr("Use RegExp"),optionsGroupBox);

    directionGroupBox = new QGroupBox(this);
    directionGroupBox->setTitle(tr("Direction"));
    downRadioButton = new QRadioButton(tr("Down"),directionGroupBox);
    upRadioButton = new QRadioButton(tr("Up"),directionGroupBox);
    downRadioButton->setChecked(true);

    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroupBox);
    optionsLayout->addWidget(caseCheckBox);
    optionsLayout->addWidget(wordCheckBox);
    optionsLayout->addWidget(regexpCheckBox);

    QVBoxLayout *directionLayout = new QVBoxLayout(directionGroupBox);
    directionLayout->addWidget(upRadioButton);
    directionLayout->addWidget(downRadioButton);

    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(optionsGroupBox);
    checkBoxLayout->addWidget(directionGroupBox);
    secondLayout->addLayout(checkBoxLayout);

    QVBoxLayout *buttonLayout = new QVBoxLayout;

    findButton = new QPushButton(tr("Find"));
    replaceButton = new QPushButton(tr("Replace"));
    replaceAllButton = new QPushButton(tr("Replace All"));
    QPushButton *closeButton = new QPushButton(tr("Close"));
    QSpacerItem *spacer = new QSpacerItem(0,5,QSizePolicy::Minimum,QSizePolicy::Expanding);

    buttonLayout->addWidget(findButton);
    buttonLayout->addWidget(replaceButton);
    buttonLayout->addWidget(replaceAllButton);
    buttonLayout->addSpacerItem(spacer);
    buttonLayout->addWidget(closeButton);


    mainLayout->addLayout(secondLayout);
    mainLayout->addLayout(buttonLayout);

    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(findEdit,SIGNAL(textChanged(QString)),this,SLOT(findTextChanged(QString)));
    connect(regexpCheckBox,SIGNAL(toggled(bool)),this,SLOT(regexpStateChanged(bool)));

    connect(findButton,SIGNAL(clicked()),this,SLOT(find()));
    connect(replaceButton,SIGNAL(clicked()),this,SLOT(replace()));
    connect(replaceAllButton,SIGNAL(clicked()),this,SLOT(replaceAll()));
}

void FindReplaceDialog::setTextEdit(QTextEdit *edit)
{
    textEdit = edit;
    if(textEdit->textCursor().hasSelection()){
        findEdit->setText(textEdit->textCursor().selectedText());
    }
    textEdit->textCursor().clearSelection();
    //connect(textEdit, SIGNAL(copyAvailable(bool)), replaceButton, SLOT(setEnabled(bool)));
    //connect(textEdit, SIGNAL(copyAvailable(bool)), replaceAllButton, SLOT(setEnabled(bool)));
}

void FindReplaceDialog::setFindOnly(bool findOnly)
{
    replaceLabel->setHidden(findOnly);
    replaceEdit->setHidden(findOnly);
    replaceButton->setHidden(findOnly);
    replaceAllButton->setHidden(findOnly);
    //regexpCheckBox->setHidden(findOnly);
}

void FindReplaceDialog::regexpStateChanged(bool ckeched)
{
    if (ckeched)
        validateRegExp(findEdit->text());
    else
        validateRegExp("");
}

void FindReplaceDialog::validateRegExp(const QString &text)
{
    if (!regexpCheckBox->isChecked() || text.size() == 0) {
        errorLabel->setText("");
        return;
    }

    QRegExp regexp(text, (caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));

    if(regexp.isValid()){
        errorLabel->clear();
    }else{
        displayError(regexp.errorString());
    }
}

void FindReplaceDialog::displayError(const QString &error)
{
    if(error.isEmpty()){
        errorLabel->clear();
    }else{
        errorLabel->setText("<span style=\" font-weight:600; color:#ff0000;\">" + error + "</span>");
    }
}

void FindReplaceDialog::displayMessage(const QString &message)
{
    if(message.isEmpty()) {
        errorLabel->clear();
    }else{
        errorLabel->setText("<span style=\" font-weight:600; color:green;\">" + message + "</span>");
    }
}

void FindReplaceDialog::findTextChanged(QString text)
{
    findButton->setEnabled(!text.isEmpty());
    validateRegExp(text);
}

void FindReplaceDialog::find()
{
    if(!textEdit){
        displayError(tr("Impossible to search"));
        return;
    }

    QTextDocument::FindFlags searchFlags;
    if(upRadioButton->isChecked())
        searchFlags |= QTextDocument::FindBackward;
    if(caseCheckBox->isChecked())
        searchFlags |= QTextDocument::FindCaseSensitively;
    if(wordCheckBox->isChecked())
        searchFlags |= QTextDocument::FindWholeWords;

    bool result = false;
    if(regexpCheckBox->isChecked()){
        QRegExp regexp(findEdit->text(), (caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));

        QTextCursor textCursor = textEdit->document()->find(regexp, textCursor, searchFlags);
        textEdit->setTextCursor(textCursor);
        result = (!textCursor.isNull());
    }else{
        result = textEdit->find(findEdit->text(), searchFlags);
    }

    if (result) {
        errorLabel->clear();
    } else {
        displayError(tr("No match found"));
        //textCursor.setPosition(0);
        //textEdit->setTextCursor(textCursor);
    }
}

void FindReplaceDialog::replace(){
    if(!textEdit->textCursor().hasSelection()) {
        find();
    }else{
        textEdit->textCursor().insertText(replaceEdit->text());
        find();
    }
}

void FindReplaceDialog::replaceAll(){
    int i=0;
    while(textEdit->textCursor().hasSelection()){
        textEdit->textCursor().insertText(replaceEdit->text());
        find();
        i++;
    }
    displayMessage(tr("Replaced %n occurrence(s)","",i));
}
