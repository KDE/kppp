/* 
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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


#ifndef _MAIN_H_
#define _MAIN_H_

// this include is needed since gcc sometimes gets 
// confused if qdir.h is included later (strange error)
#include <qdir.h>


#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <qapplication.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qtimer.h>
#include <qlcdnumber.h>
#include <qpainter.h>
#include <qtabdialog.h>
#include <qradiobutton.h>
#include <qpixmap.h> 
#include <qcheckbox.h>

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

class KPPPWidget : public QWidget {
  Q_OBJECT
public:

  KPPPWidget( QWidget *parent=0, const char *name=0 );
  ~KPPPWidget();

  void setPW_Edit(const QString &);
  virtual bool eventFilter(QObject *, QEvent *);

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
  void sigInt();
  void sigChld();
  void sigPPPDDied();

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
};

void killpppd();
void sighandler(int);
pid_t execute_command(const QString &);
pid_t create_pidfile();
bool remove_pidfile();
void myShutDown();

#endif
