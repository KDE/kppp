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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __KPPPWIDGET_H__
#define __KPPPWIDGET_H__

#include <qstring.h>
//Added by qt3to4:
#include <QLabel>
#include <Q3Frame>
#include <QGridLayout>
#include <QEvent>


#include "accounting.h"
#include "conwindow.h"
#include "accounts.h"
#include "connect.h"
#include "debug.h"
#include "pppstatdlg.h"


class GeneralWidget;
class ModemsWidget;
class ModemWidget;
class GraphSetup;
class AboutWidget;
class PPPStats;
class KPageDialog;
class KPushButton;

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

  explicit KPPPWidget( QWidget *parent=0, const char *name=0 );
  ~KPPPWidget();

  void setPW_Edit(const QString &);
  virtual bool eventFilter(QObject *, QEvent *);

  void setQuitOnDisconnect (bool b);
  bool quitOnDisconnect () {return m_bQuitOnDisconnect;}

private slots:
  void newdefaultaccount(int);
	void newdefaultmodem(int);
  void expandbutton();
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

  void delayedDisconnect();

public slots:
  void beginConnect();
  void resetaccounts();
	void resetmodems();
  void resetCosts(const QString &);
  void resetVolume(const QString &);
  void disconnect();
  void log_window_toggled(bool on);
  void startAccounting();
  void stopAccounting();
  void showStats();
  bool isConnected() const {return connected;}
signals:
  void begin_connect();
  void cmdl_start();

 signals: //dbus signal
    void sig_aboutToConnect();
    void sig_aboutToDisconnect();
    void sig_connected();
    void sig_disconnected();

public:
  QCheckBox *log;
  bool connected;
  DebugWidget *debugwindow;
  QString con_speed;
  ConnectWidget *con;
  ConWindow *con_win;
  PPPStatsDlg *statdlg;
  AccountingBase *acct;
  KPushButton *quit_b;
  PPPStats *stats;

private:
  void prepareSetupDialog();
  void interruptConnection();
  void sigChld();
  void sigPPPDDied();
  QString encodeWord(const QString &s);
  void showNews ();
	void setButtons();

  QString ruleset_load_errmsg;

  QGridLayout *l1;
  KPushButton *help_b;
  KPushButton *setup_b;
  QFrame *fline;
  QFrame *fline1;
  QPushButton *connect_b;
  QComboBox *connectto_c;
  QComboBox *modem_c;
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
	QLabel *label7;
  QLabel *radio_label;


  KPageDialog *tabWindow;
  AccountWidget *accounts;
  GeneralWidget *general;
  ModemsWidget *modems;
  GraphSetup *graph;
  AboutWidget *about;


  QString m_strCmdlAccount;
  QString m_strCmdlModem;
  bool m_bQuitOnDisconnect;
  bool m_bCmdlAccount;
  bool m_bCmdlModem;
  bool m_bModemCShown;

  QTimer *disconnectTimer;
};


#endif

