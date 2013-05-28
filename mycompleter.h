#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QCompleter>
#include <QKeyEvent>

class MyCompleter : public QCompleter
{
  Q_OBJECT

    public:
  inline MyCompleter(const QStringList& words, QObject * parent) : QCompleter(parent), m_list(words), m_model()
    {
      setModel(&m_model);
    }

  inline void update(QString word)
  {
    // Do any filtering you like.
    // Here we just include all items that contain word.
    QStringList filtered = m_list.filter(word, caseSensitivity());
    m_model.setStringList(filtered);
    m_word = word;
    complete();
  }

  inline QString word()
  {
    return m_word;
  }

  inline void setStringList(QStringList &list)
  {
      m_list = list;
  }

 private:
  QStringList m_list;
  QStringListModel m_model;
  QString m_word;
};

class MyCompleterLineEdit : public QLineEdit
{
  Q_OBJECT

    public:
  MyCompleterLineEdit(QWidget *parent = 0);
  ~MyCompleterLineEdit();

  void setCompleter(MyCompleter *c);
  MyCompleter *completer() const;

 protected:
  //void keyPressEvent(QKeyEvent *e);

  private slots:
  void insertCompletion(const QString &completion);

 private:
  MyCompleter *c;

public slots:
  void updateHint();

signals:
  void selectionChanged(QString);
};

#endif // LINEEDIT_H
