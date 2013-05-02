#include "qprintpreview.h"

QPrintPreview::QPrintPreview(QWidget *parent, Qt::WindowFlags flags) : printDialog(0), ownPrinter(false), initialized(false), QWidget(parent,flags)
{
    init();
}

QPrintPreview::QPrintPreview(QPrinter *printer, QWidget *parent, Qt::WindowFlags flags) : printDialog(0), ownPrinter(false), initialized(false), QWidget(parent,flags)
{
    init(printer);
}

QPrintPreview::~QPrintPreview()
{
    if (this->ownPrinter)
        delete this->printer;
    delete this->printDialog;
}

void QPrintPreview::init(QPrinter *_printer)
{
    if (_printer) {
        preview = new QPrintPreviewWidget(_printer, this);
        printer = _printer;
    } else {
        ownPrinter = true;
        printer = new QPrinter;
        preview = new QPrintPreviewWidget(printer, this);
    }
    QObject::connect(preview, SIGNAL(paintRequested(QPrinter*)), this, SIGNAL(paintRequested(QPrinter*)));
    QObject::connect(preview, SIGNAL(previewChanged()), this, SLOT(_q_previewChanged()));
    setupActions();

    pageNumEdit = new LineEdit;
    pageNumEdit->setAlignment(Qt::AlignRight);
    pageNumEdit->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    pageNumLabel = new QLabel;
    QObject::connect(pageNumEdit, SIGNAL(editingFinished()), this, SLOT(_q_pageNumEdited()));

    zoomFactor = new QComboBox;
    zoomFactor->setEditable(true);
    zoomFactor->setMinimumContentsLength(7);
    zoomFactor->setInsertPolicy(QComboBox::NoInsert);
    LineEdit *zoomEditor = new LineEdit;
    zoomEditor->setValidator(new ZoomFactorValidator(1, 1000, 1, zoomEditor));
    zoomFactor->setLineEdit(zoomEditor);
    static const short factorsX2[] = { 25, 50, 100, 200, 250, 300, 400, 800, 1600 };
    for (int i = 0; i < int(sizeof(factorsX2) / sizeof(factorsX2[0])); ++i)
        zoomFactor->addItem(tr("%1%").arg(factorsX2[i] / 2.0));
    QObject::connect(zoomFactor->lineEdit(), SIGNAL(editingFinished()),
                     this, SLOT(_q_zoomFactorChanged()));
    QObject::connect(zoomFactor, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(_q_zoomFactorChanged()));

    QPrintPreviewMainWindow *mw = new QPrintPreviewMainWindow(this);
    QToolBar *toolbar = new QToolBar(mw);
    toolbar->setFloatable(false);
    toolbar->addAction(fitWidthAction);
    toolbar->addAction(fitPageAction);
    toolbar->addSeparator();
    toolbar->addWidget(zoomFactor);
    toolbar->addAction(zoomOutAction);
    toolbar->addAction(zoomInAction);
    toolbar->addSeparator();
    toolbar->addAction(portraitAction);
    toolbar->addAction(landscapeAction);
    toolbar->addSeparator();
    toolbar->addAction(firstPageAction);
    toolbar->addAction(prevPageAction);

    // this is to ensure the label text and the editor text are
    // aligned in all styles - the extra QVBoxLayout is a workaround
    // for bug in QFormLayout
    QWidget *pageEdit = new QWidget(toolbar);
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(0, 0, 0, 0);
#ifdef Q_WS_MAC
    // We query the widgets about their size and then we fix the size.
    // This should do the trick for the laying out part...
    QSize pageNumEditSize, pageNumLabelSize;
    pageNumEditSize = pageNumEdit->minimumSizeHint();
    pageNumLabelSize = pageNumLabel->minimumSizeHint();
    pageNumEdit->resize(pageNumEditSize);
    pageNumLabel->resize(pageNumLabelSize);
#endif
    QFormLayout *formLayout = new QFormLayout;
#ifdef Q_WS_MAC
    // We have to change the growth policy in Mac.
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
#endif
    formLayout->setWidget(0, QFormLayout::LabelRole, pageNumEdit);
    formLayout->setWidget(0, QFormLayout::FieldRole, pageNumLabel);
    vboxLayout->addLayout(formLayout);
    vboxLayout->setAlignment(Qt::AlignVCenter);
    pageEdit->setLayout(vboxLayout);
    toolbar->addWidget(pageEdit);

    toolbar->addAction(nextPageAction);
    toolbar->addAction(lastPageAction);
    toolbar->addSeparator();
    toolbar->addAction(singleModeAction);
    toolbar->addAction(facingModeAction);
    toolbar->addAction(overviewModeAction);
    toolbar->addSeparator();
    toolbar->addAction(pageSetupAction);
    toolbar->addAction(printAction);
#if defined(Q_WS_MAC) && !defined(QT_MAC_USE_COCOA)
    toolbar->addAction(closeAction);
#endif

    // Cannot use the actions' triggered signal here, since it doesn't autorepeat
    QToolButton *zoomInButton = static_cast<QToolButton *>(toolbar->widgetForAction(zoomInAction));
    QToolButton *zoomOutButton = static_cast<QToolButton *>(toolbar->widgetForAction(zoomOutAction));
    zoomInButton->setAutoRepeat(true);
    zoomInButton->setAutoRepeatInterval(200);
    zoomInButton->setAutoRepeatDelay(200);
    zoomOutButton->setAutoRepeat(true);
    zoomOutButton->setAutoRepeatInterval(200);
    zoomOutButton->setAutoRepeatDelay(200);
    QObject::connect(zoomInButton, SIGNAL(clicked()), this, SLOT(_q_zoomIn()));
    QObject::connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(_q_zoomOut()));

    mw->addToolBar(toolbar);
    mw->setCentralWidget(preview);
    // QMainWindows are always created as top levels, force it to be a
    // plain widget
    mw->setParent(this, Qt::Widget);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(mw);
    topLayout->setMargin(0);
    this->setLayout(topLayout);

    QString caption = QCoreApplication::translate("QPrintPreview", "Print Preview");
    if (!printer->docName().isEmpty())
        caption += QString::fromLatin1(": ") + printer->docName();
    this->setWindowTitle(caption);

    if (!printer->isValid()
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
        || printer->outputFormat() != QPrinter::NativeFormat
#endif
        )
        pageSetupAction->setEnabled(false);
    preview->setFocus();
}

static inline void qt_setupActionIcon(QAction *action, const QLatin1String &name)
{
    QLatin1String imagePrefix(":/trolltech/dialogs/qprintpreviewdialog/images/");
    QIcon icon;
    icon.addFile(imagePrefix + name + QLatin1String("-24.png"), QSize(24, 24));
    icon.addFile(imagePrefix + name + QLatin1String("-32.png"), QSize(32, 32));
    action->setIcon(icon);
}

void QPrintPreview::setupActions()
{
    // Navigation
    navGroup = new QActionGroup(this);
    navGroup->setExclusive(false);
    nextPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreview", "Next page"));
    prevPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreview", "Previous page"));
    firstPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreview", "First page"));
    lastPageAction = navGroup->addAction(QCoreApplication::translate("QPrintPreview", "Last page"));
    qt_setupActionIcon(nextPageAction, QLatin1String("go-next"));
    qt_setupActionIcon(prevPageAction, QLatin1String("go-previous"));
    qt_setupActionIcon(firstPageAction, QLatin1String("go-first"));
    qt_setupActionIcon(lastPageAction, QLatin1String("go-last"));
    QObject::connect(navGroup, SIGNAL(triggered(QAction*)), this, SLOT(_q_navigate(QAction*)));


    fitGroup = new QActionGroup(this);
    fitWidthAction = fitGroup->addAction(QCoreApplication::translate("QPrintPreview", "Fit width"));
    fitPageAction = fitGroup->addAction(QCoreApplication::translate("QPrintPreview", "Fit page"));
    fitWidthAction->setObjectName(QLatin1String("fitWidthAction"));
    fitPageAction->setObjectName(QLatin1String("fitPageAction"));
    fitWidthAction->setCheckable(true);
    fitPageAction->setCheckable(true);
    qt_setupActionIcon(fitWidthAction, QLatin1String("fit-width"));
    qt_setupActionIcon(fitPageAction, QLatin1String("fit-page"));
    QObject::connect(fitGroup, SIGNAL(triggered(QAction*)), this, SLOT(_q_fit(QAction*)));

    // Zoom
    zoomGroup = new QActionGroup(this);
    zoomInAction = zoomGroup->addAction(QCoreApplication::translate("QPrintPreview", "Zoom in"));
    zoomOutAction = zoomGroup->addAction(QCoreApplication::translate("QPrintPreview", "Zoom out"));
    qt_setupActionIcon(zoomInAction, QLatin1String("zoom-in"));
    qt_setupActionIcon(zoomOutAction, QLatin1String("zoom-out"));

    // Portrait/Landscape
    orientationGroup = new QActionGroup(this);
    portraitAction = orientationGroup->addAction(QCoreApplication::translate("QPrintPreview", "Portrait"));
    landscapeAction = orientationGroup->addAction(QCoreApplication::translate("QPrintPreview", "Landscape"));
    portraitAction->setCheckable(true);
    landscapeAction->setCheckable(true);
    qt_setupActionIcon(portraitAction, QLatin1String("layout-portrait"));
    qt_setupActionIcon(landscapeAction, QLatin1String("layout-landscape"));
    QObject::connect(portraitAction, SIGNAL(triggered(bool)), preview, SLOT(setPortraitOrientation()));
    QObject::connect(landscapeAction, SIGNAL(triggered(bool)), preview, SLOT(setLandscapeOrientation()));

    // Display mode
    modeGroup = new QActionGroup(this);
    singleModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreview", "Show single page"));
    facingModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreview", "Show facing pages"));
    overviewModeAction = modeGroup->addAction(QCoreApplication::translate("QPrintPreview", "Show overview of all pages"));
    qt_setupActionIcon(singleModeAction, QLatin1String("view-page-one"));
    qt_setupActionIcon(facingModeAction, QLatin1String("view-page-sided"));
    qt_setupActionIcon(overviewModeAction, QLatin1String("view-page-multi"));
    singleModeAction->setObjectName(QLatin1String("singleModeAction"));
    facingModeAction->setObjectName(QLatin1String("facingModeAction"));
    overviewModeAction->setObjectName(QLatin1String("overviewModeAction"));

    singleModeAction->setCheckable(true);
    facingModeAction->setCheckable(true);
    overviewModeAction->setCheckable(true);
    QObject::connect(modeGroup, SIGNAL(triggered(QAction*)), this, SLOT(_q_setMode(QAction*)));

    // Print
    printerGroup = new QActionGroup(this);
    printAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreview", "Print"));
    pageSetupAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreview", "Page setup"));
    qt_setupActionIcon(printAction, QLatin1String("print"));
    qt_setupActionIcon(pageSetupAction, QLatin1String("page-setup"));
    QObject::connect(printAction, SIGNAL(triggered(bool)), this, SLOT(_q_print()));
    QObject::connect(pageSetupAction, SIGNAL(triggered(bool)), this, SLOT(_q_pageSetup()));
#if defined(Q_WS_MAC) && !defined(QT_MAC_USE_COCOA)
    closeAction = printerGroup->addAction(QCoreApplication::translate("QPrintPreview", "Close"));
    QObject::connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(reject()));
#endif

    // Initial state:
    fitPageAction->setChecked(true);
    singleModeAction->setChecked(true);
    if (preview->orientation() == QPrinter::Portrait)
        portraitAction->setChecked(true);
    else
        landscapeAction->setChecked(true);
}


bool QPrintPreview::isFitting()
{
    return (fitGroup->isExclusive()
            && (fitWidthAction->isChecked() || fitPageAction->isChecked()));
}


void QPrintPreview::setFitting(bool on)
{
    if (isFitting() == on)
        return;
    fitGroup->setExclusive(on);
    if (on) {
        QAction* action = fitWidthAction->isChecked() ? fitWidthAction : fitPageAction;
        action->setChecked(true);
        if (fitGroup->checkedAction() != action) {
            // work around exclusitivity problem
            fitGroup->removeAction(action);
            fitGroup->addAction(action);
        }
    } else {
        fitWidthAction->setChecked(false);
        fitPageAction->setChecked(false);
    }
}

void QPrintPreview::updateNavActions()
{
    int curPage = preview->currentPage();
    int numPages = preview->pageCount();
    nextPageAction->setEnabled(curPage < numPages);
    prevPageAction->setEnabled(curPage > 1);
    firstPageAction->setEnabled(curPage > 1);
    lastPageAction->setEnabled(curPage < numPages);
    pageNumEdit->setText(QString::number(curPage));
}

void QPrintPreview::updatePageNumLabel()
{
    int numPages = preview->pageCount();
    int maxChars = QString::number(numPages).length();
    pageNumLabel->setText(QString::fromLatin1("/ %1").arg(numPages));
    int cyphersWidth = this->fontMetrics().width(QString().fill(QLatin1Char('8'), maxChars));
    int maxWidth = pageNumEdit->minimumSizeHint().width() + cyphersWidth;
    pageNumEdit->setMinimumWidth(maxWidth);
    pageNumEdit->setMaximumWidth(maxWidth);
    pageNumEdit->setValidator(new QIntValidator(1, numPages, pageNumEdit));
    // any old one will be deleted later along with its parent pageNumEdit
}

void QPrintPreview::updateZoomFactor()
{
    zoomFactor->lineEdit()->setText(QString().sprintf("%.1f%%", preview->zoomFactor()*100));
}

void QPrintPreview::_q_fit(QAction* action)
{
    setFitting(true);
    if (action == fitPageAction)
        preview->fitInView();
    else
        preview->fitToWidth();
}

void QPrintPreview::_q_zoomIn()
{
    setFitting(false);
    preview->zoomIn();
    updateZoomFactor();
}

void QPrintPreview::_q_zoomOut()
{
    setFitting(false);
    preview->zoomOut();
    updateZoomFactor();
}

void QPrintPreview::_q_pageNumEdited()
{
    bool ok = false;
    int res = pageNumEdit->text().toInt(&ok);
    if (ok)
        preview->setCurrentPage(res);
}

void QPrintPreview::_q_navigate(QAction* action)
{
    int curPage = preview->currentPage();
    if (action == prevPageAction)
        preview->setCurrentPage(curPage - 1);
    else if (action == nextPageAction)
        preview->setCurrentPage(curPage + 1);
    else if (action == firstPageAction)
        preview->setCurrentPage(1);
    else if (action == lastPageAction)
        preview->setCurrentPage(preview->pageCount());
    updateNavActions();
}

void QPrintPreview::_q_setMode(QAction* action)
{
    if (action == overviewModeAction) {
        preview->setViewMode(QPrintPreviewWidget::AllPagesView);
        setFitting(false);
        fitGroup->setEnabled(false);
        navGroup->setEnabled(false);
        pageNumEdit->setEnabled(false);
        pageNumLabel->setEnabled(false);
    } else if (action == facingModeAction) {
        preview->setViewMode(QPrintPreviewWidget::FacingPagesView);
    } else {
        preview->setViewMode(QPrintPreviewWidget::SinglePageView);
    }
    if (action == facingModeAction || action == singleModeAction) {
        fitGroup->setEnabled(true);
        navGroup->setEnabled(true);
        pageNumEdit->setEnabled(true);
        pageNumLabel->setEnabled(true);
        setFitting(true);
    }
}

void QPrintPreview::_q_print()
{

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    if (printer->outputFormat() != QPrinter::NativeFormat) {
        QString title;
        QString suffix;
        if (printer->outputFormat() == QPrinter::PdfFormat) {
            title = QCoreApplication::translate("QPrintPreview", "Export to PDF");
            suffix = QLatin1String(".pdf");
        } else {
            title = QCoreApplication::translate("QPrintPreview", "Export to PostScript");
            suffix = QLatin1String(".ps");
        }
        QString fileName = QFileDialog::getSaveFileName(this, title, printer->outputFileName(),
                                                        QLatin1Char('*') + suffix);
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty())
                fileName.append(suffix);
            printer->setOutputFileName(fileName);
        }
        if (!printer->outputFileName().isEmpty())
            preview->print();
        return;
    }
#endif

    if (!printDialog)
        printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        preview->print();
    }
}

void QPrintPreview::_q_pageSetup()
{
    QPageSetupDialog pageSetup(printer, this);
    if (pageSetup.exec() == QDialog::Accepted) {
        // update possible orientation changes
        if (preview->orientation() == QPrinter::Portrait) {
            portraitAction->setChecked(true);
            preview->setPortraitOrientation();
        }else {
            landscapeAction->setChecked(true);
            preview->setLandscapeOrientation();
        }
    }
}

void QPrintPreview::_q_previewChanged()
{
    updateNavActions();
    updatePageNumLabel();
    updateZoomFactor();
}

void QPrintPreview::_q_zoomFactorChanged()
{
    QString text = zoomFactor->lineEdit()->text();
    bool ok;
    qreal factor = text.remove(QLatin1Char('%')).toFloat(&ok);
    factor = qMax(qreal(1.0), qMin(qreal(1000.0), factor));
    if (ok) {
        preview->setZoomFactor(factor/100.0);
        zoomFactor->setEditText(QString::fromLatin1("%1%").arg(factor));
        setFitting(false);
    }
}

void QPrintPreview::updatePreview()
{
    preview->updatePreview();
}

///////////////////////////////////////////////////////////////////////////

/*void QPrintPreview::setVisible(bool visible)
{
    // this will make the dialog get a decent default size
    if (visible && !this->initialized) {
        this->preview->updatePreview();
        this->initialized = true;
    }
    //QDialog::setVisible(visible);
    this->preview->updatePreview();
}


void QPrintPreview::done(int result)
{
    //QDialog::done(result);
    if (this->receiverToDisconnectOnClose) {
        disconnect(this, SIGNAL(finished(int)),
                   this->receiverToDisconnectOnClose, this->memberToDisconnectOnClose);
        this->receiverToDisconnectOnClose = 0;
    }
    this->memberToDisconnectOnClose.clear();
}*/

QPrinter *QPrintPreview::getPrinter()
{
    return this->printer;
}

QPrintPreviewWidget *QPrintPreview::getPreview()
{
    return this->preview;
}

void QPrintPreview::pressPrint(QString filename)
{
    _q_print();
}

void QPrintPreview::exportPDF(QString filename)
{
    //Save
    QPrinter::OutputFormat previousFormat = printer->outputFormat();
    QString previousOutputFilename = printer->outputFileName();
    //Set new
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(filename);
    //Print
    preview->print();
    //Restore
    printer->setOutputFormat(previousFormat);
    printer->setOutputFileName(previousOutputFilename);
}

void QPrintPreview::setCurrentPage(int page)
{
    preview->setCurrentPage(page);
}

void QPrintPreview::setViewMode ( QPrintPreviewWidget::ViewMode viewMode )
{
    switch(viewMode){
    case QPrintPreviewWidget::SinglePageView: singleModeAction->setChecked(true); break;
    case QPrintPreviewWidget::FacingPagesView: facingModeAction->setChecked(true); break;
    case QPrintPreviewWidget::AllPagesView: overviewModeAction->setChecked(true); break;
    }
    preview->setViewMode(viewMode);
}

void QPrintPreview::setZoomMode(QPrintPreviewWidget::ZoomMode zoomMode)
{
    switch(zoomMode){
    case QPrintPreviewWidget::FitToWidth: fitWidthAction->setChecked(true); break;
    case QPrintPreviewWidget::FitInView: fitPageAction->setChecked(true); break;
    }
    preview->setZoomMode(zoomMode);

}
