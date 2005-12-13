/*
 *
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 *
 * Copyright (C) 1997 Bernd Johannes Wuebben
 * wuebben@math.cornell.edu
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */


#ifndef _MINITERM_H_
#define _MINITERM_H_

#include <qdialog.h>
#include <qpixmap.h>
#include <qevent.h>
#include <q3multilineedit.h>
//Added by qt3to4:
#include <QLabel>
#include <QKeyEvent>
#include <QMenu>
#include <QCloseEvent>
#include <ktoolbar.h>

class KHelpMenu;

class QTimer;
class QMenuBar;
class KToolBar;
class KHelpMenu;
class QPushButton;
class QLabel;

class MyTerm : public Q3MultiLineEdit {
  Q_OBJECT
public:
  MyTerm(QWidget *parent=0, const char *name=0);

  void keyPressEvent (QKeyEvent*);
  void insertChar(unsigned char c);
  void newLine();
  void backspace();
  void del();
  void myreturn();
  void mynewline();
};


class MiniTerm : public QDialog {
  Q_OBJECT
public:

  MiniTerm(QWidget *parent=0, const char *name=0);
  ~MiniTerm();

  void closeEvent( QCloseEvent *e );

public slots:
  void cancelbutton();
  void init();
  void readChar(unsigned char);
  void help();
  void resetModem();

protected:
  void setupToolbar();

  QPushButton *cancel;
  MyTerm *terminal;
  QTimer *inittimer;

  QMenuBar    * menubar;
  KToolBar     * toolbar;
  QMenu  * m_file;
  QMenu  * m_edit;
  QMenu  * m_options;
  KHelpMenu  * m_help;
  QLabel      * statusbar;
  QPushButton *pb1;
  QPushButton *pb2;
  QPushButton *pb3;
  QPushButton *pb4;

  QPixmap pb1_pixmap;
  QPixmap pb2_pixmap;
  QPixmap pb3_pixmap;
  QPixmap pb4_pixmap;
};


#endif
