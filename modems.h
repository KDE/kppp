/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 *
 *
 *
 *            Copyright (C) 2004 Simone Gotti
 *               <simone.gotti@email.it>
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _MODEMS_H_
#define _MODEMS_H_

#include <qwidget.h>
#include <qpushbutton.h>
#include <q3listbox.h>
//Added by qt3to4:
#include <QLabel>
#include "acctselect.h"

class KPageDialog;
class QCheckBox;
class QLineEdit;
class Q3TabDialog;
class DialWidget;
class ScriptWidget;
class IPWidget;
class DNSWidget;
class GatewayWidget;

class ModemsWidget : public QWidget {
  Q_OBJECT
public:
  ModemsWidget( QWidget *parent=0, const char *name=0 );
  ~ModemsWidget() {}

private slots:
  void editmodem();
  void copymodem();
  void newmodem();
  void deletemodem();
  void slotListBoxSelect(int);
  void modemNameChanged(const QString &);

private:
  int doTab();

signals:
  void resetmodems();

private:
  QString prettyPrintVolume(unsigned int);

  KPageDialog *tabWindow;
	ModemWidget *modem1;
  ModemWidget2 *modem2;
 /* DialWidget *dial_w;
  AccountingSelector *acct;
  IPWidget *ip_w;
  DNSWidget *dns_w;
  GatewayWidget *gateway_w;
  ScriptWidget *script_w;*/

  QLabel *costlabel;
  QLineEdit *costedit;
  QLabel *vollabel;
  QLineEdit *voledit;

  Q3ListBox *modemlist_l;
  QPushButton *edit_b;
  QPushButton *copy_b;
  QPushButton *new_b;
  QPushButton *delete_b;
};

#endif

