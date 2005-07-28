#ifndef _LOGINTERM_H_
#define _LOGINTERM_H_

#include <qdialog.h>
#include <q3multilineedit.h>
#include <qapplication.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QKeyEvent>

class LoginMultiLineEdit : public Q3MultiLineEdit {

  Q_OBJECT

public:

  LoginMultiLineEdit(QWidget *parent, const char *name);
  ~LoginMultiLineEdit();

  void keyPressEvent(QKeyEvent *k);
  void insertChar(unsigned char c);
  void myreturn();
  void mynewline();

public slots:
  void readChar(unsigned char);
};


class LoginTerm : public QDialog {
  Q_OBJECT
public:
  LoginTerm(QWidget *parent, const char *name);

  bool pressedContinue();

public slots:
  void cancelbutton();
  void continuebutton();

private:
  LoginMultiLineEdit *text_window;
  QPushButton *cancel_b;
  QPushButton *continue_b;

  bool cont;
};

#endif



