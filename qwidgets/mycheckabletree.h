#ifndef MYCHECKABLETREE_H
#define MYCHECKABLETREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <QDebug>

class MyCheckableTree : public QWidget
{
    Q_OBJECT
public:
    explicit MyCheckableTree(QWidget *parent = 0);
    
    //QAbstractItemModel* model();
    int nbChild();
    QVariant getData(int index);
    Qt::CheckState getState(int index);

private:
    QTreeWidget *tree;
    QTreeWidgetItem *root;

signals:
    
public slots:
    void setRootLabel(QString label);
    void setHeaderLabel(QString label);
    void addRow(QString label, QVariant var = QVariant(), bool checked = true);

    void itemChanged(QTreeWidgetItem*,int);
    
};

#endif // MYCHECKABLETREE_H
