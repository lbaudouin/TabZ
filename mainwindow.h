#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTabWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QToolButton>
#include <QUrl>

#include "tab.h"
#include "xta.h"
#include "options.h"
#include "optionsform.h"
#include "chords.h"

struct RecentFile{
    QString title;
    QString artist;
    QString path;
    QDateTime date;
    bool wasOpen;
};
struct QPairFirstComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first < b.first;
    }
};
struct QPairSecondComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.second < b.second;
    }
};
struct QPairFirstComparerInverse
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first > b.first;
    }
};
struct QPairSecondComparerInverse
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.second > b.second;
    }
};



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void setUpToolBar();
    void setUpMenu();
    int addTab(XTAinfo info);
    Tab* getCurrentTab();
    
    void readRecent();
    void saveRecent();
    void updateRecent();
    void addRecent(XTAinfo &info);

private:
    Ui::MainWindow *ui;
    Qt::WindowStates previousState;
    XTA *xta;
    Chords *chords;

    OptionsValues options;

    QMap<QAction*,Tab*> mapTabAction;

    QList<RecentFile> recent;
    QList<QObject*> recentAction;

    QTime startTime;

    QAction *exitFullScreenAction;

signals:
    void setColorsEnabled(bool);
    void setColors(QList<ColorRegExp>);
    void optionsChanged(OptionsValues);

    //For single instance
    void needToShow();

private slots:
    void openFile();
    void openFile(QString);
    void clearRecent();

public slots:
    void pressNew(QString text = QString());
    void pressOpen();
    void pressOpenFolder();
    void pressOpenPrevious();
    void pressSave();
    void pressSaveAs();
    void pressClose();
    void pressCloseAll();
    void pressSetFullScreen();
    void pressExitFullScreen();

    void pressPrevious();
    void pressNext();
    void pressGoTo();

    void pressEditMode();
    void pressReadOnlyMode();

    void pressSelectAll();
    void pressUndo();
    void pressRedo();
    void pressCut();
    void pressCopy();
    void pressPaste();

    void pressPreview();
    void pressPrint();
    void pressExportPDF();

    void pressPreference();
    void pressChordsManager();

    void pressInsertTab();

    void pressSearchLyrics();
    void pressSearchXTA();

    void displaySaveIcon(int,bool);

    void tabCloseRequest(int);

    void currentTabChanged(int);
    void setUndoAvailable(bool);
    void setRedoAvailable(bool);

    void restart(QString);


    //For single instance
    void handleMessage(const QString& message);

};

#endif // MAINWINDOW_H
