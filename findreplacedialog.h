#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpacerItem>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>

#include <QDebug>

class FindReplaceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindReplaceDialog(QWidget *parent = 0);
    void setTextEdit(QTextEdit *);

protected:
    void displayError(const QString &error);
    void displayMessage(const QString &message);

    void validateRegExp(const QString &text);

private:
    QLineEdit *findEdit,*replaceEdit;
    QLabel *errorLabel,*replaceLabel;
    QGroupBox *optionsGroupBox,*directionGroupBox;
    QCheckBox *caseCheckBox,*wordCheckBox,*regexpCheckBox;
    QRadioButton *downRadioButton,*upRadioButton;
    QPushButton *findButton,*replaceButton,*replaceAllButton;

    QTextEdit *textEdit;
    
signals:
    
public slots:
    void findTextChanged(QString);
    void setFindOnly(bool);
    void regexpStateChanged(bool ckeched);

    void find();
    void replace();
    void replaceAll();
    
};

#endif // FINDREPLACEDIALOG_H
