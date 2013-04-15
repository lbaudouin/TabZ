#ifndef QPROGRESSBARDIALOG_H
#define QPROGRESSBARDIALOG_H

#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class QProgressBarDialog : public QWidget
{
    Q_OBJECT
public:
    QProgressBarDialog(){
        this->setWindowTitle("Download");
        vlayout = new QVBoxLayout(this);
        QLabel *label = new QLabel(tr("Please wait..."));
        vlayout->addWidget(label,0);
        progress = new QProgressBar;
        progress->setMinimum(0);
        progress->setMaximum(0);
        progress->setValue(0);
        vlayout->addWidget(progress);

        //TODO, use standard button ?
        QPushButton *button = new QPushButton(tr("Cancel"));
        connect(button,SIGNAL(clicked()),this,SLOT(pressCancel()));
        vlayout->addWidget(button);
    }

    void setValue(int value){progress->setValue(value);}
    void setMinimum(int minimum){progress->setMinimum(minimum);}
    void setMaximum(int maximum){progress->setMaximum(maximum);}
    void setFormat(QString format){progress->setFormat(format);}

private:
    QProgressBar *progress;
    QVBoxLayout *vlayout;

private slots:
    void pressCancel() {emit cancel();}

signals:
    void cancel();
};

#endif // QPROGRESSBARDIALOG_H
