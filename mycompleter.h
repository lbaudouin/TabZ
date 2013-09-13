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
  inline MyCompleter(const QStringList& words, QObject * parent) : QCompleter(parent), m_model()
    {
      setModel(&m_model);
      setStringList(words);
    }

  inline void update(QString word)
  {
    // Do any filtering you like.
    // Here we just include all items that contain word.
    QStringList filtered = m_list.filter(removeAccents(word), caseSensitivity());
    m_model.setStringList(filtered);
    m_word = word;
    complete();
  }

  inline QString word()
  {
    return m_word;
  }

  inline void setStringList(const QStringList &list)
  {
      m_list.clear();
      foreach(QString word, list){
          m_list.push_back( removeAccents(word) );
      }
      //m_list = list;
  }

  inline QString removeAccents(QString s) {
      if (diacriticLetters_.isEmpty()) {
          diacriticLetters_ = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
          noDiacriticLetters_ << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
      }

      QString output = "";
      for (int i = 0; i < s.length(); i++) {
          QChar c = s[i];
          int dIndex = diacriticLetters_.indexOf(c);
          if (dIndex < 0) {
              output.append(c);
          } else {
              QString replacement = noDiacriticLetters_[dIndex];
              output.append(replacement);
          }
      }

      return output;
  }

 private:
  QStringList m_list;
  QStringListModel m_model;
  QString m_word;
  QString diacriticLetters_;
  QStringList noDiacriticLetters_;
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
