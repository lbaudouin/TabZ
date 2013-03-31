#include "options.h"

OptionsValues::OptionsValues()
{
    openReadOnly = false;
    enableColors = true;
    selectNewTab = true;
    defaultPath = "";
}

void OptionsValues::addNode(QDomDocument &dom, QDomElement &parent, QString tag, QString data)
{
    QDomElement node = dom.createElement(tag);
    parent.appendChild(node);

    QDomText textNode = dom.createTextNode(data);
    node.appendChild(textNode);
}

void OptionsValues::addNode(QDomDocument &dom, QDomElement &parent, QString tag, int data)
{
    addNode(dom,parent,tag,QString::number(data));
}

void OptionsValues::save(QWidget *parent)
{
    QFile file("options.xml");
    file.open(QFile::WriteOnly);

    QTextStream stream(&file);

    QDomDocument dom;

    QDomElement mainNode = dom.createElement("options");
    dom.appendChild(mainNode);

    {

        addNode(dom,mainNode,"enableColors",enableColors);
        addNode(dom,mainNode,"defaultPath",defaultPath);
        addNode(dom,mainNode,"selectNewTab",selectNewTab);
        addNode(dom,mainNode,"openReadOnly",openReadOnly);


        QDomElement colorNode = dom.createElement("colors");
        mainNode.appendChild(colorNode);

        //foreach(const ColorRegExp &colorRegExp, colors){
        foreach(ColorRegExp colorRegExp, colors){
            QDomElement elem = dom.createElement("ColorRegExp");
            elem.setAttribute("regexp",colorRegExp.regExp);
            elem.setAttribute("color",QString("%1,%2,%3").arg(QString::number(colorRegExp.color.red()),QString::number(colorRegExp.color.green()),QString::number(colorRegExp.color.blue())));
            elem.setAttribute("weight",colorRegExp.weight);
            elem.setAttribute("text",colorRegExp.isText);
            colorNode.appendChild(elem);
        }

    }

    stream << dom.toString();

    file.close();
}

void OptionsValues::parse(QWidget *parent)
{
    QDomDocument *dom = new QDomDocument("docXML");
    QFile xml_doc("options.xml");

    if(!xml_doc.open(QIODevice::ReadOnly)){
        if(parent)
            QMessageBox::warning(parent,parent->tr("Failed to open XML document"),parent->tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(xml_doc.fileName()));
        return;
    }
    if (!dom->setContent(&xml_doc)){
        xml_doc.close();
        if(parent)
            QMessageBox::warning(parent,parent->tr("Error opening the XML document"),parent->tr("The XML document could not be assigned to the object QDomDocument."));
        return;
    }

    QDomElement dom_element = dom->documentElement();
    QDomNode node = dom_element.firstChild();


    while(!node.isNull())
    {
        QDomElement element = node.toElement();

        if(element.tagName()=="enableColors"){
            enableColors = element.text().toInt();
        }
        if(element.tagName()=="selectNewTab"){
            selectNewTab = element.text().toInt();
        }
        if(element.tagName()=="openReadOnly"){
            openReadOnly = element.text().toInt();
        }
        if(element.tagName()=="defaultPath"){
            defaultPath = element.text();
        }

        if(element.tagName()=="colors"){
            QDomNode child = node.firstChild();

            while(!child.isNull()){

                QDomElement colorElement = child.toElement();
                if(colorElement.tagName()=="ColorRegExp"){
                    QString regExp = colorElement.attribute("regexp");
                    QString colorString = colorElement.attribute("color");
                    QString weightText =  colorElement.attribute("weight");
                    int isText = colorElement.attribute("text").toInt();

                    QStringList colorList = colorString.split(",");
                    QColor color(0,0,0);
                    if(colorList.size()==3){
                        color.setRgb(colorList[0].toInt(),colorList[1].toInt(),colorList[2].toInt());
                    }

                    ColorRegExp cre(regExp,color,weightText.isEmpty()?50:weightText.toInt(),isText);

                    colors.push_back(cre);
                }

                child =child.nextSibling();
            }
        }

        node = node.nextSibling();
    }

    xml_doc.close();

}


Options::Options(OptionsValues options, QWidget *parent) : options_(options),
    QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);

    createColorTab(tabWidget);
    createOptionsTab(tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::RestoreDefaults|QDialogButtonBox::Cancel|QDialogButtonBox::Save);
    connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));

    mainLayout->addWidget(buttonBox);
}

void Options::createColorTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    checkEnableColor = new QCheckBox;
    checkEnableColor->setChecked(options_.enableColors);
    formLayout->addRow(tr("Enable color:"),checkEnableColor);

    tab->addTab(w,tr("Color"));
}

void Options::createOptionsTab(QTabWidget *tab)
{
    QWidget *w = new QWidget;

    QFormLayout *formLayout = new QFormLayout;
    w->setLayout(formLayout);

    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("TEST");

    editDefaultFolder = new QLineEdit;
    editDefaultFolder->setText(options_.defaultPath);

    checkSelectNewTab = new QCheckBox;
    checkSelectNewTab->setChecked(options_.selectNewTab);

    checkOpenReadOnly = new QCheckBox;
    checkOpenReadOnly->setChecked(options_.openReadOnly);

    formLayout->addRow(tr("Default folder:"),editDefaultFolder);
    formLayout->addRow(tr("Select new tab:"),checkSelectNewTab);
    formLayout->addRow(tr("Open read only:"),checkOpenReadOnly);



    tab->addTab(w,tr("Options"));
}

OptionsValues Options::getOptions()
{
    options_.enableColors = checkEnableColor->isChecked();
    options_.selectNewTab = checkSelectNewTab->isChecked();
    options_.openReadOnly = checkOpenReadOnly->isChecked();
    options_.defaultPath = editDefaultFolder->text();
    return options_;
}
