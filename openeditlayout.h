#ifndef OPENEDITLAYOUT_H
#define OPENEDITLAYOUT_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QLineEdit>
#include <QFileDialog>

class OpenEditLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    enum OpenMode { FileMode = 0, SaveMode, FolderMode, ListMode };
    OpenEditLayout(OpenMode mode, bool buttonOnLeft = false, QWidget *parent = 0) : QHBoxLayout(parent)
    {
        mode_ = mode;
        title_ = tr("Select a file");

        QToolButton *button = new QToolButton;
        button->setIcon(QIcon(":images/open"));
        edit = new QLineEdit;
        edit->setReadOnly(true);

        if(buttonOnLeft){
            this->addWidget(button);
            this->addWidget(edit);
        }else{
            this->addWidget(edit);
            this->addWidget(button);
        }

        connect(button,SIGNAL(clicked()),this,SLOT(pressOpen()));
        //connect(this,SIGNAL(pathChanged(QString)),edit,SLOT(setText(QString)));
    }

    QLineEdit* getEditWidget() { return edit; }

private:
    QLineEdit *edit;
    OpenMode mode_;
    QString title_;
    QString folder_;
    QString filter_;

signals:
    //void pathChanged(QString path);

public slots:
    void setTitle(QString title) { title_ = title; }
    void setFolder(QString folder) { folder_ = folder; }
    void setFilter(QString filter) { filter_ = filter; }
    void setPath(QString path) { /*emit pathChanged(path);*/ edit->setText(path); }

    void pressOpen(){
        QString path;
        switch(mode_){
        case FileMode: path = QFileDialog::getOpenFileName(this->widget(),title_,folder_,filter_); break;
        case SaveMode: path = QFileDialog::getSaveFileName(this->widget(),title_,folder_,filter_); break;
        case FolderMode: path = QFileDialog::getExistingDirectory(this->widget(),title_,folder_); break;
        case ListMode: path = QFileDialog::getOpenFileNames(this->widget(),title_,folder_,filter_).join("; "); break;
        }
        //emit this->setPath(path);
        if(!path.isEmpty())
            edit->setText(path);
    }
};

#endif // OPENEDITLAYOUT_H
