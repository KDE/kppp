/*
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

#ifndef _PPPDARGS_H_
#define _PPPDARGS_H_

#include <qdialog.h>

class QListBox;
class QLabel;
class QPushButton;
class QLineEdit;
class KPushButton;


class PPPdArguments : public QDialog {
Q_OBJECT
public:
  PPPdArguments(QWidget *parent=0, const char *name=0);
  ~PPPdArguments() {}

private slots:
  void addbutton();
  void removebutton();
  void defaultsbutton();
  void closebutton();
  void textChanged(const QString &);
  void itemSelected(int);

private:
  void init();

  QLabel *argument_label;

  QLineEdit *argument;

  QPushButton *add;
  QPushButton *remove;
  KPushButton *defaults;

  QListBox *arguments;

  QPushButton *closebtn;
};
#endif










