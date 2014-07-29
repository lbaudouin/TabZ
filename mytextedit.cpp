#include "mytextedit.h"

MyTextEdit::MyTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

bool MyTextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    return true;
    //return source->hasText() || source->hasImage() || source->hasUrls();
}

void MyTextEdit::insertFromMimeData(const QMimeData *source)
{
    /*qDebug() << "insertFromMimeData";

    qDebug() << "Text:" << source->hasText() << source->text();
    qDebug() << "Image:" << source->hasImage();
    qDebug() << "Html:" << source->hasHtml() << source->html();
    qDebug() << "Urls:" << source->hasUrls() << source->urls();
    qDebug() << "Format:" << source->hasFormat("text/xta") << source->formats();

    qDebug() << source->text() << source->text().isEmpty() << source->text().length();

    qDebug();*/

    if(source->hasUrls()){
        foreach(QUrl url, source->urls()){
            QString path = url.toLocalFile();
            if(path.endsWith(".txt",Qt::CaseInsensitive)){
                QFile file(path);
                if(file.open(QFile::ReadOnly)){
                    QTextStream in(&file);
                    QString text = in.readAll();
                    QTextCursor cursor = this->textCursor();
                    cursor.insertText(text);
                }
                file.close();
                continue;
            }

            QImage img;
            if(img.load(url.toLocalFile())){
                emit this->insertNewImage(img);
            }
        }
        return;
    }

    if(source->hasImage()){
        QImage image = qvariant_cast<QImage>(source->imageData());
        emit this->insertNewImage(image);
        return;
    }

    if(source->hasText() && !source->text().isEmpty() && source->text()!=QString(QChar::ObjectReplacementCharacter)){
        //qDebug() << "insertText";
        QTextCursor cursor = this->textCursor();
        cursor.insertText(source->text());
        return;
    }

    if(source->hasHtml()){
        //qDebug() << "insertHtml";
        QTextCursor cursor = this->textCursor();
        cursor.insertHtml(source->html());
        return;
    }
}

void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Insert){
        this->setOverwriteMode( !this->overwriteMode() );
    }
    QTextEdit::keyPressEvent(event);
}
