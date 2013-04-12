#ifndef MYPRINTERPRIVATE_H
#define MYPRINTERPRIVATE_H

#include <QPrinter>
#include <QPrintEngine>
#include <QAbstractPrintDialog>

class MyPrinterPrivate
{
public:
    MyPrinterPrivate(QPrinter *printer)
        : printEngine(0)
        , paintEngine(0)
        , q_ptr(printer)
        , options(QAbstractPrintDialog::PrintToFile | QAbstractPrintDialog::PrintPageRange |
                QAbstractPrintDialog::PrintCollateCopies | QAbstractPrintDialog::PrintShowPageSize)
        , printRange(QAbstractPrintDialog::AllPages)
        , minPage(1)
        , maxPage(INT_MAX)
        , fromPage(0)
        , toPage(0)
        , use_default_engine(true)
        , validPrinter(false)
        , hasCustomPageMargins(false)
        , hasUserSetPageSize(false)
    {
    }

    ~MyPrinterPrivate() {

    }

    void createDefaultEngines();
    QList<const QPicture *> previewPages() const;
    void setPreviewMode(bool);

    void addToManualSetList(QPrintEngine::PrintEnginePropertyKey key);

    QPrinter::PrinterMode printerMode;
    QPrinter::OutputFormat outputFormat;
    QPrintEngine *printEngine;
    QPaintEngine *paintEngine;

    QPrintEngine *realPrintEngine;
    QPaintEngine *realPaintEngine;
    QPreviewPaintEngine *previewEngine;

    QPrinter *q_ptr;

    QAbstractPrintDialog::PrintDialogOptions options;
    QAbstractPrintDialog::PrintRange printRange;
    int minPage, maxPage, fromPage, toPage;

    uint use_default_engine : 1;
    uint had_default_engines : 1;

    uint validPrinter : 1;
    uint hasCustomPageMargins : 1;
    uint hasUserSetPageSize : 1;

    // Used to remember which properties have been manually set by the user.
    QList<QPrintEngine::PrintEnginePropertyKey> manualSetList;
};

#endif // MYPRINTERPRIVATE_H
