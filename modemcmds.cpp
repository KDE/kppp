/*
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
 * License along with dummyWidget program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <qlayout.h>
#include <qslider.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <kbuttonbox.h>
#include <stdlib.h>
#include <kapplication.h> // for getMiniIcon()
#include <klocale.h>
#include "modemcmds.h"
#include "pppdata.h"
#include <kwin.h>

#include <q3groupbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>


#define ADJUSTEDIT(e) e->setText("XXXXXXXXqy"); e->setMinimumSize(e->sizeHint()); e->setFixedHeight(e->sizeHint().height()); e->setText(""); e->setMaxLength(MODEMSTR_SIZE);

// a little trick to make the label look like a disabled lineedit
#define FORMATSLIDERLABEL(l) l->setFixedWidth(l->sizeHint().width()); l->setFixedHeight(QLineEdit(dummyWidget).sizeHint().height()); l->setAlignment(Qt::AlignCenter); l->setFrameStyle(Q3Frame::WinPanel|Q3Frame::Sunken); l->setLineWidth(2);

ModemCommands::ModemCommands(QWidget *parent, const char *name)
  : KDialogBase(parent, name, true, i18n("Edit Modem Commands"), Ok|Cancel)
{
  KWin::setIcons(winId(), kapp->icon(), kapp->miniIcon());
  QWidget *dummyWidget = new QWidget(this);
  setMainWidget(dummyWidget);

  const int GRIDROWS = 22;
  int row = 0;

  // toplevel layout
  QVBoxLayout *tl = new QVBoxLayout(dummyWidget, 10, 4);

  // add grid + frame
  QGridLayout *l1 = new QGridLayout(GRIDROWS, 4);
  tl->addLayout(l1);
  box = new Q3GroupBox(dummyWidget, "box");
  l1->addMultiCellWidget(box, row++, GRIDROWS, 0, 3);

  // put slider and label into a separate H-Box
  QHBoxLayout *l2 = new QHBoxLayout;
  l1->addLayout(l2, row, 2);
  lpreinitslider = new QLabel("MMMM", dummyWidget);
  FORMATSLIDERLABEL(lpreinitslider);

  preinitslider = new QSlider(0, 300, 1, 0,
                                       Qt::Horizontal, dummyWidget);
  preinitslider->setFixedHeight(preinitslider->sizeHint().height());
  connect(preinitslider, SIGNAL(valueChanged(int)),
	  lpreinitslider, SLOT(setNum(int)));
  l2->addWidget(lpreinitslider, 0);
  l2->addWidget(preinitslider, 1);

  lpreinit = new QLabel(i18n("Pre-init delay (sec/100):"), dummyWidget);
  l1->addWidget(lpreinit, row++, 1);

  for(int i = 0; i < PPPData::NumInitStrings; i++) {
      initstr[i] = new QLineEdit(dummyWidget);
      QLabel *initLabel = new QLabel(i18n("Initialization string %1:").arg(i + 1),
				    dummyWidget);
      ADJUSTEDIT(initstr[i]);
      l1->addWidget(initLabel, row, 1);
      l1->addWidget(initstr[i], row++, 2);
  }

  QHBoxLayout *l3 = new QHBoxLayout;
  l1->addLayout(l3, row, 2);
  linitslider = new QLabel("MMMM", dummyWidget);
  FORMATSLIDERLABEL(linitslider);
  initslider = new QSlider(1, 300, 1, 0,
				Qt::Horizontal, dummyWidget);
  initslider->setFixedHeight(initslider->sizeHint().height());
  connect(initslider, SIGNAL(valueChanged(int)),
	  linitslider, SLOT(setNum(int)));
  l3->addWidget(linitslider, 0);
  l3->addWidget(initslider, 1);

  label3 = new QLabel(i18n("Post-init delay (sec/100):"), dummyWidget);
  l1->addWidget(label3, row++, 1);

  /* Set ATS11 (Dial tone duration) between 0-255 (Default ~ 70) */
  QHBoxLayout *l4 = new QHBoxLayout;
  l1->addLayout(l4, row, 2);
  ldurationslider = new QLabel("MMMM", dummyWidget);
  FORMATSLIDERLABEL(ldurationslider);
  durationslider = new QSlider(1, 255, 1, 0,
				Qt::Horizontal, dummyWidget);
  durationslider->setFixedHeight(durationslider->sizeHint().height());
  connect(durationslider, SIGNAL(valueChanged(int)),
	  ldurationslider, SLOT(setNum(int)));
  l4->addWidget(ldurationslider, 0);
  l4->addWidget(durationslider, 1);

  lduration = new QLabel(i18n("Dialing speed (sec/100):"), dummyWidget);
  l1->addWidget(lduration, row++, 1);


  initresp = new QLineEdit(dummyWidget);
  label2 = new QLabel(i18n("Init &response:"), dummyWidget);
  label2->setBuddy(initresp);
  ADJUSTEDIT(initresp);
  l1->addWidget(label2, row, 1);
  l1->addWidget(initresp, row++, 2);

  nodetectdialtone = new QLineEdit(dummyWidget);
  lnodetectdialtone = new QLabel(i18n("No di&al tone detection:"), dummyWidget);
  lnodetectdialtone->setBuddy(nodetectdialtone);
  ADJUSTEDIT(nodetectdialtone);
  l1->addWidget(lnodetectdialtone, row, 1);
  l1->addWidget(nodetectdialtone, row++, 2);

  dialstr = new QLineEdit(dummyWidget);
  label4 = new QLabel(i18n("Dial &string:"),dummyWidget);
  label4->setBuddy(dialstr);
  ADJUSTEDIT(dialstr);
  l1->addWidget(label4, row, 1);
  l1->addWidget(dialstr, row++, 2);

  connectresp = new QLineEdit(dummyWidget);
  label5 = new QLabel(i18n("Co&nnect response:"), dummyWidget);
  label5->setBuddy(connectresp);
  ADJUSTEDIT(connectresp);
  l1->addWidget(label5, row, 1);
  l1->addWidget(connectresp, row++, 2);

  busyresp = new QLineEdit(dummyWidget);
  label6 = new QLabel(i18n("Busy response:"), dummyWidget);
  ADJUSTEDIT(busyresp);
  l1->addWidget(label6, row, 1);
  l1->addWidget(busyresp, row++, 2);

  nocarrierresp = new QLineEdit(dummyWidget);
  label7 = new QLabel(i18n("No carr&ier response:"), dummyWidget);
  label7->setBuddy(nocarrierresp);
  ADJUSTEDIT(nocarrierresp);
  l1->addWidget(label7, row, 1);
  l1->addWidget(nocarrierresp, row++, 2);

  nodialtoneresp = new QLineEdit(dummyWidget);
  label8 = new QLabel(i18n("No dial tone response:"), dummyWidget);
  ADJUSTEDIT(nodialtoneresp);
  l1->addWidget(label8, row, 1);
  l1->addWidget(nodialtoneresp, row++, 2);

  hangupstr = new QLineEdit(dummyWidget);
  label9 = new QLabel(i18n("&Hangup string:"), dummyWidget);
  label9->setBuddy(hangupstr);
  ADJUSTEDIT(hangupstr);
  l1->addWidget(label9, row, 1);
  l1->addWidget(hangupstr, row++, 2);

  hangupresp = new QLineEdit(dummyWidget);
  label10 = new QLabel(i18n("Hangup response:"), dummyWidget);
  ADJUSTEDIT(hangupresp);
  l1->addWidget(label10, row, 1);
  l1->addWidget(hangupresp, row++, 2);

  answerstr = new QLineEdit(dummyWidget);
  label11 = new QLabel(i18n("Answ&er string:"), dummyWidget);
  label11->setBuddy(answerstr);
  ADJUSTEDIT(answerstr);
  l1->addWidget(label11, row, 1);
  l1->addWidget(answerstr, row++, 2);

  ringresp = new QLineEdit(dummyWidget);
  label12 = new QLabel(i18n("Ring response:"), dummyWidget);
  ADJUSTEDIT(ringresp);
  l1->addWidget(label12, row, 1);
  l1->addWidget(ringresp, row++, 2);

  answerresp = new QLineEdit(dummyWidget);
  label13 = new QLabel(i18n("Ans&wer response:"), dummyWidget);
  label13->setBuddy(answerresp);
  ADJUSTEDIT(answerresp);
  l1->addWidget(label13, row, 1);
  l1->addWidget(answerresp, row++, 2);

  dlpresp = new QLineEdit(dummyWidget);
  label17 = new QLabel(i18n("DLP response:"), dummyWidget);
  ADJUSTEDIT(dlpresp);
  l1->addWidget(label17, row, 1);
  l1->addWidget(dlpresp, row++, 2);

  escapestr = new QLineEdit(dummyWidget);
  label14 = new QLabel(i18n("Escape strin&g:"), dummyWidget);
  label14->setBuddy(escapestr);
  ADJUSTEDIT(escapestr);
  l1->addWidget(label14, row, 1);
  l1->addWidget(escapestr, row++, 2);

  escaperesp = new QLineEdit(dummyWidget);
  label15 = new QLabel(i18n("Escape response:"), dummyWidget);
  ADJUSTEDIT(escaperesp);
  l1->addWidget(label15, row, 1);
  l1->addWidget(escaperesp, row++, 2);

  QHBoxLayout *l5 = new QHBoxLayout;
  l1->addLayout(l5, row, 2);
  lslider = new QLabel("MMMM", dummyWidget);
  FORMATSLIDERLABEL(lslider);

  slider = new QSlider(0, 255, 1, 0,
				Qt::Horizontal, dummyWidget);
  slider->setFixedHeight(slider->sizeHint().height());
  connect(slider, SIGNAL(valueChanged(int)),
	  lslider, SLOT(setNum(int)));
  l5->addWidget(lslider, 0);
  l5->addWidget(slider, 1);

  label16 = new QLabel(i18n("Guard time (sec/50):"), dummyWidget);
  l1->addWidget(label16, row++, 1);

  QLabel *l = new QLabel(i18n("Volume off/low/high:"), dummyWidget);
  l1->addWidget(l, row, 1);
  QHBoxLayout *l6 = new QHBoxLayout;
  l1->addLayout(l6, row++, 2);
  volume_off = new QLineEdit(dummyWidget);
  volume_off->setFixedHeight(volume_off->sizeHint().height());
  volume_off->setMinimumWidth((int)(volume_off->sizeHint().width() / 2));
  volume_medium = new QLineEdit(dummyWidget);
  volume_medium->setFixedHeight(volume_medium->sizeHint().height());
  volume_medium->setMinimumWidth((int)(volume_medium->sizeHint().width() / 2));
  volume_high = new QLineEdit(dummyWidget);
  volume_high->setFixedHeight(volume_high->sizeHint().height());
  volume_high->setMinimumWidth((int)(volume_high->sizeHint().width() / 2));
  l6->addWidget(volume_off);
  l6->addWidget(volume_medium);
  l6->addWidget(volume_high);

  initstr[0]->setFocus();

  l1->addColSpacing(0, 10);
  l1->addColSpacing(3, 10);
  l1->addRowSpacing(0, 5);
  l1->addRowSpacing(GRIDROWS, 5);

  //set stuff from gpppdata
  preinitslider->setValue(gpppdata.modemPreInitDelay());
  lpreinitslider->setNum(gpppdata.modemPreInitDelay());
  for(int i = 0; i < PPPData::NumInitStrings; i++)
      initstr[i]->setText(gpppdata.modemInitStr(i));
  initslider->setValue(gpppdata.modemInitDelay());
  linitslider->setNum(gpppdata.modemInitDelay());
  initresp->setText(gpppdata.modemInitResp());

  durationslider->setValue(gpppdata.modemToneDuration());
  ldurationslider->setNum(gpppdata.modemToneDuration());

  nodetectdialtone->setText(gpppdata.modemNoDialToneDetectionStr());
  dialstr->setText(gpppdata.modemDialStr());
  dlpresp->setText(gpppdata.modemDLPResp());

  connectresp->setText(gpppdata.modemConnectResp());
  busyresp->setText(gpppdata.modemBusyResp());
  nocarrierresp->setText(gpppdata.modemNoCarrierResp());
  nodialtoneresp->setText(gpppdata.modemNoDialtoneResp());

  escapestr->setText(gpppdata.modemEscapeStr());
  escaperesp->setText(gpppdata.modemEscapeResp());

  hangupstr->setText(gpppdata.modemHangupStr());
  hangupresp->setText(gpppdata.modemHangupResp());

  answerstr->setText(gpppdata.modemAnswerStr());
  ringresp->setText(gpppdata.modemRingResp());
  answerresp->setText(gpppdata.modemAnswerResp());

  slider->setValue(gpppdata.modemEscapeGuardTime());
  lslider->setNum(gpppdata.modemEscapeGuardTime());

  volume_off->setText(gpppdata.volumeOff());
  volume_medium->setText(gpppdata.volumeMedium());
  volume_high->setText(gpppdata.volumeHigh());
	
	// Save the data, so if it not exist we save the default
	gpppdata.save();
}


void ModemCommands::slotOk() {
  gpppdata.setModemPreInitDelay(lpreinitslider->text().toInt());
  for(int i = 0; i < PPPData::NumInitStrings; i++)
      gpppdata.setModemInitStr(i, initstr[i]->text());
  gpppdata.setModemInitResp(initresp->text());
  gpppdata.setModemInitDelay(linitslider->text().toInt());

  gpppdata.setModemToneDuration(ldurationslider->text().toInt());
  gpppdata.setModemNoDialToneDetectionStr(nodetectdialtone->text());
  gpppdata.setModemDialStr(dialstr->text());
  gpppdata.setModemDLPResp(dlpresp->text());

  gpppdata.setModemConnectResp(connectresp->text());
  gpppdata.setModemBusyResp(busyresp->text());
  gpppdata.setModemNoCarrierResp(nocarrierresp->text());
  gpppdata.setModemNoDialtoneResp(nodialtoneresp->text());

  gpppdata.setModemEscapeStr(escapestr->text());
  gpppdata.setModemEscapeResp(escaperesp->text());
  gpppdata.setModemEscapeGuardTime(lslider->text().toInt());
  gpppdata.setModemHangupStr(hangupstr->text());
  gpppdata.setModemHangupResp(hangupresp->text());

  gpppdata.setModemAnswerStr(answerstr->text());
  gpppdata.setModemRingResp(ringresp->text());
  gpppdata.setModemAnswerResp(answerresp->text());

  gpppdata.setVolumeHigh(volume_high->text());
  gpppdata.setVolumeMedium(volume_medium->text());
  gpppdata.setVolumeOff(volume_off->text());

  gpppdata.save();
  accept();
}


void ModemCommands::slotCancel() {
    //set stuff from gpppdata
  preinitslider->setValue(gpppdata.modemPreInitDelay());
  lpreinitslider->setNum(gpppdata.modemPreInitDelay());
  for(int i = 0; i < PPPData::NumInitStrings; i++)
      initstr[i]->setText(gpppdata.modemInitStr(i));
  initslider->setValue(gpppdata.modemInitDelay());
  linitslider->setNum(gpppdata.modemInitDelay());
  initresp->setText(gpppdata.modemInitResp());

  durationslider->setValue(gpppdata.modemToneDuration());
  ldurationslider->setNum(gpppdata.modemToneDuration());

  nodetectdialtone->setText(gpppdata.modemNoDialToneDetectionStr());
  dialstr->setText(gpppdata.modemDialStr());
  dlpresp->setText(gpppdata.modemDLPResp());

  connectresp->setText(gpppdata.modemConnectResp());
  busyresp->setText(gpppdata.modemBusyResp());
  nocarrierresp->setText(gpppdata.modemNoCarrierResp());
  nodialtoneresp->setText(gpppdata.modemNoDialtoneResp());

  escapestr->setText(gpppdata.modemEscapeStr());
  escaperesp->setText(gpppdata.modemEscapeResp());

  hangupstr->setText(gpppdata.modemHangupStr());
  hangupresp->setText(gpppdata.modemHangupResp());

  answerstr->setText(gpppdata.modemAnswerStr());
  ringresp->setText(gpppdata.modemRingResp());
  answerresp->setText(gpppdata.modemAnswerResp());

  slider->setValue(gpppdata.modemEscapeGuardTime());
  lslider->setNum(gpppdata.modemEscapeGuardTime());

  volume_off->setText(gpppdata.volumeOff());
  volume_medium->setText(gpppdata.volumeMedium());
  volume_high->setText(gpppdata.volumeHigh());
  reject();
}

#include "modemcmds.moc"
