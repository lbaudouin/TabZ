#include "mycheckabletree.h"

MyCheckableTree::MyCheckableTree(QWidget *parent) :
    QWidget(parent)
{
    tree = new QTreeWidget;
    tree->setColumnCount(1);
    tree->setHeaderHidden(true);

    root = new QTreeWidgetItem(tree);
    root->setCheckState(0,Qt::Checked);

    tree->expandAll();

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(tree);

    connect(tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));
}

void MyCheckableTree::setRootLabel(QString label)
{
    root->setText(0,label);
}

void MyCheckableTree::addRow(QString label, QVariant var, bool checked)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(root, QStringList(label));
    if(checked)
        item->setCheckState(0,Qt::Checked);
    else
        item->setCheckState(0,Qt::Unchecked);
    item->setData(0,Qt::UserRole,var);
}

void MyCheckableTree::setHeaderLabel(QString label)
{
    tree->setHeaderLabel(label);
    tree->setHeaderHidden(!label.isEmpty());
}

/*QAbstractItemModel* MyCheckableTree::model()
{
    return tree->model();
}*/

void MyCheckableTree::itemChanged(QTreeWidgetItem *item,int /*col*/)
{
    disconnect(tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));
    if(item==root){
        if(root->checkState(0)==Qt::Checked){
            for(int i=0;i<root->childCount();i++){
                root->child(i)->setCheckState(0,Qt::Checked);
            }
        }else if(root->checkState(0)==Qt::Unchecked){
            for(int i=0;i<root->childCount();i++){
                root->child(i)->setCheckState(0,Qt::Unchecked);
            }
        }
    }else{
        int nbChecked =0;
        int nbUnchecked = 0;
        for(int i=0;i<root->childCount();i++){
            if(root->child(i)->checkState(0)==Qt::Checked)
                nbChecked++;
            else
                nbUnchecked++;
        }
        if(nbChecked==0)
            root->setCheckState(0,Qt::Unchecked);
        else if(nbUnchecked==0)
            root->setCheckState(0,Qt::Checked);
        else
            root->setCheckState(0,Qt::PartiallyChecked);

    }
    connect(tree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));
}

int MyCheckableTree::nbChild()
{
    return root->childCount();
}

QVariant MyCheckableTree::getData(int index)
{
    return root->child(index)->data(0,Qt::UserRole);
}

Qt::CheckState MyCheckableTree::getState(int index)
{
    return root->child(index)->checkState(0);
}
