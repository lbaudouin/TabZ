#ifndef DOWNLOADXTA_H
#define DOWNLOADXTA_H

#include <QDialog>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHttp>
#include <QUrl>
#include <QLineEdit>
#include <QToolButton>

#include <QDebug>

class DownloadXTA : public QDialog
{
    Q_OBJECT
public:
    explicit DownloadXTA(QWidget *parent = 0);
    
private:
    QTreeView *tree;
    QHttp *http;
    QLineEdit *edit;

    QFile* xta_file;
    int xta_id;
    QMap<int,QFile*> files;

protected:
    void parse(QString filepath);

signals:
    
public slots:
    void pressDownload();
    void setFolder(QString path);

private slots:
    void itemChanged(QStandardItem *item);
    void downloadFinished(int id, bool error);
    void selectFolder();
    
};

#endif // DOWNLOADXTA_H
