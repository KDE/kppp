/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <qwidget.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <q3groupbox.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <Q3Frame>
#include <kcolorbutton.h>

class QSlider;
class KIntNumInput;
class ModemCommands;

class GeneralWidget : public QWidget {
  Q_OBJECT
public:
  explicit GeneralWidget( QWidget *parent=0, const char *name=0 );

private slots:
  void 	pppdtimeoutchanged(int);
  void 	caption_toggled(bool);
  void  iconify_toggled(bool on);
  void 	redial_toggled(bool on);
  void 	nocarrier_toggled(bool on);
  void 	xserver_toggled(bool on);
  void  quit_toggled(bool);
  void  docking_toggled(bool on);

};


class ModemWidget : public QWidget {
  Q_OBJECT
public:
  explicit ModemWidget(QWidget *parent=0, bool isnewmodem=true);
    bool save();
    QLineEdit *connectName() { return connectname_l;}
private slots:

private:
	QLineEdit *connectname_l;
  QLabel *connect_label;

  QComboBox 	*enter;
  QLabel 	*label1;
  QLabel 	*label2;
  QLabel 	*labeltmp;
  QLabel 	*labelenter;
  QComboBox 	*modemdevice;
  QComboBox 	*flowcontrol;

  QComboBox *baud_c;
  QLabel *baud_label;

  KIntNumInput 	*modemtimeout;
  QCheckBox     *modemlockfile;
  QStringList    flowListItem;
};


class ModemWidget2 : public QWidget {
  Q_OBJECT
public:
  explicit ModemWidget2(QWidget *parent=0, const char *name=0);
	bool save();

private slots:
//  void 	use_cdline_toggled(bool);
  void 	modemcmdsbutton();
  void 	terminal();
  void 	query_modem();

private:
  QLabel 	*labeltmp;
  QPushButton 	*modemcmds;
  QPushButton 	*modeminfo_button;
  QPushButton 	*terminal_button;
  Q3Frame 	*fline;
  QCheckBox     *waitfordt;
  KIntNumInput 	*busywait;
  QCheckBox 	*chkbox1;
  QSlider       *volume;

	ModemCommands* mc;
};

class GraphSetup : public QWidget {
  Q_OBJECT
public:
  GraphSetup(QWidget *parent = 0);

private slots:
  void enableToggled(bool);
  void colorChanged(const QColor &);

private:
  void save();

  Q3GroupBox *grpColor;
  QCheckBox *enable_check;

  KColorButton *bg_color;
  KColorButton *text_color;
  KColorButton *in_color;
  KColorButton *out_color;

};

#endif


