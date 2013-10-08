#include "epubgenerator.h"

EpubGenerator::EpubGenerator() : outputEpub_("tabz.epub")
{
    tmpFolder_ = QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator();
    epubFolder_ = tmpFolder_ + "epub" + QDir::separator();
    outputFolder_ = tmpFolder_;
}

void EpubGenerator::generate(const QList<XTAinfo> &listXta)
{
    QDir dir(epubFolder_);
    QStringList list = dir.entryList();
    foreach(QString file, list)
        dir.remove(file);

    if(dir.exists(epubFolder_ + "META-INF")){
        dir.cd(epubFolder_ + "META-INF");
        list = dir.entryList();
        foreach(QString file, list)
            dir.remove(file);
    }
    if(dir.exists(epubFolder_ + "tabz")){
        dir.cd(epubFolder_ + "tabz");
        list = dir.entryList();
        foreach(QString file, list)
            dir.remove(file);
    }
    if(dir.exists(epubFolder_ + "fonts")){
        dir.cd(epubFolder_ + "fonts");
        list = dir.entryList();
        foreach(QString file, list)
            dir.remove(file);
    }

    dir.mkpath(epubFolder_ + "META-INF");
    dir.mkpath(epubFolder_ + "tabz");
    dir.mkpath(epubFolder_ + "fonts");

    QFile::remove(tmpFolder_ + "tabz.zip");
    QFile::remove(tmpFolder_ + "tabz.epub");

    QFile::copy("/tmp/lucida.ttf",epubFolder_ + "fonts" + QDir::separator() + "lucida.ttf");

    //Generate pages
    QStringList pages = generatePages(listXta);

    //Generate 'content.opf'
    generateContent(pages);

    //Generate 'titlepage.xhtml'
    generateTitlePage();

    //Generate 'stylesheet.css'
    generateStyleSheet();

    //Generate 'mimetype'
    generateMimetype();

    //Generate 'toc.ncx'
    generateToc(pages);

    //Generate 'META-INF/container.xml'
    generateContainer();

    QProcess process;
    process.setWorkingDirectory(epubFolder_);

    //Compress files in zip
    //QProcess process(zip_exec + arg + filename);
    process.start("zip -r " + tmpFolder_ + "tabz.zip .");
    process.waitForFinished();

    //Rename zip into epub
    QFile::remove(outputFolder_ + outputEpub_);
    QFile::rename(tmpFolder_ + "tabz.zip", outputFolder_ + outputEpub_);
}

void EpubGenerator::generateContent(const QStringList &list)
{
    QFile file(epubFolder_ + "content.opf");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n";
    stream << "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookId\" version=\"2.0\">\n";
    stream << "    <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n";
    //stream << "        <dc:identifier id=\"BookId\" opf:scheme=\"UUID\">urn:uuid:30d28590-b1f3-419c-8f8b-56e3748621f9</dc:identifier>\n";
    stream << "        <dc:creator opf:file-as=\"Giedre\" opf:role=\"aut\">Giedre</dc:creator>\n";
    stream << "        <dc:title>Camionnette</dc:title>\n";
    stream << "    </metadata>\n";
    stream << "    <manifest>\n";
    stream << "        <item href=\"toc.ncx\" id=\"ncx\" media-type=\"application/x-dtbncx+xml\"/>\n";
   stream << "         <item href=\"stylesheet.css\" id=\"css\" media-type=\"text/css\"/>\n";
    //stream << "        <item href=\"cover.jpg\" id=\"cover\" media-type=\"image/jpeg\"/>");
    stream << "        <item href=\"titlepage.xhtml\" id=\"titlepage.xhtml\" media-type=\"application/xhtml+xml\"/>\n";
    foreach(QString page, list){
        stream << "        <item href=\"tabz/"+page+"\" id=\""+page+"\" media-type=\"application/xhtml+xml\"/>\n";
    }
    stream << "    </manifest>\n";
    stream << "    <spine toc=\"ncx\">\n";
    stream << "        <itemref idref=\"titlepage.xhtml\"/>\n";
    foreach(QString page, list){
        stream << "        <itemref idref=\""+page+"\"/>\n";
    }
    stream << "    </spine>\n";
    stream << "    <guide>\n";
    stream << "         <reference href=\"titlepage.xhtml\" type=\"cover\" title=\"Cover\"/>\n";
    stream << "    </guide>\n";
    stream << "</package>";

    file.close();
}

void EpubGenerator::generateMimetype()
{
    QFile file(epubFolder_ + "mimetype");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "application/epub+zip";

    file.close();
}

void EpubGenerator::generateStyleSheet()
{
    QFile file(epubFolder_ + "stylesheet.css");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "@font-face { font-family: \"Lucida Console\"; src: url('fonts/lucida.ttf'); } \n";
    stream << "body {\n";
    stream << "  white-space: pre;\n";
    stream << "  font-family:\"Lucida Console\", monospace;\n";
    stream << "};";

    file.close();
}

void EpubGenerator::generateTitlePage()
{
    /*QPixmap pix(464,747);
    pix.fill(Qt::red);
    pix.save(epubFolder_ + "cover.jpg");*/

    QFile file(epubFolder_ + "titlepage.xhtml");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "<?xml version='1.0' encoding='utf-8'?>\n";
    stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n";
    stream << "    <head>\n";
    stream << "        <meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\n";
    stream << "        <meta name=\"calibre:cover\" content=\"true\"/>\n";
    stream << "        <title>Cover</title>\n";
    stream << "        <style type=\"text/css\" title=\"override_css\">\n";
    stream << "            @page {padding: 0pt; margin:0pt}\n";
    stream << "            body { text-align: center; padding:0pt; margin: 0pt; }\n";
    stream << "        </style>\n";
    stream << "    </head>\n";
    stream << "    <body>\n";
    stream << "        <h1>"+title_+"</h1>\n";
    stream << "        <div>\n";
    //stream << "            <svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" width=\"100%\" height=\"100%\" viewBox=\"0 0 464 747\" preserveAspectRatio=\"xMidYMid meet\">\n";
    //stream << "                <image width=\"464\" height=\"747\" xlink:href=\"cover.jpg\"/>\n";
    //stream << "            </svg>\n";
    stream << "        </div>\n";
    stream << "     Generated by TabZ\n";
    stream << "    </body>\n";
    stream << "</html>";
    file.close();
}

void EpubGenerator::generateToc(const QStringList &list)
{
    QFile file(epubFolder_ + "toc.ncx");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    stream << "<!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\"\n";
    stream << "   \"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\">\n";
    stream << "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">\n";
    stream << "<head>\n";
    stream << "   <meta name=\"dtb:uid\" content=\"urn:uuid:30d28590-b1f3-419c-8f8b-56e3748621f9\" />\n";
    stream << "   <meta name=\"dtb:depth\" content=\"0\" />\n";
    stream << "   <meta name=\"dtb:totalPageCount\" content=\"0\" />\n";
    stream << "   <meta name=\"dtb:maxPageNumber\" content=\"0\" />\n";
    stream << "</head>\n";
    stream << "<docTitle>\n";
    stream << "   <text>Unknown</text>\n";
    stream << "</docTitle>\n";
    stream << "<navMap>\n";
    stream << "<navPoint id=\"navPoint-1\" playOrder=\"1\">\n";
    stream << "  <navLabel>\n";
    stream << "    <text>Start</text>\n";
    stream << "  </navLabel>\n";
    stream << "  <content src=\"titlepage.xhtml\" />\n";
    foreach(QString page, list){
        stream << "  <content src=\"tabz/"+page+"\" />\n";
    }
    stream << "</navPoint>\n";
    stream << "</navMap>\n";
    stream << "</ncx>";
    file.close();
}

void EpubGenerator::generateContainer()
{
    QFile file(epubFolder_ + "META-INF" + QDir::separator() + "container.xml");
    file.open(QFile::WriteOnly);
    QTextStream stream(&file);

    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    stream << "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n";
    stream << "    <rootfiles>\n";
    stream << "        <rootfile full-path=\"content.opf\" media-type=\"application/oebps-package+xml\"/>\n";
    stream << "   </rootfiles>\n";
    stream << "</container>";

    file.close();
}

QStringList EpubGenerator::generatePages(const QList<XTAinfo> &list)
{
    QStringList pagenames;

    for(int i=0;i<list.size();i++){
        QString filename = QString("page%1.html").arg(i,4,10,QChar('0'));

        QFile file(epubFolder_ + "tabz" + QDir::separator() + filename);
        file.open(QFile::WriteOnly);
        QTextStream stream(&file);

        //Qt::escape
        QString text = list.at(i).text;

        while(text.contains(" \n"))
            text = text.replace(" \n","\n");

        while(text.contains("  "))
            text = text.replace("  ","&nbsp;&nbsp;");

        while(text.contains("&nbsp; "))
            text = text.replace("&nbsp; ","&nbsp;&nbsp;");

        text.prepend("<p>");
        text.append("</p>");

        text.replace("\n","</br>\n");

        /*QTextDocument textDoc;
        textDoc.setPlainText(text);
        QString ret = textDoc.toHtml();
        text = ret.section("<body",1).section(">",1).section("</body>",0,0);*/

        stream << "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n";
        stream << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\n";
        stream << "  \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n";
        stream << "\n";
        stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
        stream << "<head>\n";
        stream << "  <title></title>\n";
        stream << "  <meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\n";
        stream << "  <link rel=\"stylesheet\" type=\"text/css\" href=\"../stylesheet.css\" />\n";
        stream << "</head>\n";
        stream << "\n";
        stream << "<body>\n";
        stream << "  <h1>"+list.at(i).title+"</h1>\n";
        stream << "  <h2>"+list.at(i).artist+"</h2>\n";
        stream << "  " + text + "\n";
        stream << "</body>\n";
        stream << "</html>";


        pagenames << filename;
    }

    return pagenames;
}
