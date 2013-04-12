#ifndef QPRINTPREVIEW_H
#define QPRINTPREVIEW_H

#include <QWidget>
#include <QMenu>
#include <QValidator>
#include <QLineEdit>
#include <QMainWindow>
#include <QAction>
#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPageSetupDialog>
#include <QPrinter>
#include <QStyle>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>
#include <QFormLayout>
#include <QCoreApplication>
#include <QPrintDialog>
#include <QPointer>

//#include "myprintpreviewwidget.h"
#include <QPrintPreviewWidget>

#include <math.h>

class QPrintPreviewMainWindow : public QMainWindow
{
public:
    QPrintPreviewMainWindow(QWidget *parent) : QMainWindow(parent) {}
    QMenu *createPopupMenu() { return 0; }
};

class ZoomFactorValidator : public QDoubleValidator
{
public:
    ZoomFactorValidator(QObject* parent)
        : QDoubleValidator(parent) {}
    ZoomFactorValidator(qreal bottom, qreal top, int decimals, QObject *parent)
        : QDoubleValidator(bottom, top, decimals, parent) {}

    State validate(QString &input, int &pos) const
    {
        bool replacePercent = false;
        if (input.endsWith(QLatin1Char('%'))) {
            input = input.left(input.length() - 1);
            replacePercent = true;
        }
        State state = QDoubleValidator::validate(input, pos);
        if (replacePercent)
            input += QLatin1Char('%');
        const int num_size = 4;
        if (state == Intermediate) {
            int i = input.indexOf(QLocale::system().decimalPoint());
            if ((i == -1 && input.size() > num_size)
                || (i != -1 && i > num_size))
                return Invalid;
        }
        return state;
    }
};

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget* parent = 0)
        : QLineEdit(parent)
    {
        setContextMenuPolicy(Qt::NoContextMenu);
        connect(this, SIGNAL(returnPressed()), SLOT(handleReturnPressed()));
    }

protected:
    void focusInEvent(QFocusEvent *e)
    {
        origText = text();
        QLineEdit::focusInEvent(e);
    }

    void focusOutEvent(QFocusEvent *e)
    {
        if (isModified() && !hasAcceptableInput())
            setText(origText);
        QLineEdit::focusOutEvent(e);
    }

private slots:
    void handleReturnPressed()
    {
        origText = text();
    }

private:
    QString origText;
};

class QPrintPreview : public QWidget
{
    Q_OBJECT
public:
    explicit QPrintPreview(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    explicit QPrintPreview(QPrinter *printer, QWidget *parent = 0, Qt::WindowFlags flags = 0);
   ~QPrintPreview();

//public:
//    explicit QPrintPreviewDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
//    explicit QPrintPreviewDialog(QPrinter *printer, QWidget *parent = 0, Qt::WindowFlags flags = 0);
//    ~QPrintPreviewDialog();


    QPrinter* getPrinter();

    //void setVisible(bool visible);
    //void done(int result);

signals:
    void paintRequested(QPrinter *printer);

protected:
    void init(QPrinter *printer = 0);
    void populateScene();
    void layoutPages();
    void setupActions();
    void updateNavActions();
    void setFitting(bool on);
    bool isFitting();
    void updatePageNumLabel();
    void updateZoomFactor();

private:
    QPrintDialog *printDialog;
    QPrintPreviewWidget *preview;
    QPrinter *printer;
    bool ownPrinter;
    bool initialized;

    // widgets:
    QLineEdit *pageNumEdit;
    QLabel *pageNumLabel;
    QComboBox *zoomFactor;

    // actions:
    QActionGroup* navGroup;
    QAction *nextPageAction;
    QAction *prevPageAction;
    QAction *firstPageAction;
    QAction *lastPageAction;

    QActionGroup* fitGroup;
    QAction *fitWidthAction;
    QAction *fitPageAction;

    QActionGroup* zoomGroup;
    QAction *zoomInAction;
    QAction *zoomOutAction;

    QActionGroup* orientationGroup;
    QAction *portraitAction;
    QAction *landscapeAction;

    QActionGroup* modeGroup;
    QAction *singleModeAction;
    QAction *facingModeAction;
    QAction *overviewModeAction;

    QActionGroup *printerGroup;
    QAction *printAction;
    QAction *pageSetupAction;
#if defined(Q_WS_MAC) && !defined(QT_MAC_USE_COCOA)
    QAction *closeAction;
#endif

    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;

private slots:
    void _q_fit(QAction *action);
    void _q_zoomIn();
    void _q_zoomOut();
    void _q_navigate(QAction *action);
    void _q_setMode(QAction *action);
    void _q_pageNumEdited();
    void _q_print();
    void _q_pageSetup();
    void _q_previewChanged();
    void _q_zoomFactorChanged();

signals:
    
public slots:
    void updatePreview();
    
};

#endif // QPRINTPREVIEW_H
