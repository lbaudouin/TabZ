#include "downloadxta.h"

DownloadXTA::DownloadXTA(QWidget *parent) :
    QDialog(parent)
{
    this->setMinimumSize(500,600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    tree = new QTreeView;
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    edit = new QLineEdit;
    edit->setText(QString("xta") + QDir::separator());
    QHBoxLayout *folderLayout = new QHBoxLayout;
    QToolButton *toolFolder = new QToolButton;
    toolFolder->setIcon(QIcon(":images/open"));
    folderLayout->addWidget(toolFolder);
    folderLayout->addWidget(edit);

    statusWidget = new QWidget;
    QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
    imageLabel = new QLabel;
    QMovie* movie = new QMovie(":images/progress");
    movie->start();
    imageLabel->setMovie(movie);
    statusLabel = new QLabel(tr("Downloading..."));
    statusLayout->addWidget(imageLabel);
    statusLayout->addWidget(statusLabel);
    QSpacerItem *spacer = new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Ignored);
    statusLayout->addSpacerItem(spacer);

    mainLayout->addWidget(statusWidget);
    mainLayout->addWidget(tree);
    mainLayout->addLayout(folderLayout);
    mainLayout->addWidget(buttons);

    connect(toolFolder,SIGNAL(clicked()),this,SLOT(selectFolder()));
    connect(buttons,SIGNAL(accepted()),this,SLOT(pressDownload()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(reject()));

    http = new QHttp;
    http->setHost("lbaudouin.chez.com");
    connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(downloadFinished(int,bool)));

    xta_file = new QFile("xta.xml");
    xta_file->open(QFile::WriteOnly);
    xta_id = http->get("xta.xml",xta_file);

}

void DownloadXTA::parse(QString path)
{
    QDomDocument *dom = new QDomDocument("docXML");

    if(!QFile::exists(path)){
        return;
    }

    QFile xml_doc(path);

    if(!xml_doc.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Failed to open XML document"),tr("The XML document '%1' could not be opened. Verify that the name is correct and that the document is well placed.").arg(xml_doc.fileName()));
        return;
    }
    if(!dom->setContent(&xml_doc)){
        xml_doc.close();
        QMessageBox::warning(this,tr("Error opening the XML document"),tr("The XML document could not be assigned to the object QDomDocument."));
        return;
    }

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *rootItem = model->invisibleRootItem();
    rootItem->setEditable(false);

    QDomElement dom_element = dom->documentElement();
    QDomNode artistNode = dom_element.firstChild();

    while(!artistNode.isNull())
    {
        QDomElement element = artistNode.toElement();

        if(element.tagName()=="artist"){
            QString artist = element.attribute("name");

            QStandardItem *artistItem = new QStandardItem(artist);
            artistItem->setCheckable(true);
            artistItem->setEditable(false);
            artistItem->setData(true);
            rootItem->appendRow(artistItem);

            QDomNode songNode = artistNode.firstChild();

            while(!songNode.isNull()){

                QDomElement songElement = songNode.toElement();
                if(songElement.tagName()=="song"){
                    QString title = songElement.attribute("title");
                    QString comment = songElement.attribute("comment");
                    QString path = songElement.attribute("file");

                    QStandardItem *songItem = new QStandardItem(title);
                    songItem->setCheckable(true);
                    songItem->setEditable(false);
                    songItem->setData(path);

                    QStandardItem *commentItem = new QStandardItem(comment);
                    commentItem->setCheckable(false);
                    commentItem->setEditable(false);

                    artistItem->appendRow(QList<QStandardItem*>() << songItem << commentItem);
                }

                songNode = songNode.nextSibling();
            }
        }

        artistNode = artistNode.nextSibling();
    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Available songs") /*<< tr("Comment")*/);
    tree->setModel(model);

    xml_doc.close();

    connect(model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(itemChanged(QStandardItem*)));
}

void DownloadXTA::itemChanged(QStandardItem *item)
{
    for(int i=0;;i++){
        QStandardItem *c = item->child(i);
        if(c==0) return;
        c->setCheckState(item->checkState());
    }
}

void DownloadXTA::pressDownload()
{
    int rows = tree->model()->rowCount();

    for(int i=0;i<rows;i++){
        QStandardItem *item =((QStandardItemModel*)tree->model())->item(i);
        for(int j=0;;j++){
            QStandardItem *c = item->child(j);
            if(c==0) break;
            if(c->checkState()){
                //Create path
                QString path = c->data().toString();
                path.remove(0,4); //remove "xta/"
                //Create file info
                QFileInfo fi(edit->text() + QDir::separator() + path);
                //Create folder
                QDir dir;
                dir.mkpath(fi.absoluteDir().path());
                //Create file
                QFile *file = new QFile(fi.absoluteFilePath());
                file->open(QFile::WriteOnly);
                //Download file
                int id = http->get(QUrl("/"+c->data().toString()).encodedPath(),file);
                files.insert(id,file);
            }
        }
    }
    this->accept();
}

void DownloadXTA::downloadFinished(int id, bool error)
{
    if(xta_id==id){
        xta_file->close();
        statusLabel->setText(tr("Parsing..."));
        parse("xta.xml");
        statusWidget->hide();
        imageLabel->movie()->stop();
    }

    if(files.contains(id))
        files[id]->close();

    if(!http->currentRequest().isValid())
        return;

    if(error){
        QString errorText;

        switch(http->error()){
        case QHttp::HostNotFound : errorText = tr("The host name lookup failed"); break;
        case QHttp::ConnectionRefused : errorText = tr("The server refused the connection"); break;
        case QHttp::UnexpectedClose : errorText = tr("The server closed the connection unexpectedly"); break;
        case QHttp::InvalidResponseHeader : errorText = tr("The server sent an invalid response header"); break;
        case QHttp::WrongContentLength : errorText = tr("The client could not read the content correctly because an error with respect to the content length occurred"); break;
        case QHttp::Aborted : errorText = tr("The request was aborted with abort()"); break;
        case QHttp::ProxyAuthenticationRequiredError : errorText = tr("QHttp is using a proxy, and the proxy server requires authentication to establish a connection"); break;
        case QHttp::AuthenticationRequiredError : errorText = tr("The web server requires authentication to complete the request"); break;
        case QHttp::UnknownError : errorText = tr("An unknown error occurred"); break;
        case QHttp::NoError : errorText =  ""; break;
        }

        if(!errorText.isEmpty()){
            QMessageBox::critical(this,tr("Error"),errorText);
        }

        return;
    }
}

void DownloadXTA::selectFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this,tr("Select output folder"),tr("Folder path"));
    if(!folder.isEmpty()){
        setFolder(folder + QDir::separator());
    }
}

void DownloadXTA::setFolder(QString path)
{
    if(!path.isEmpty())
        edit->setText(path);
}
