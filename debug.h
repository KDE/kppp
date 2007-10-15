/*
 *        kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *            Copyright (C) 1997  Bernd Wuebben
 *                 wuebben@math.cornel.edu
 *
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

#ifndef _DEBUGWIDGET_
#define _DEBUGWIDGET_

#include <qdialog.h>
#include <qstring.h>
#include <qlabel.h>
#include <q3multilineedit.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QHideEvent>
#include <QFrame>
#include <QResizeEvent>


class myMultiEdit : public Q3MultiLineEdit {
public:

  explicit myMultiEdit(QWidget *parent=0, const char *name=0);

  void newLine();
};


class DebugWidget : public QDialog {
  Q_OBJECT
public:
  DebugWidget(QWidget *parent=0);

  void clear();

public slots:
  void statusLabel(const QString &);
  void toggleVisibility();
  void addChar(unsigned char);

protected:
  virtual void hideEvent(QHideEvent *);
  virtual void resizeEvent(QResizeEvent *e);

private:
  void enter();
  QFrame *fline;
  QPushButton *dismiss;
  myMultiEdit *text_window;

  QLabel *statuslabel;
};

#endif

