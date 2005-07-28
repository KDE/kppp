/* -*- C++ -*-
 *
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


#ifndef _EDIT_H_
#define _EDIT_H_

#include <qdialog.h>
#include <qpushbutton.h>
#include <q3groupbox.h> 
#include <qscrollbar.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <q3listbox.h>
#include <qradiobutton.h>
#include <q3buttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <kdialogbase.h>
#include "scriptedit.h"
#include "kpppconfig.h"
#include "pppdargs.h"
class IPLineEdit;

class DialWidget : public QWidget {
  Q_OBJECT
public:
  DialWidget( QWidget *parent=0, bool isnewaccount = true, const char *name=0 );
  ~DialWidget() {}

public slots:
  bool save();
  void pppdargsbutton();
  void numbersChanged();
  void cbtypeChanged(int);
  void selectionChanged(int);
  void addNumber();
  void delNumber();
  void upNumber();
  void downNumber();
  
private:
  QLineEdit *connectname_l;
  QLabel *connect_label;
  QLabel *number_label;
  QPushButton *pppdargs;
  QComboBox *auth;
  QLabel *auth_l;
  QCheckBox *store_password;

  // callback support
  QComboBox *cbtype;
  QLabel *cbtype_l;
  QLineEdit *cbphone;
  QLabel *cbphone_l;
  // for the phonenumber selection
  QPushButton *add, *del, *up, *down;
  Q3ListBox *numbers;
};


/////////////////////////////////////////////////////////////////////////////
//
// tab-window to select what to execute when
//
/////////////////////////////////////////////////////////////////////////////
class ExecWidget : public QWidget {
  Q_OBJECT
public:
  ExecWidget(QWidget *parent=0, bool isnewaccount=true, const char *name=0);

public slots:
  bool save();

private:
  QLineEdit *before_connect;
  QLabel *before_connect_l;

  QLineEdit *command;
  QLabel *command_label;

  QLineEdit *predisconnect;
  QLabel *predisconnect_label;

  QLineEdit *discommand;
  QLabel *discommand_label;
};


class IPWidget : public QWidget {
  Q_OBJECT
public:
  IPWidget( QWidget *parent=0, bool isnewaccount = true, const char *name=0 );
  ~IPWidget() {}

public slots:
  void save();

protected slots:
  void hitIPSelect( int );
  void autoname_t(bool on);

private:
  QLabel *ipaddress_label;
  QLabel *sub_label;
  Q3GroupBox *box1;
  Q3GroupBox *box;

  Q3ButtonGroup *rb;
  QRadioButton *dynamicadd_rb;
  QRadioButton *staticadd_rb;

  IPLineEdit *ipaddress_l;
  IPLineEdit *subnetmask_l;

  QCheckBox *autoname;
};


class DNSWidget : public QWidget {
  Q_OBJECT
public:
  DNSWidget( QWidget *parent=0, bool isnewaccount = true, const char *name=0 );
  ~DNSWidget() {}

public slots:
  void save();

protected slots:
  void adddns();
  void removedns();
  void DNS_Edit_Changed(const QString &);
  void DNS_Entry_Selected(int);
  void DNS_Mode_Selected(int);

private:
  QLabel *conf_label;
  Q3ButtonGroup *bg;
  QRadioButton *autodns, *mandns;
  QLabel *dns_label;
  QLabel *servers_label;
  IPLineEdit *dnsipaddr;
  QPushButton *add;
  QPushButton *remove;
  Q3ListBox *dnsservers;
  QLineEdit *dnsdomain;
  QLabel *dnsdomain_label;
  QCheckBox *exdnsdisabled_toggle;
};


class GatewayWidget : public QWidget {
  Q_OBJECT
public:
  GatewayWidget( QWidget *parent=0, bool isnewaccount = true, const char *name=0 );
  ~GatewayWidget() {}

public slots:
  void save();

private slots:
  void hitGatewaySelect( int );

private:
  Q3GroupBox *box;
  QLabel *gate_label;
  Q3GroupBox *box1;
  Q3ButtonGroup *rb;
  QRadioButton *defaultgateway;
  QRadioButton *staticgateway;
  IPLineEdit *gatewayaddr;
  QCheckBox *defaultroute;
};


class ScriptWidget : public QWidget {
  Q_OBJECT
public:
  ScriptWidget( QWidget *parent=0, bool isnewaccount = true, const char *name=0 );
  ~ScriptWidget() {}

public slots:
  void save();
  bool check();

private slots:
  void addButton();
  void insertButton();
  void removeButton();

  //signals linked to the scroll bar
  void scrolling(int);

  //signals to keep the two listboxes highlighted in sync
  void slhighlighted(int);
  void stlhighlighted(int);

private:
  void adjustScrollBar();

  ScriptEdit *se;
  QPushButton *add;
  QPushButton *remove;
  QPushButton *insert;
  Q3ListBox *sl, *stl;
  
  QScrollBar *slb;
};


/////////////////////////////////////////////////////////////////////////////
//
// Used to specify a new phone number
//
/////////////////////////////////////////////////////////////////////////////
class PhoneNumberDialog : public KDialogBase {
  Q_OBJECT
public:
  PhoneNumberDialog(QWidget *parent = 0);
  
  QString phoneNumber();

private slots:
  void textChanged(const QString &);

private: 
  QLineEdit *le;
};


#endif
