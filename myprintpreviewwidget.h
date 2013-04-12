#ifndef MYPRINTPREVIEWWIDGET_H
#define MYPRINTPREVIEWWIDGET_H

#include <QWidget>
#include <QPrinter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsView>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QPaintEngine>
#include <QPicture>

//#include "myprinterprivate.h"

#include <qmath.h>

class PageItem : public QGraphicsItem
{
public:
    PageItem(int _pageNum, const QPicture* _pagePicture, QSize _paperSize, QRect _pageRect)
        : pageNum(_pageNum), pagePicture(_pagePicture),
          paperSize(_paperSize), pageRect(_pageRect)
    {
        qreal border = qMax(paperSize.height(), paperSize.width()) / 25;
        brect = QRectF(QPointF(-border, -border),
                       QSizeF(paperSize)+QSizeF(2*border, 2*border));
        setCacheMode(DeviceCoordinateCache);
    }

    inline QRectF boundingRect() const
    { return brect; }

    inline int pageNumber() const
    { return pageNum; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(widget);

    #if 0
        // Draw item bounding rect, for debugging
        painter->save();
        painter->setPen(QPen(Qt::red, 0));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(QRectF(-border()+1.0, -border()+1.0, boundingRect().width()-2, boundingRect().height()-2));
        painter->restore();
    #endif

        QRectF paperRect(0,0, paperSize.width(), paperSize.height());

        // Draw shadow
        painter->setClipRect(option->exposedRect);
        qreal shWidth = paperRect.width()/100;
        QRectF rshadow(paperRect.topRight() + QPointF(0, shWidth),
                       paperRect.bottomRight() + QPointF(shWidth, 0));
        QLinearGradient rgrad(rshadow.topLeft(), rshadow.topRight());
        rgrad.setColorAt(0.0, QColor(0,0,0,255));
        rgrad.setColorAt(1.0, QColor(0,0,0,0));
        painter->fillRect(rshadow, QBrush(rgrad));
        QRectF bshadow(paperRect.bottomLeft() + QPointF(shWidth, 0),
                       paperRect.bottomRight() + QPointF(0, shWidth));
        QLinearGradient bgrad(bshadow.topLeft(), bshadow.bottomLeft());
        bgrad.setColorAt(0.0, QColor(0,0,0,255));
        bgrad.setColorAt(1.0, QColor(0,0,0,0));
        painter->fillRect(bshadow, QBrush(bgrad));
        QRectF cshadow(paperRect.bottomRight(),
                       paperRect.bottomRight() + QPointF(shWidth, shWidth));
        QRadialGradient cgrad(cshadow.topLeft(), shWidth, cshadow.topLeft());
        cgrad.setColorAt(0.0, QColor(0,0,0,255));
        cgrad.setColorAt(1.0, QColor(0,0,0,0));
        painter->fillRect(cshadow, QBrush(cgrad));

        painter->setClipRect(paperRect & option->exposedRect);
        painter->fillRect(paperRect, Qt::white);
        if (!pagePicture)
            return;
        painter->drawPicture(pageRect.topLeft(), *pagePicture);

        // Effect: make anything drawn in the margins look washed out.
        QPainterPath path;
        path.addRect(paperRect);
        path.addRect(pageRect);
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(QColor(255, 255, 255, 180));
        painter->drawPath(path);

    #if 0
        // Draw frame around paper.
        painter->setPen(QPen(Qt::black, 0));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(paperRect);
    #endif

        // todo: drawtext "Page N" below paper
    }


private:
    int pageNum;
    const QPicture* pagePicture;
    QSize paperSize;
    QRect pageRect;
    QRectF brect;
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* parent = 0)
        : QGraphicsView(parent)
    {
#ifdef Q_WS_MAC
        setFrameStyle(QFrame::NoFrame);
#endif
    }
signals:
    void resized();

protected:
    void resizeEvent(QResizeEvent* e)
    {
        QGraphicsView::resizeEvent(e);
        emit resized();
    }

    void showEvent(QShowEvent* e)
    {
        QGraphicsView::showEvent(e);
        emit resized();
    }
};


class MyPrintPreviewWidget : public QWidget
{
    Q_OBJECT
public:  
    explicit MyPrintPreviewWidget(QPrinter *printer, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    explicit MyPrintPreviewWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MyPrintPreviewWidget();

    enum ViewMode {
        SinglePageView,
        FacingPagesView,
        AllPagesView
    };

    enum ZoomMode {
        CustomZoom,
        FitToWidth,
        FitInView
    };


    qreal zoomFactor() const;
    QPrinter::Orientation orientation() const;
    ViewMode viewMode() const;
    ZoomMode zoomMode() const;
    int currentPage() const;
#ifdef QT_DEPRECATED
    QT_DEPRECATED int numPages() const;
#endif
    int pageCount() const;
    void setVisible(bool visible);

protected:
    void init();
    void populateScene();
    void layoutPages();
    void generatePreview();
    void zoom(qreal zoom);
    int calcCurrentPage();

private:
    GraphicsView *graphicsView;
    QGraphicsScene *scene;

    //MyPrinterPrivate *printerPrivate;

    int curPage;
    QList<const QPicture *> pictures;
    QList<QGraphicsItem *> pages;

    MyPrintPreviewWidget::ViewMode viewMode_;
    MyPrintPreviewWidget::ZoomMode zoomMode_;
    qreal zoomFactor_;
    bool ownPrinter_;
    QPrinter* printer_;
    bool initialized_;
    bool fitting;

public slots:
    void print();

    void zoomIn(qreal zoom = 1.1);
    void zoomOut(qreal zoom = 1.1);
    void setZoomFactor(qreal zoomFactor);
    void setOrientation(QPrinter::Orientation orientation);
    void setViewMode(ViewMode viewMode);
    void setZoomMode(ZoomMode zoomMode);
    void setCurrentPage(int pageNumber);

    void fitToWidth();
    void fitInView();
    void setLandscapeOrientation();
    void setPortraitOrientation();
    void setSinglePageViewMode();
    void setFacingPagesViewMode();
    void setAllPagesViewMode();

    void updatePreview();

signals:
    void paintRequested(QPrinter *printer);
    void previewChanged();

private slots:
    void _q_fit(bool doFitting = false);
    void _q_updateCurrentPage();
};

#endif // MYPRINTPREVIEWWIDGET_H
