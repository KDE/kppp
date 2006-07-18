/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 *
 * Copyright (C) 1997 Bernd Johannes Wuebben
 *                    wuebben@math.cornell.edu
 *
 * This file contributed by: Markus Wuebben, mwuebben@fiwi02.wiwi.uni-tuebingen.de
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
 */

#include <unistd.h>
#include <qregexp.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>
#include <kwin.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kpushbutton.h>
#include "modeminfo.h"
#include "modem.h"
#include <klocale.h>
#include <kiconloader.h>

ModemTransfer::ModemTransfer(QWidget *parent, const char *name)
  : QDialog(parent, name,TRUE, Qt::WStyle_Customize|Qt::WStyle_NormalBorder)
{
  setWindowTitle(i18n("ATI Query"));
  KWin::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));

  QVBoxLayout *tl = new QVBoxLayout(this);
  tl->setSpacing(10);
  tl->setMargin(10);

  progressBar = new KProgressBar(this);
  progressBar->setMaximum(8);

  statusBar = new QLabel( this );
  statusBar->setObjectName( "sBar" );
  statusBar->setFrameStyle(QFrame::Panel|QFrame::Sunken);
  statusBar->setAlignment(Qt::AlignCenter);

  // This is a rather complicated case. Since we do not know which
  // message is the widest in the national language, we'd to
  // search all these messages. This is a little overkill, so I take
  // the longest english message, translate it and give it additional
  // 20 percent space. Hope this is enough.
  statusBar->setText(i18n("Unable to create modem lock file."));
  statusBar->setFixedWidth((statusBar->sizeHint().width() * 12) / 10);
  statusBar->setFixedHeight(statusBar->sizeHint().height() + 4);

  // set original text
  statusBar->setText(i18n("Looking for modem..."));
  progressBar->setFixedHeight(statusBar->minimumSize().height());
  tl->addWidget(progressBar);
  tl->addWidget(statusBar);

  cancel = new KPushButton(KStdGuiItem::cancel(), this);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1);
  l1->addStretch(1);
  l1->addWidget(cancel);

  setFixedSize(sizeHint());

  step = 0;

  ////////////////////////////////////////////////

  timeout_timer = new QTimer(this);
  connect(timeout_timer, SIGNAL(timeout()), SLOT(time_out_slot()));

  scripttimer = new QTimer(this);
  connect(scripttimer, SIGNAL(timeout()), SLOT(do_script()));

  timeout_timer->start(15000,TRUE); // 15 secs single shot
  QTimer::singleShot(500, this, SLOT(init()));

}


void ModemTransfer::ati_done() {
  scripttimer->stop();
  timeout_timer->stop();
  Modem::modem->closetty();
  Modem::modem->unlockdevice();
  hide();

  // open the result window
  ModemInfo *mi = new ModemInfo(this);
  for(int i = 0; i < NUM_OF_ATI; i++)
    mi->setAtiString(i, ati_query_strings[i]);
  mi->exec();
  delete mi;

  accept();
}


void ModemTransfer::time_out_slot() {
  timeout_timer->stop();
  scripttimer->stop();

  KMessageBox::error(this, i18n("Modem query timed out."));
  reject();
}


void ModemTransfer::init() {

  kapp->processEvents();

  int lock = Modem::modem->lockdevice();
  if (lock == 1) {

    statusBar->setText(i18n("Modem device is locked."));
    return;
  }

  if (lock == -1) {

    statusBar->setText(i18n("Unable to create modem lock file."));
    return;
  }


  if(Modem::modem->opentty()) {
    if(Modem::modem->hangup()) {
      usleep(100000);  // wait 0.1 secs
      Modem::modem->writeLine("ATE0Q1V1"); // E0 don't echo the commands I send ...

      statusBar->setText(i18n("Modem Ready"));
      kapp->processEvents();
      usleep(100000);  // wait 0.1 secs
      kapp->processEvents();
      scripttimer->start(1000);	 	// this one does the ati query

      // clear modem buffer
      Modem::modem->flush();

      Modem::modem->notify(this, SLOT(readChar(unsigned char)));
      return;
    }
  }

  // opentty() or hangup() failed
  statusBar->setText(Modem::modem->modemMessage());
  step = 99; // wait until cancel is pressed
  Modem::modem->unlockdevice();
}


void ModemTransfer::do_script() {
  QString msg;
  QString query;

  switch(step) {
  case 0:
    readtty();
    statusBar->setText("ATI...");
    progressBar->advance(1);
    Modem::modem->writeLine("ATI\n");
    break;

  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    readtty();
    msg.sprintf("ATI %d ...", step);
    query.sprintf("ATI%d\n", step);
    statusBar->setText(msg);
    progressBar->advance(1);
    Modem::modem->writeLine(query.toLocal8Bit());
    break;

  default:
    readtty();
    ati_done();
  }
  step++;
}

void ModemTransfer::readChar(unsigned char c) {
  if(readbuffer.length() < 255)
    readbuffer += c;
}

void ModemTransfer::readtty() {

  if (step == 0)
    return;

  readbuffer.replace(QRegExp("[\n\r]")," ");         // remove stray \n and \r
  readbuffer = readbuffer.trimmed(); // strip of leading or trailing white
                                                 // space

  if(step <= NUM_OF_ATI)
    ati_query_strings[step-1] = readbuffer;

  readbuffer = "";
}


void ModemTransfer::cancelbutton() {
  scripttimer->stop();
  Modem::modem->stop();
  timeout_timer->stop();

  statusBar->setText(i18n("One moment please..."));
  kapp->processEvents();

  Modem::modem->hangup();

  Modem::modem->closetty();
  Modem::modem->unlockdevice();
  reject();
}


void ModemTransfer::closeEvent( QCloseEvent *e ) {
  cancelbutton();
  e->accept();
}


ModemInfo::ModemInfo(QWidget *parent, const char* name)
  : QDialog(parent, name, TRUE, Qt::WStyle_Customize|Qt::WStyle_NormalBorder)
{
  QString label_text;

  setWindowTitle(i18n("Modem Query Results"));
  KWin::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));

  QVBoxLayout *tl = new QVBoxLayout(this);
  tl->setSpacing(10);
  tl->setMargin(10);

  QGridLayout *l1 = new QGridLayout();
  l1->setMargin( 5 );
  tl->addLayout(l1, 1);
  for(int  i = 0 ; i < NUM_OF_ATI ; i++) {

    label_text = "";
    if ( i == 0)
      label_text.sprintf("ATI :");
    else
      label_text.sprintf("ATI %d:", i );

    ati_label[i] = new QLabel(label_text, this);
    l1->addWidget(ati_label[i], i, 0);

    ati_label_result[i] =  new QLineEdit(this);
    ati_label_result[i]->setMinimumWidth(fontMetrics().width('H') * 24);
    l1->addWidget(ati_label_result[i], i, 1);
  }
  //tl->addSpacing(1);

  QHBoxLayout *l2 = new QHBoxLayout;
  QPushButton *ok = new KPushButton(KStdGuiItem::close(), this);
  ok->setDefault(TRUE);
  ok->setFocus();

  tl->addLayout(l2);
  l2->addStretch(1);

  connect(ok, SIGNAL(clicked()), SLOT(accept()));
  l2->addWidget(ok);

  setMinimumSize(sizeHint());
}


void ModemInfo::setAtiString(int i, QString s) {
  if(i < NUM_OF_ATI)
    ati_label_result[i]->setText(s);
}

#include "modeminfo.moc"
