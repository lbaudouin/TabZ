#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QString>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

struct ColorRegExp
{
    ColorRegExp(QString _regExp, QColor _color, int _weight, int _isText){
        regExp = _regExp;
        color = _color;
        weight = _weight;
        isText = _isText;
    }

    QString regExp;
    QColor color;
    int weight;
    int isText;
};

struct OptionsValues
{
    OptionsValues();
    bool enableColors;
    bool selectNewTab;
    bool openReadOnly;
    QString defaultPath;

    QList<ColorRegExp> colors;

    void save(QWidget* parent = 0);
    void parse(QWidget* parent = 0);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data);
    void addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data);
};

class Options : public QDialog
{
    Q_OBJECT
public:
    explicit Options(OptionsValues options, QWidget *parent = 0);
    OptionsValues getOptions();

private:
    OptionsValues options_;

    QCheckBox *checkEnableColor;
    QCheckBox *checkSelectNewTab;
    QCheckBox *checkOpenReadOnly;
    QLineEdit *editDefaultFolder;

protected:
    void createColorTab(QTabWidget *tab);
    void createOptionsTab(QTabWidget *tab);
    
signals:
    
public slots:
    
};

#endif // OPTIONS_H
