#include "downloadxta.h"

DownloadXTA::DownloadXTA(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(tr("XTA downloader"));
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

    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

    QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    diskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
#else
    diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
#endif
    manager->setCache(diskCache);

    QNetworkRequest request(QUrl(QString("http://lbaudouin.chez.com/xta.xml")));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "utf-8" );
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("text/plain; charset=UTF-8").toAscii());
#else
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("text/plain; charset=UTF-8"));
#endif
    //request.setRawHeader("Accept-Charset", "utf-8;");
    listXtaReply = manager->get(request);
}

void DownloadXTA::parse(QString content)
{
    QDomDocument *dom = new QDomDocument("docXML");

    if(!dom->setContent(content)){
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
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                QNetworkRequest request(QUrl("http://lbaudouin.chez.com/"+c->data().toString()));
#else
                QNetworkRequest request(QUrl("http://lbaudouin.chez.com/"+c->data().toString()));
#endif
                QNetworkReply *reply = manager->get(request);
                reply->setProperty("userdata", QVariant(fi.absoluteFilePath()));
            }
        }
    }
    this->accept();
}

void DownloadXTA::replyFinished(QNetworkReply *reply)
{
    if(reply == listXtaReply){
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QString content = codec->toUnicode(reply->readAll());

        statusLabel->setText(tr("Parsing..."));
        parse(content);
        statusWidget->hide();
        imageLabel->movie()->stop();
        return;
    }

    if(reply->error()==QNetworkReply::NoError){
        QFile file(reply->property("userdata").toString());
        file.open(QFile::WriteOnly);
        file.write(reply->readAll());
        file.close();
    }else{
        qDebug() << reply->errorString();
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
