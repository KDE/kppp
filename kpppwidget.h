/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 *            Copyright (C) 1998-2002 Harri Porten <porten@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __KPPPWIDGET_H__
#define __KPPPWIDGET_H__

#include <qstring.h>

#include "accounting.h"
#include "conwindow.h"
#include "accounts.h"
#include "connect.h"
#include "debug.h"
#include "pppstatdlg.h"

class GeneralWidget;
class ModemWidget;
class ModemWidget2;
class GraphSetup;
class AboutWidget;
class PPPStats;


class SignalEvent : public QEvent {
public:
  SignalEvent(int s) : QEvent(User), sig(s) { }
  int sigType() const { return sig; }
private:
  int sig;
};


class KPPPWidget : public QWidget {
  Q_OBJECT
public:

  KPPPWidget( QWidget *parent=0, const char *name=0 );
  ~KPPPWidget();

  void setPW_Edit(const QString &);
  virtual bool eventFilter(QObject *, QEvent *);

  void setQuitOnDisconnect (bool b);
  bool quitOnDisconnect () {return m_bQuitOnDisconnect;};

private slots:
  void newdefaultaccount(int);
  void expandbutton();
  void beginConnect();
  void quitbutton();
  void helpbutton();
  void setup();
  void rulesetLoadError();
  void usernameChanged(const QString &);
  void passwordChanged(const QString &);
  void enterPressedInID();
  void enterPressedInPW();
  void saveMyself();
  void shutDown();

public slots:
  void resetaccounts();
  void resetCosts(const QString &);
  void resetVolume(const QString &);
  void disconnect();
  void log_window_toggled(bool on);
  void startAccounting();
  void stopAccounting();
  void showStats();

signals:
  void begin_connect();
  void cmdl_start();

public:
  QCheckBox *log;
  bool connected;
  DebugWidget *debugwindow;
  QString con_speed;
  ConnectWidget *con;
  ConWindow *con_win;
  PPPStatsDlg *statdlg;
  AccountingBase *acct;
  QPushButton *quit_b;
  PPPStats *stats;

private:
  void prepareSetupDialog();
  void interruptConnection();
  void sigChld();
  void sigPPPDDied();
  QString encodeWord(const QString &s);
  void showNews ();

  QString ruleset_load_errmsg;

  QPushButton *help_b;
  QPushButton *setup_b;
  QFrame *fline;
  QFrame *fline1;
  QPushButton *connect_b;
  QComboBox *connectto_c;
  QLabel *ID_Label;
  QLabel *PW_Label;
  QLineEdit *ID_Edit;
  QLineEdit *PW_Edit;
  QLabel *label1;
  QLabel *label2;  
  QLabel *label3;
  QLabel *label4;
  QLabel *label5;
  QLabel *label6;
  QLabel *radio_label;


  QTabDialog *tabWindow;
  AccountWidget *accounts;
  GeneralWidget *general;
  ModemWidget *modem1;
  ModemWidget2 *modem2;
  GraphSetup *graph;
  AboutWidget *about;


  QString m_strCmdlAccount;
  bool m_bQuitOnDisconnect;
  bool m_bCmdlAccount;
};


#endif

