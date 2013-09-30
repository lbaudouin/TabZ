#include "epubgenerator.h"

EpubGenerator::EpubGenerator()
{
    //Generate 'conten.opf'

    //Generate 'titlepage.xhtml'

    //Generate 'stylesheet.css'

    //Generate 'mimetype'

    //Generate 'toc.ncx'


    /*
    //Mkdir 'META-INF'

    //Generate 'container.xml'
    */

    //Compress files in zip

    //Rename zip into epub
}

void EpubGenerator::generateContent()
{
    QFile file("/tmp/epub/content.opf");
    file.open(QFile::WriteOnly);

    file.write("<?xml version='1.0' encoding='utf-8'?>");
    file.write("<package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\" unique-identifier=\"uuid_id\">");
    file.write("  <metadata xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:calibre=\"http://calibre.kovidgoyal.net/2009/metadata\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\">");
    file.write("    <dc:publisher>TabS</dc:publisher>");
    file.write("    <meta name=\"calibre:title_sort\" content=\"Radix\"/>");
    file.write("    <dc:description></dc:description>");
    file.write("    <dc:language>fr</dc:language>");
    file.write("    <dc:creator opf:file-as=\"Attanasio,A.A.\" opf:role=\"aut\">Attanasio,A.A.</dc:creator>");
    file.write("    <meta name=\"calibre:timestamp\" content=\"2011-04-19T17:56:03+00:00\"/>");
    file.write("    <dc:title>Radix</dc:title>");
    file.write("    <meta name=\"cover\" content=\"cover\"/>");
    file.write("    <dc:date>1981-01-04T23:00:00+00:00</dc:date>");
    file.write("    <dc:contributor opf:role=\"bkp\">calibre (0.7.50) [http://calibre-ebook.com]</dc:contributor>");
    file.write("    <dc:identifier opf:scheme=\"ISBN\">2843622786</dc:identifier>");
    file.write("    <dc:identifier id=\"uuid_id\" opf:scheme=\"uuid\">71121a26-35ef-42b5-bc24-e65661a4f921</dc:identifier>");
    file.write("    <dc:subject>SF</dc:subject>");
    file.write("  </metadata>");
    file.write("  <manifest>");
    file.write("    <item href=\"page000.html\" id=\"html31\" media-type=\"application/xhtml+xml\"/>");
    file.write("    <item href=\"page001.html\" id=\"html30\" media-type=\"application/xhtml+xml\"/>");
    file.write("    <item href=\"page029.html\" id=\"html2\" media-type=\"application/xhtml+xml\"/>");
    file.write("    <item href=\"page030.html\" id=\"html1\" media-type=\"application/xhtml+xml\"/>");
    file.write("    <item href=\"cover.jpeg\" id=\"cover\" media-type=\"image/jpeg\"/>");
    file.write("    <item href=\"image001.jpg\" id=\"added\" media-type=\"image/jpeg\"/>");
    file.write("    <item href=\"image002.gif\" id=\"added1\" media-type=\"image/gif\"/>");
    file.write("    <item href=\"stylesheet.css\" id=\"css\" media-type=\"text/css\"/>");
    file.write("    <item href=\"titlepage.xhtml\" id=\"titlepage\" media-type=\"application/xhtml+xml\"/>");
    file.write("    <item href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" id=\"ncx\"/>");
    file.write("  </manifest>");
    file.write("  <spine toc=\"ncx\">");
    file.write("    <itemref idref=\"titlepage\"/>");
    file.write("    <itemref idref=\"html31\"/>");
    file.write("    <itemref idref=\"html30\"/>");
    file.write("    <itemref idref=\"html29\"/>");
    file.write("  </spine>");
    file.write("  <guide>");
    file.write("    <reference href=\"titlepage.xhtml\" type=\"cover\" title=\"Cover\"/>");
    file.write("  </guide>");
    file.write("</package>");

    file.close();
}

void EpubGenerator::generateMimetype()
{
    QFile file("/tmp/epub/mimetype");
    file.open(QFile::WriteOnly);
    file.write("application/epub+zip");
    file.close();
}

void EpubGenerator::generateStyleSheet()
{
    QFile file("/tmp/epub/stylesheet.css");
    file.open(QFile::WriteOnly);
    file.write("@namespace h \"http://www.w3.org/1999/xhtml\""
    ".MsoNormal {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: center;"
    "    text-indent: 0"
    "}"
    ".MsoNormal1 {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: right;"
    "    text-indent: 0"
    "}"
    ".MsoNormal2 {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: right;"
    "    text-indent: 21.3pt"
    "}"
    ".MsoNormal3 {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: justify;"
    "    text-indent: 21.3pt"
    "}"
    ".MsoNormal4 {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: justify;"
    "    text-indent: 0"
    "}"
    ".MsoNormal5 {"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 0;"
    "    text-align: justify;"
    "    text-indent: 42.55pt"
    "}"
    ".WordSection {"
    "    display: block;"
    "    page: WordSection1"
    "}"
    ".ZFin {"
    "    background: #B3B3B3;"
    "    border: none;"
    "    color: black;"
    "    display: block;"
    "    font-size: 1.33333em;"
    "    font-style: italic;"
    "    letter-spacing: 3pt;"
    "    line-height: 1.2;"
    "    margin-bottom: 0.0001pt;"
    "    margin-left: 0;"
    "    margin-right: 0;"
    "    margin-top: 1cm;"
    "    padding-bottom: 0;"
    "    padding-left: 0;"
    "    padding-right: 0;"
    "    padding-top: 0;"
    "    text-align: center"
    "}");
    file.close();
}

void EpubGenerator::generateTitlePage()
{
    QFile file("/tmp/epub/titlepage.xhtml");
    file.open(QFile::WriteOnly);
    file.write("<?xml version='1.0' encoding='utf-8'?>");
    file.write("<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">");
    file.write("    <head>");
    file.write("        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>");
    file.write("        <meta name=\"calibre:cover\" content=\"true\"/>");
    file.write("        <title>Cover</title>");
    file.write("        <style type=\"text/css\" title=\"override_css\">");
    file.write("            @page {padding: 0pt; margin:0pt}");
    file.write("            body { text-align: center; padding:0pt; margin: 0pt; }");
    file.write("        </style>");
    file.write("    </head>");
    file.write("    <body>");
    file.write("        <div>");
    file.write("            <svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\" width=\"100%\" height=\"100%\" viewBox=\"0 0 464 747\" preserveAspectRatio=\"xMidYMid meet\">");
    file.write("                <image width=\"464\" height=\"747\" xlink:href=\"cover.jpeg\"/>");
    file.write("            </svg>");
    file.write("        </div>");
    file.write("    </body>");
    file.write("</html>");
    file.close();
}

void EpubGenerator::generateToc()
{
    QFile file("/tmp/epub/toc.ncx");
    file.open(QFile::WriteOnly);
    file.write("<?xml version='1.0' encoding='utf-8'?>");
    file.write("<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"fr\">");
    file.write("  <head>");
    file.write("    <meta content=\"71121a26-35ef-42b5-bc24-e65661a4f921\" name=\"dtb:uid\"/>");
    file.write("    <meta content=\"3\" name=\"dtb:depth\"/>");
    file.write("    <meta content=\"calibre (0.7.50)\" name=\"dtb:generator\"/>");
    file.write("    <meta content=\"0\" name=\"dtb:totalPageCount\"/>");
    file.write("    <meta content=\"0\" name=\"dtb:maxPageNumber\"/>");
    file.write("  </head>");
    file.write("  <docTitle>");
    file.write("    <text>Radix</text>");
    file.write("  </docTitle>");
    file.write("  <navMap>");
    file.write("    <navPoint id=\"3692c452-b771-49a0-b40a-21c27b23273e\" playOrder=\"1\">");
    file.write("      <navLabel>");
    file.write("        <text>LES DISTORS</text>");
    file.write("      </navLabel>");
    file.write("      <content src=\"Attanasio,A.A.-Radix(1981).French.ebook.AlexandriZ_split_004.html\"/>");
    file.write("      <navPoint id=\"eb1fcad0-e6d9-496b-8d8b-f1920c66866c\" playOrder=\"2\">");
    file.write("    </navPoint>");
    file.write("  </navMap>");
    file.write("</ncx>");
    file.close();
}
