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
#include <QtNetwork>
#include <QDesktopServices>
#include <QUrl>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QMovie>

#include <QDebug>

class DownloadXTA : public QDialog
{
    Q_OBJECT
public:
    explicit DownloadXTA(QWidget *parent = 0);
    
private:
    QTreeView *tree;
    QNetworkAccessManager *manager;
    QLineEdit *edit;

    QFile* xta_file;
    int xta_id;
    QMap<int,QFile*> files;

    QLabel *imageLabel,*statusLabel;
    QWidget *statusWidget;

    QNetworkReply* listXtaReply;

protected:
    void parse(QString content);

signals:
    
public slots:
    void pressDownload();
    void setFolder(QString path);

private slots:
    void itemChanged(QStandardItem *item);
    void selectFolder();

    void replyFinished(QNetworkReply *reply);

};

#endif // DOWNLOADXTA_H
