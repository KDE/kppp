/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
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


//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QEvent>
#include <QCloseEvent>
#include "conwindow.h"
#include "docking.h"
#include "pppdata.h"
#include "pppstats.h"
#include <klocale.h>
#include <kglobal.h>

extern PPPData gpppdata;

ConWindow::ConWindow(QWidget *parent, const char *name, QWidget *mainwidget,
		     PPPStats *st)
  : QWidget(parent),
    minutes(0),
    seconds(0),
    hours(0),
    days(0),
    tl1(0),
    stats(st),
    accountingEnabled(false),
    volumeAccountingEnabled(false)
{
  setObjectName(name);
  info1 = new QLabel(i18n("Connected at:"), this);
  info2 = new QLabel("", this);

  timelabel1 = new QLabel(i18n("Time connected:"), this);
  timelabel2 = new QLabel("000:00:00", this);

  vollabel = new QLabel(i18n("Volume:"), this);
  volinfo  = new QLabel("", this);

  // now the stuff for accounting
  session_bill_l = new QLabel(i18n("Session bill:"), this);
  session_bill = new QLabel("", this);
  total_bill_l = new QLabel(i18n("Total bill:"), this);
  total_bill = new QLabel("", this);

  setWindowTitle("kppp");

  cancelbutton = new QPushButton(this);
  cancelbutton->setText(i18n("&Disconnect"));
  connect(cancelbutton, SIGNAL(clicked()), mainwidget, SLOT(disconnect()));

  statsbutton = new QPushButton(this);
  statsbutton->setText(i18n("De&tails"));
  statsbutton->setFocus();
  connect(statsbutton, SIGNAL(clicked()), mainwidget, SLOT(showStats()));

  clocktimer = new QTimer(this);
  connect(clocktimer, SIGNAL(timeout()), SLOT(timeclick()));

  // read window position from config file
  int p_x, p_y;
  gpppdata.winPosConWin(p_x, p_y);
  setGeometry(p_x, p_y, 320, 110);
}

ConWindow::~ConWindow() {
  stopClock();
}

// save window position when window was closed
bool ConWindow::event(QEvent *e) {
  if (e->type() == QEvent::Hide)
  {
    gpppdata.setWinPosConWin(x(), y());
    return true;
  }
  else
    return QWidget::event(e);
}

QString ConWindow::prettyPrintVolume(unsigned int n) {
  int idx = 0;
  const QString quant[] = {i18n("Byte"), i18n("KB"),
		   i18n("MB"), i18n("GB"), QString()};

  float n1 = n;
  while(n >= 1024 && !quant[idx].isNull()) {
    idx++;
    n /= 1024;
  }

  int i = idx;
  while(i--)
    n1 = n1 / 1024.0;

  QString s = KGlobal::locale()->formatNumber( n1, idx==0 ? 0 : 1 );
  s += ' ' + quant[idx];
  return s;
}

void ConWindow::accounting(bool on) {
  // cache accounting settings
  accountingEnabled = on;
  volumeAccountingEnabled = gpppdata.VolAcctEnabled();

  // delete old layout
  if(tl1 != 0)
    delete tl1;

  // add layout now
  tl1 = new QVBoxLayout(this);
  tl1->setSpacing(10);
  tl1->setMargin(10);
  tl1->addSpacing(5);
  QHBoxLayout *tl = new QHBoxLayout;
  tl1->addLayout(tl);
  tl->addSpacing(20);
  QGridLayout *l1;

  int vol_lines = 0;
  if(gpppdata.VolAcctEnabled())
    vol_lines = 1;

  l1 = new QGridLayout();
  l1->setMargin( 5 );
  tl->addLayout(l1);
  l1->setColumnStretch(0, 0);
  l1->setColumnStretch(1, 1);

  info2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  timelabel2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  session_bill->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  total_bill->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  volinfo->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  // make sure that there's enough space for the bills
  QString s1 = session_bill->text();
  QString s2 = total_bill->text();
  QString s3 = volinfo->text();

  session_bill->setText("888888.88 XXX");
  total_bill->setText("888888.88 XXX");
  volinfo->setText("8888.8 MB");
  session_bill->setFixedSize(session_bill->sizeHint());
  total_bill->setFixedSize(total_bill->sizeHint());
  volinfo->setFixedSize(volinfo->sizeHint());
  session_bill->setText(s1);
  total_bill->setText(s2);
  volinfo->setText(s3);

  l1->addWidget(info1, 0, 0);
  l1->addWidget(info2, 0, 1);
  l1->addWidget(timelabel1, 1, 0);
  l1->addWidget(timelabel2, 1, 1);
  if(accountingEnabled) {
    session_bill_l->show();
    session_bill->show();
    total_bill_l->show();
    total_bill->show();
    l1->addWidget(session_bill_l, 2, 0);
    l1->addWidget(session_bill, 2, 1);
    l1->addWidget(total_bill_l, 3, 0);
    l1->addWidget(total_bill, 3, 1);

    if(volumeAccountingEnabled) {
      vollabel->show();
      volinfo->show();
      l1->addWidget(vollabel, 4, 0);
      l1->addWidget(volinfo, 4, 1);
    } else {
      vollabel->hide();
      volinfo->hide();
    }

  } else {
    session_bill_l->hide();
    session_bill->hide();
    total_bill_l->hide();
    total_bill->hide();

    if(volumeAccountingEnabled) {
      vollabel->show();
      volinfo->show();
      l1->addWidget(vollabel, 2, 0);
      l1->addWidget(volinfo, 2, 1);
    } else {
      vollabel->hide();
      volinfo->hide();
    }
  }

  tl->addSpacing(10);
  QVBoxLayout *l2 = new QVBoxLayout();
  l2->setSpacing( 5 );
  tl->addLayout(l2);
  l2->addStretch(1);
  l2->addWidget(statsbutton);
  l2->addWidget(cancelbutton);

  l2->addStretch(1);

  tl1->addSpacing(5);

  setFixedSize(sizeHint());
/*
  do not overwrite position read from config
  // If this gets re-enabled, fix it for Xinerama before committing to CVS.
  setGeometry((QApplication::desktop()->width() - width()) / 2,
	      (QApplication::desktop()->height() - height())/2,
	      width(),
	      height());
*/
}


void ConWindow::dock() {
  DockWidget::dock_widget->show();
  hide();
}


void ConWindow::startClock() {
  minutes = 0;
  seconds = 0;
  hours = 0;
  QString title ;

  title = gpppdata.accname();

  if(gpppdata.get_show_clock_on_caption()){
    title += " 00:00" ;
  }
  setWindowTitle(title);

  timelabel2->setText("00:00:00");
  clocktimer->start(1000);
}


void ConWindow::setConnectionSpeed(const QString &speed) {
  info2->setText(speed);
}


void ConWindow::stopClock() {
  clocktimer->stop();
}


void ConWindow::timeclick() {
  QString tooltip = i18n("Connection: %1\n"
			 "Connected at: %2\n"
			 "Time connected: %3",
		     gpppdata.accname(), info2->text(),
		     time_string2);

  if(accountingEnabled)
      tooltip += i18n("\nSession Bill: %1\nTotal Bill: %2",
		  session_bill->text(), total_bill->text());
  // volume accounting
  if(volumeAccountingEnabled) {

    volinfo->setEnabled(true);
    int bytes = gpppdata.totalBytes();
    volinfo->setText(prettyPrintVolume(bytes));
  }

  seconds++;

  if(seconds >= 60 ) {
    minutes ++;
    seconds = 0;
  }

  if (minutes >= 60){
    minutes = 0;
    hours ++;
  }

  if( hours >= 24){
    days ++;
    hours = 0;
  }

  time_string.sprintf("%02d:%02d",hours,minutes);
  time_string2 = "";
  if (days)
      time_string2.sprintf("%d d %02d:%02d:%02d",
			   days,hours,minutes,seconds);

  else
    time_string2.sprintf("%02d:%02d:%02d",hours,minutes,seconds);

  caption_string = gpppdata.accname();
  caption_string += ' ';
  caption_string += time_string;


  timelabel2->setText(time_string2);

  if(gpppdata.get_show_clock_on_caption() && (seconds == 1)){
    // we update the Caption only once per minute not every second
    // otherwise I get a flickering icon
    setWindowTitle(caption_string);
  }

  DockWidget::dock_widget->setToolTip( tooltip);
}


void ConWindow::closeEvent( QCloseEvent *e ){
  // we don't want to lose the
  // conwindow since this is our last connection kppp.
  // if we lost it we could only kill the program by hand to get on with life.
  e->ignore();

  if(gpppdata.get_dock_into_panel())
    dock();
}


void ConWindow::slotAccounting(QString total, QString session) {
  total_bill->setText(total);
  session_bill->setText(session);
}

#include "conwindow.moc"
