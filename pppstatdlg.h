/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _PPPSTATSDLG_H_
#define _PPPSTATSDLG_H_


#include <qdialog.h>
#include <qevent.h>
#include <qpixmap.h>

class QLabel;
class QFrame;
class QGroupBox;
class QLineEdit;
class QTimer;
class QPushButton;

const int MAX_GRAPH_WIDTH   = 600;
const int GRAPH_UPDATE_TIME = 1000;

class PPPStats;

class PPPStatsDlg : public QWidget {

  Q_OBJECT

public:

  PPPStatsDlg(QWidget *parent, const char *name,QWidget *main,
	      PPPStats *st);
  ~PPPStatsDlg();

protected:

  void closeEvent( QCloseEvent *e );
  void paintEvent (QPaintEvent *e) ;
  bool event( QEvent *e );

public slots:

  void timeclick();
  void cancel();
  void take_stats();
  void stop_stats();
  void paintIcon(int);
  void updateGraph();

public:

  void update_data();
  void toCurrentDesktop();

private:
  void paintGraph();
  PPPStats *stats;

  QLabel *pixmap_l;
  QWidget *main;
  QPushButton *cancelbutton;

  QLabel *labela1[5];
  QLabel *labela2[5];
  QLabel *labelb1[5];
  QLabel *labelb2[5];

  QLabel *ip_address_label1;
  QLineEdit *ip_address_label2;
  QLabel *ip_address_label3;
  QLineEdit *ip_address_label4;

  QLabel *modem_pic_label;
  QPixmap modem_pixmap;
  QPixmap modem_left_pixmap;
  QPixmap modem_right_pixmap;
  QPixmap modem_both_pixmap;
  QPixmap big_modem_both_pixmap;
  QPixmap big_modem_left_pixmap;
  QPixmap big_modem_right_pixmap;
  QPixmap big_modem_none_pixmap;

  int ibytes_last;
  int obytes_last;
  bool need_to_paint;

  QString 	ibytes_string;
  QString 	ipackets_string;
  QString 	compressedin_string;
  QString 	uncompressedin_string;
  QString 	errorin_string;
  QString 	obytes_string;
  QString	opackets_string;
  QString	compressed_string;
  QString 	packetsunc_string;
  QString 	packetsoutunc_string;
  QGroupBox    *box;

  // graph widget
  QFrame *graph;
  int bin[MAX_GRAPH_WIDTH];
  int bout[MAX_GRAPH_WIDTH];
  int bin_last;
  int bout_last;
  int ringIdx;
  int max;
  QTimer *graphTimer;
  QColor bg, text, in, out;
};

#endif
