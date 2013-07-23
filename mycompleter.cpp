#include "mycompleter.h"

MyCompleterLineEdit::MyCompleterLineEdit(QWidget *parent)
  : QLineEdit(parent), c(0)
{
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(updateHint()));
}

MyCompleterLineEdit::~MyCompleterLineEdit()
{
}

void MyCompleterLineEdit::setCompleter(MyCompleter *completer)
{
  if (c)
    QObject::disconnect(c, 0, this, 0);

  c = completer;

  if (!c)
    return;

  c->setWidget(this);
  connect(c, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
}

MyCompleter *MyCompleterLineEdit::completer() const
{
  return c;
}

void MyCompleterLineEdit::insertCompletion(const QString& completion)
{
  setText(completion);
  c->popup()->hide();
  //selectAll();
  //insert(completion);
  setText(completion);
  emit selectionChanged(completion);
}

void MyCompleterLineEdit::updateHint()
{
    c->update(text());
    c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
}

