#include "myprintpreviewwidget.h"

MyPrintPreviewWidget::MyPrintPreviewWidget(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent,flags)
{
    this->printer_ = new QPrinter;
    this->ownPrinter_ = true;
    this->init();
}

MyPrintPreviewWidget::MyPrintPreviewWidget(QPrinter *printer, QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent,flags)
{
    this->printer_ = printer;
    this->ownPrinter_ = false;
    this->init();
}

MyPrintPreviewWidget::~MyPrintPreviewWidget()
{
    if (this->ownPrinter_)
        delete this->printer_;
}

void MyPrintPreviewWidget::_q_fit(bool doFitting)
{
    if (curPage < 1 || curPage > pages.count())
        return;

    if (!doFitting && !fitting)
        return;

    if (doFitting && fitting) {
        QRect viewRect = graphicsView->viewport()->rect();
        if (zoomMode_ == MyPrintPreviewWidget::FitInView) {
            QList<QGraphicsItem*> containedItems = graphicsView->items(viewRect, Qt::ContainsItemBoundingRect);
            foreach(QGraphicsItem* item, containedItems) {
                PageItem* pg = static_cast<PageItem*>(item);
                if (pg->pageNumber() == curPage)
                    return;
            }
        }

        int newPage = calcCurrentPage();
        if (newPage != curPage)
            curPage = newPage;
    }

    QRectF target = pages.at(curPage-1)->sceneBoundingRect();
    if (viewMode_ == MyPrintPreviewWidget::FacingPagesView) {
        // fit two pages
        if (curPage % 2)
            target.setLeft(target.left() - target.width());
        else
            target.setRight(target.right() + target.width());
    } else if (viewMode_ == MyPrintPreviewWidget::AllPagesView) {
        target = scene->itemsBoundingRect();
    }

    if (zoomMode_ == MyPrintPreviewWidget::FitToWidth) {
        QTransform t;
        qreal scale = graphicsView->viewport()->width() / target.width();
        t.scale(scale, scale);
        graphicsView->setTransform(t);
        if (doFitting && fitting) {
            QRectF viewSceneRect = graphicsView->viewportTransform().mapRect(graphicsView->viewport()->rect());
            viewSceneRect.moveTop(target.top());
            graphicsView->ensureVisible(viewSceneRect); // Nah...
        }
    } else {
        graphicsView->fitInView(target, Qt::KeepAspectRatio);
        if (zoomMode_ == MyPrintPreviewWidget::FitInView) {
            int step = qRound(graphicsView->matrix().mapRect(target).height());
            graphicsView->verticalScrollBar()->setSingleStep(step);
            graphicsView->verticalScrollBar()->setPageStep(step);
        }
    }

    zoomFactor_ = graphicsView->transform().m11() * (float(printer_->logicalDpiY()) / this->logicalDpiY());
    emit this->previewChanged();
}

void MyPrintPreviewWidget::_q_updateCurrentPage()
{
    if (viewMode_ == MyPrintPreviewWidget::AllPagesView)
        return;

    int newPage = calcCurrentPage();
    if (newPage != curPage) {
        curPage = newPage;
        emit this->previewChanged();
    }
}

int MyPrintPreviewWidget::calcCurrentPage()
{
    int maxArea = 0;
    int newPage = curPage;
    QRect viewRect = graphicsView->viewport()->rect();
    QList<QGraphicsItem*> items = graphicsView->items(viewRect);
    for (int i=0; i<items.size(); ++i) {
        PageItem* pg = static_cast<PageItem*>(items.at(i));
        QRect overlap = graphicsView->mapFromScene(pg->sceneBoundingRect()).boundingRect() & viewRect;
        int area = overlap.width() * overlap.height();
        if (area > maxArea) {
            maxArea = area;
            newPage = pg->pageNumber();
        } else if (area == maxArea && pg->pageNumber() < newPage) {
            newPage = pg->pageNumber();
        }
    }
    return newPage;
}

void MyPrintPreviewWidget::init()
{
    //printerPrivate = new MyPrinterPrivate(this->printer_);
    graphicsView = new GraphicsView;
    graphicsView->setInteractive(false);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    QObject::connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(_q_updateCurrentPage()));
    QObject::connect(graphicsView, SIGNAL(resized()), this, SLOT(_q_fit()));

    scene = new QGraphicsScene(graphicsView);
    scene->setBackgroundBrush(Qt::gray);
    graphicsView->setScene(scene);

    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(graphicsView);
}

void MyPrintPreviewWidget::populateScene()
{
    // remove old pages
    for (int i = 0; i < pages.size(); i++)
        scene->removeItem(pages.at(i));
    qDeleteAll(pages);
    pages.clear();

    int numPages = pictures.count();
    QSize paperSize = printer_->paperRect().size();
    QRect pageRect = printer_->pageRect();

    for (int i = 0; i < numPages; i++) {
        PageItem* item = new PageItem(i+1, pictures.at(i), paperSize, pageRect);
        scene->addItem(item);
        pages.append(item);
    }
}

void MyPrintPreviewWidget::layoutPages()
{
    int numPages = pages.count();
    if (numPages < 1)
        return;

    int numPagePlaces = numPages;
    int cols = 1; // singleMode and default
    if (viewMode_ == MyPrintPreviewWidget::AllPagesView) {
        if (printer_->orientation() == QPrinter::Portrait)
            cols = qCeil(qSqrt((float) numPages));
        else
            cols = qFloor(qSqrt((float) numPages));
        cols += cols % 2;  // Nicer with an even number of cols
    }
    else if (viewMode_ == MyPrintPreviewWidget::FacingPagesView) {
        cols = 2;
        numPagePlaces += 1;
    }
    int rows = qCeil(qreal(numPagePlaces) / cols);

    qreal itemWidth = pages.at(0)->boundingRect().width();
    qreal itemHeight = pages.at(0)->boundingRect().height();
    int pageNum = 1;
    for (int i = 0; i < rows && pageNum <= numPages; i++) {
        for (int j = 0; j < cols && pageNum <= numPages; j++) {
            if (!i && !j && viewMode_ == MyPrintPreviewWidget::FacingPagesView) {
                // Front page doesn't have a facing page
                continue;
            } else {
                pages.at(pageNum-1)->setPos(QPointF(j*itemWidth, i*itemHeight));
                pageNum++;
            }
        }
    }
    scene->setSceneRect(scene->itemsBoundingRect());
}

void MyPrintPreviewWidget::generatePreview()
{
    //TODO, generate pictures
    //printer_->d_func()->setPreviewMode(true);
    //emit this->paintRequested(printer_);
    //printer_->d_func()->setPreviewMode(false);
    //pictures = printer_->d_func()->previewPages();
    //(QPreviewPaintEngine*)printer_->paintEngine();

    QPicture *page = new QPicture;
    QPainter *tmp_painter = new QPainter(page);

    QList<const QPicture *> pages;
    //pictures = printer_->d_func()->previewPages();


    populateScene(); // i.e. setPreviewPrintedPictures() e.l.
    layoutPages();
    curPage = qBound(1, curPage, pages.count());
    if (fitting)
        _q_fit();
    emit this->previewChanged();
}

void MyPrintPreviewWidget::setCurrentPage(int pageNumber)
{
    if (pageNumber < 1 || pageNumber > pages.count())
        return;

    int lastPage = curPage;
    curPage = pageNumber;

    if (lastPage != curPage && lastPage > 0 && lastPage <= pages.count()) {
        if (zoomMode_ != MyPrintPreviewWidget::FitInView) {
            QScrollBar *hsc = graphicsView->horizontalScrollBar();
            QScrollBar *vsc = graphicsView->verticalScrollBar();
            QPointF pt = graphicsView->transform().map(pages.at(curPage-1)->pos());
            vsc->setValue(int(pt.y()) - 10);
            hsc->setValue(int(pt.x()) - 10);
        } else {
            graphicsView->centerOn(pages.at(curPage-1));
        }
    }
}

void MyPrintPreviewWidget::zoom(qreal zoom)
{
    zoomFactor_ *= zoom;
    graphicsView->scale(zoom, zoom);
}

void MyPrintPreviewWidget::setZoomFactor(qreal _zoomFactor)
{
    fitting = false;
    zoomMode_ = MyPrintPreviewWidget::CustomZoom;
    zoomFactor_ = _zoomFactor;
    graphicsView->resetTransform();
    int dpi_y = this->logicalDpiY();
    int printer_dpi_y = printer_->logicalDpiY();
    graphicsView->scale(zoomFactor_*(dpi_y/float(printer_dpi_y)),
                        zoomFactor_*(dpi_y/float(printer_dpi_y)));
}

///////////////////////////////////////

/*!
    Returns the current view mode. The default view mode is SinglePageView.
*/
MyPrintPreviewWidget::ViewMode MyPrintPreviewWidget::viewMode() const
{
    return this->viewMode_;
}

/*!
    Sets the view mode to \a mode. The default view mode is
    SinglePageView.
*/
void MyPrintPreviewWidget::setViewMode(ViewMode mode)
{
    this->viewMode_ = mode;
    this->layoutPages();
    if (this->viewMode_ == AllPagesView) {
        this->graphicsView->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        this->fitting = false;
        this->zoomMode_ = MyPrintPreviewWidget::CustomZoom;
        this->zoomFactor_ = this->graphicsView->transform().m11() * (float(this->printer_->logicalDpiY()) / logicalDpiY());
        emit previewChanged();
    } else {
        this->fitting = true;
        this->_q_fit();
    }
}

/*!
    Returns the current orientation of the preview. This value is
    obtained from the QPrinter object associated with the preview.
*/
QPrinter::Orientation MyPrintPreviewWidget::orientation() const
{
    return this->printer_->orientation();
}

/*!
    Sets the current orientation to \a orientation. This value will be
    set on the QPrinter object associated with the preview.
*/
void MyPrintPreviewWidget::setOrientation(QPrinter::Orientation orientation)
{
    this->printer_->setOrientation(orientation);
    this->generatePreview();
}

/*!
    Prints the preview to the printer associated with the preview.
*/
void MyPrintPreviewWidget::print()
{
    // ### make use of the generated pages
    emit paintRequested(this->printer_);
}

/*!
    Zooms the current view in by \a factor. The default value for \a
    factor is 1.1, which means the view will be scaled up by 10%.
*/
void MyPrintPreviewWidget::zoomIn(qreal factor)
{
    this->fitting = false;
    this->zoomMode_ = MyPrintPreviewWidget::CustomZoom;
    this->zoom(factor);
}

/*!
    Zooms the current view out by \a factor. The default value for \a
    factor is 1.1, which means the view will be scaled down by 10%.
*/
void MyPrintPreviewWidget::zoomOut(qreal factor)
{

    this->fitting = false;
    this->zoomMode_ = MyPrintPreviewWidget::CustomZoom;
    this->zoom(1/factor);
}

/*!
    Returns the zoom factor of the view.
*/
qreal MyPrintPreviewWidget::zoomFactor() const
{
    return this->zoomFactor_;
}

/*!
    \obsolete
    Returns the number of pages in the preview.
    \sa pageCount()
*/
int MyPrintPreviewWidget::numPages() const
{
    return this->pages.size();
}

/*!
    \since 4.6
    Returns the number of pages in the preview.
*/
int MyPrintPreviewWidget::pageCount() const
{
    return this->pages.size();
}

/*!
    Returns the currently viewed page in the preview.
*/
int MyPrintPreviewWidget::currentPage() const
{
    return this->curPage;
}

/*!
    This is a convenience function and is the same as calling \c
    {setZoomMode(MyPrintPreviewWidget::FitToWidth)}.
*/
void MyPrintPreviewWidget::fitToWidth()
{
    setZoomMode(FitToWidth);
}

/*!
    This is a convenience function and is the same as calling \c
    {setZoomMode(MyPrintPreviewWidget::FitInView)}.
*/
void MyPrintPreviewWidget::fitInView()
{
    setZoomMode(FitInView);
}

/*!
    Sets the zoom mode to \a zoomMode. The default zoom mode is FitInView.

    \sa zoomMode(), viewMode(), setViewMode()
*/
void MyPrintPreviewWidget::setZoomMode(MyPrintPreviewWidget::ZoomMode zoomMode)
{
    this->zoomMode_ = zoomMode;
    if (this->zoomMode_ == FitInView || this->zoomMode_ == FitToWidth) {
        this->fitting = true;
        this->_q_fit(true);
    } else {
        this->fitting = false;
    }
}

/*!
    Returns the current zoom mode.

    \sa setZoomMode(), viewMode(), setViewMode()
*/
MyPrintPreviewWidget::ZoomMode MyPrintPreviewWidget::zoomMode() const
{
    return this->zoomMode_;
}

/*!
    This is a convenience function and is the same as calling \c
    {setOrientation(QPrinter::Landscape)}.
*/
void MyPrintPreviewWidget::setLandscapeOrientation()
{
    setOrientation(QPrinter::Landscape);
}

/*!
    This is a convenience function and is the same as calling \c
    {setOrientation(QPrinter::Portrait)}.
*/
void MyPrintPreviewWidget::setPortraitOrientation()
{
    setOrientation(QPrinter::Portrait);
}

/*!
    This is a convenience function and is the same as calling \c
    {setViewMode(MyPrintPreviewWidget::SinglePageView)}.
*/
void MyPrintPreviewWidget::setSinglePageViewMode()
{
    setViewMode(SinglePageView);
}

/*!
    This is a convenience function and is the same as calling \c
    {setViewMode(MyPrintPreviewWidget::FacingPagesView)}.
*/
void MyPrintPreviewWidget::setFacingPagesViewMode()
{
    setViewMode(FacingPagesView);
}

/*!
    This is a convenience function and is the same as calling \c
    {setViewMode(MyPrintPreviewWidget::AllPagesView)}.
*/
void MyPrintPreviewWidget::setAllPagesViewMode()
{
    setViewMode(AllPagesView);
}


/*!
    This function updates the preview, which causes the
    paintRequested() signal to be emitted.
*/
void MyPrintPreviewWidget::updatePreview()
{
    this->initialized_ = true;
    this->generatePreview();
    this->graphicsView->updateGeometry();
}

/*! \reimp
*/
void MyPrintPreviewWidget::setVisible(bool visible)
{
    if (visible && !this->initialized_)
        updatePreview();
    QWidget::setVisible(visible);
}
