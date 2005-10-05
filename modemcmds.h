
/*
 *
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 *
 * Copyright (C) 1997 Bernd Johannes Wuebben
 * wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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


#ifndef _MODEMCMDS_H_
#define _MODEMCMDS_H_

#include <q3groupbox.h>
#include <kdialogbase.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <pppdata.h>

class QLineEdit;
class QLabel;
class QPushButton;
class Q3GroupBox;

class ModemCommands : public KDialogBase {

Q_OBJECT

public:

  ModemCommands(QWidget *parent=0, const char *name=0);
  ~ModemCommands() {}

private slots:
  void slotCancel();
  void slotOk();

private:

  Q3GroupBox *box;

  QLineEdit *initstr[int(PPPData::NumInitStrings)];

  QLineEdit *initresp;
  QLabel *label2;

  QSlider *preinitslider;
  QLabel *lpreinitslider;
  QLabel *lpreinit;

  QSlider *initslider;
  QLabel *linitslider;
  QLabel *label3;

  QSlider *durationslider;
  QLabel *ldurationslider;
  QLabel *lduration;

  QLineEdit *nodetectdialtone;
  QLabel *lnodetectdialtone;

  QLineEdit *dialstr;
  QLabel *label4;

  QLineEdit *connectresp;
  QLabel *label5;

  QLineEdit *busyresp;
  QLabel *label6;

  QLineEdit *nocarrierresp;
  QLabel *label7;

  QLineEdit *nodialtoneresp;
  QLabel *label8;

  QLineEdit *hangupstr;
  QLabel *label9;

  QLineEdit *hangupresp;
  QLabel *label10;

  QLineEdit *answerstr;
  QLabel *label11;

  QLineEdit *ringresp;
  QLabel *label12;

  QLineEdit *answerresp;
  QLabel *label13;

  QLineEdit *escapestr;
  QLabel *label14;

  QLineEdit *escaperesp;
  QLineEdit *dlpresp;
  QLabel *label17;

  QLabel *label15;

  QSlider *slider;
  QLabel *lslider;
  QLabel *label16;

  QLineEdit *volume_off, *volume_medium, *volume_high;
};
#endif



