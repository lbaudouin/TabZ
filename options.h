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

struct OptionsValues
{
    OptionsValues() : enableColors(true) {}
    bool enableColors;
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

protected:
    void createColorTab(QTabWidget *tab);
    void createOptionsTab(QTabWidget *tab);
    
signals:
    
public slots:
    
};

#endif // OPTIONS_H
