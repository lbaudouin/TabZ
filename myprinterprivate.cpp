#include "myprinterprivate.h"


void MyPrinterPrivate::createDefaultEngines()
{
    QPrinter::OutputFormat realOutputFormat = outputFormat;
#if !defined (QTOPIA_PRINTENGINE)
#if defined (Q_OS_UNIX) && ! defined (Q_WS_MAC)
    if(outputFormat == QPrinter::NativeFormat) {
        realOutputFormat = QPrinter::PostScriptFormat;
    }
#endif
#endif

    switch (realOutputFormat) {
    case QPrinter::NativeFormat: {
#if defined (Q_WS_WIN)
        QWin32PrintEngine *winEngine = new QWin32PrintEngine(printerMode);
        paintEngine = winEngine;
        printEngine = winEngine;
#elif defined (Q_WS_MAC)
        QMacPrintEngine *macEngine = new QMacPrintEngine(printerMode);
        paintEngine = macEngine;
        printEngine = macEngine;
#elif defined (QTOPIA_PRINTENGINE)
        QtopiaPrintEngine *qwsEngine = new QtopiaPrintEngine(printerMode);
        paintEngine = qwsEngine;
        printEngine = qwsEngine;
#elif defined (Q_OS_UNIX)
        Q_ASSERT(false);
#endif
        }
        break;
    case QPrinter::PdfFormat: {
        QPdfEngine *pdfEngine = new QPdfEngine(printerMode);
        paintEngine = pdfEngine;
        printEngine = pdfEngine;
    }
        break;
    case QPrinter::PostScriptFormat: {
        QPSPrintEngine *psEngine = new QPSPrintEngine(printerMode);
        paintEngine = psEngine;
        printEngine = psEngine;
    }
        break;
    }
    use_default_engine = true;
    had_default_engines = true;
}

QList<const QPicture *> MyPrinterPrivate::previewPages() const
{
    if (previewEngine)
        return previewEngine->pages();
    return QList<const QPicture *>();
}

void QPrinterPrivate::setPreviewMode(bool enable)
{
    Q_Q(QPrinter);
    if (enable) {
        if (!previewEngine)
            previewEngine = new QPreviewPaintEngine;
        had_default_engines = use_default_engine;
        use_default_engine = false;
        realPrintEngine = printEngine;
        realPaintEngine = paintEngine;
        q->setEngines(previewEngine, previewEngine);
        previewEngine->setProxyEngines(realPrintEngine, realPaintEngine);
    } else {
        q->setEngines(realPrintEngine, realPaintEngine);
        use_default_engine = had_default_engines;
    }
}

void MyPrinterPrivate::addToManualSetList(QPrintEngine::PrintEnginePropertyKey key)
{
    for (int c = 0; c < manualSetList.size(); ++c) {
        if (manualSetList[c] == key) return;
    }
    manualSetList.append(key);
}
