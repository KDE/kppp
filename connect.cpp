/*
 *            kPPP: A pppd front end for the KDE project
 *
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *            Copyright (C) 1998-2001 Harri Porten <porten@kde.org>
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
 * Software Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <config.h>

#include <qlayout.h>
#include <qregexp.h>
//Added by qt3to4:
#include <QLabel>
#include <QTimerEvent>
#include <QVBoxLayout>
#include <Q3Frame>
#include <QHBoxLayout>
#include <Q3CString>
#include <QCloseEvent>

#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <assert.h>

#ifdef _XPG4_2
#define __xnet_connect	connect
#endif

#include <errno.h>

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef __linux__
#include "runtests.h"
#endif

#include "auth.h"
#include "connect.h"
#include "docking.h"
#include "main.h"
#include "modem.h"
#include "kpppconfig.h"
#include "pppdata.h"
#include "pppstats.h"
#include "requester.h"
#include "utils.h"

extern KPPPWidget *p_kppp;

QString old_hostname;
bool modified_hostname;


ConnectWidget::ConnectWidget(QWidget *parent, const char *name, PPPStats *st)
  : QWidget(parent, name),
    // initialize some important variables
    myreadbuffer(""),
    main_timer_ID(0),
    vmain(0),
    substate(-1),
    scriptindex(0),
    loopnest(0),
    loopend(false),
    semaphore(false),
    expecting(false),
    readbuffer(""),
    scanvar(""),
    scanning(false),
    pausing(false),
    termwindow(0),
    dialnumber(0),
    stats(st)
{
  modified_hostname = false;

  QVBoxLayout *tl = new QVBoxLayout(this, 8, 10);
  QString tit = i18n("Connecting to: ");
  setCaption(tit);

  QHBoxLayout *l0 = new QHBoxLayout(10);
  tl->addLayout(l0);
  l0->addSpacing(10);
  messg = new QLabel(this, "messg");
  messg->setFrameStyle(Q3Frame::Panel|Q3Frame::Sunken);
  messg->setAlignment(Qt::AlignCenter);
  messg->setText(i18n("Unable to create modem lock file."));
  messg->setMinimumHeight(messg->sizeHint().height() + 5);
  int messw = (messg->sizeHint().width() * 12) / 10;
  messw = QMAX(messw,280);
  messg->setMinimumWidth(messw);
  messg->setText(i18n("Looking for modem..."));
  l0->addWidget(messg);
  l0->addSpacing(10);

  QHBoxLayout *l1 = new QHBoxLayout(10);
  tl->addLayout(l1);
  l1->addStretch(1);

  debug = new QPushButton(i18n("&Log"), this);
  debug->setToggleButton(true);
  connect(debug, SIGNAL(clicked()), SIGNAL(toggleDebugWindow()));

  cancel = new KPushButton(KStdGuiItem::cancel(), this);
  cancel->setFocus();
  connect(cancel, SIGNAL(clicked()), SLOT(cancelbutton()));

  int maxw = QMAX(cancel->sizeHint().width(),
		 debug->sizeHint().width());
  maxw = QMAX(maxw,65);
  debug->setFixedWidth(maxw);
  cancel->setFixedWidth(maxw);
  l1->addWidget(debug);
  l1->addWidget(cancel);
  l1->addSpacing(10);

  setFixedSize(sizeHint());

  pausetimer = new QTimer(this);
  connect(pausetimer, SIGNAL(timeout()), SLOT(pause()));

  kapp->processEvents();

  timeout_timer = new QTimer(this);
  connect(timeout_timer, SIGNAL(timeout()), SLOT(script_timed_out()));

  inittimer = new QTimer(this);
  connect(inittimer, SIGNAL(timeout()), SLOT(init()));

  if_timeout_timer = new QTimer(this);
  connect(if_timeout_timer, SIGNAL(timeout()), SLOT(if_waiting_timed_out()));

  connect(this,SIGNAL(if_waiting_signal()),this,SLOT(if_waiting_slot()));

  prompt = new PWEntry( this, "pw" );
  if_timer = new QTimer(this);
  connect(if_timer,SIGNAL(timeout()), SLOT(if_waiting_slot()));
}


ConnectWidget::~ConnectWidget() {
}


void ConnectWidget::disableButtons()
{
  debug->setEnabled(false);
  cancel->setEnabled(false);
}

void ConnectWidget::enableButtons()
{
  debug->setEnabled(true);
  cancel->setEnabled(true);
}

void ConnectWidget::preinit() {
  // this is all just to keep the GUI nice and snappy ....
  // you have to see to believe ...
  messg->setText(i18n("Looking for modem..."));
  inittimer->start(100);
}

void ConnectWidget::init() {
  gpppdata.setpppdError(0);
  inittimer->stop();
  vmain = 0;
  substate = -1;
  expecting = false;
  pausing = false;
  scriptindex = 0;
  myreadbuffer = "";
  scanning = false;
  scanvar = "";
  firstrunID = true;
  firstrunPW = true;
  stats->totalbytes = 0;
  dialnumber = 0;

  p_kppp->con_speed = "";

  p_kppp->setQuitOnDisconnect (p_kppp->quitOnDisconnect() || gpppdata.quit_on_disconnect());

  comlist = &gpppdata.scriptType();
  arglist = &gpppdata.script();

  QString tit = i18n("Connecting to: %1").arg(gpppdata.accname());
  setCaption(tit);

  kapp->processEvents();

  // signal other applications that we are about to get connected
  kapp->dcopClient()->emitDCOPSignal("KpppIface", "aboutToConnect()", QByteArray());

  // run the "before-connect" command
  if (!gpppdata.command_before_connect().isEmpty()) {
    messg->setText(i18n("Running pre-startup command..."));
    emit debugMessage(i18n("Running pre-startup command..."));

    kapp->processEvents();
    QApplication::flushX();
    pid_t id = execute_command(gpppdata.command_before_connect());
    int i, status;

    do {
      kapp->processEvents();
      i = waitpid(id, &status, WNOHANG);
      usleep(100000);
    } while (i == 0 && errno == 0);
  }

  int lock = Modem::modem->lockdevice();

  if (lock == 1) {
    messg->setText(i18n("Modem device is locked."));
    vmain = 20; // wait until cancel is pressed
    return;
  }

  if (lock == -1) {
    messg->setText(i18n("Unable to create modem lock file."));
    vmain = 20; // wait until cancel is pressed
    return;
  }

  if(Modem::modem->opentty()) {
    messg->setText(Modem::modem->modemMessage());
    kapp->processEvents();
    if(Modem::modem->hangup()) {

      kapp->processEvents();

      semaphore = false;

      Modem::modem->stop();
      Modem::modem->notify(this, SLOT(readChar(unsigned char)));

      // if we are stuck anywhere we will time out
      timeout_timer->start(gpppdata.modemTimeout()*1000);

      // this timer will run the script etc.
      main_timer_ID = startTimer(10);

      return;
    }
  }

  // initialization failed
  messg->setText(Modem::modem->modemMessage());
  vmain = 20; // wait until cancel is pressed
  Modem::modem->unlockdevice();
}


void ConnectWidget::timerEvent(QTimerEvent *) {
  if (semaphore || pausing)
    return;

  if(vmain == 0) {
#ifdef DEBUG_WO_DIALING
    vmain = 10;
    return;
#endif

    assert(PPPData::NumInitStrings > 0);
    // first init string ?
    if(substate == -1) {
      messg->setText(i18n("Initializing modem..."));
      emit debugMessage(i18n("Initializing modem..."));
      substate = 0;
    }

    QString initStr = gpppdata.modemInitStr(substate);
    if (!initStr.isEmpty()) {
	// send a carriage return and then wait a bit so that the modem will
	// let us issue commands.
	if(gpppdata.modemPreInitDelay() > 0) {
	    usleep(gpppdata.modemPreInitDelay() * 5000);
	    writeline("");
	    usleep(gpppdata.modemPreInitDelay() * 5000);
	}
	setExpect(gpppdata.modemInitResp());
	writeline(initStr);
	usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec
    }

    substate++;

    /*
     * FIXME after 3.0: Make it possible to disable ATS11 since it
     * seems to be incompatible with some ISDN adapters (e.g. DataBox
     * Speed Dragon). Even better would be to detect this when doing
     * a "Modem Query"
     */
    if (MODEM_TONEDURATION != gpppdata.modemToneDuration())
        vmain = 5;
    else
        vmain = 3;

    return;
  }

  if (vmain == 5) {
    if(!expecting) {
        QString sToneDuration = "ATS11=" + QString::number(gpppdata.modemToneDuration());
        QString msg = i18n("Setting ") + sToneDuration;
        messg->setText(msg);
	emit debugMessage(msg);
	setExpect(gpppdata.modemInitResp());
	writeline(sToneDuration);
      }
    vmain = 3;
    return;
  }

  if(vmain == 3) {
    if(!expecting) {
      // done with all init strings ?
      if(substate < PPPData::NumInitStrings) {
	vmain = 0;
	return;
      }
      substate = -1;
      // skip setting the volume if command is empty
      if(gpppdata.volumeInitString().isEmpty()) {
        vmain = 4;
        return;
      }
      messg->setText(i18n("Setting speaker volume..."));
      emit debugMessage(i18n("Setting speaker volume..."));

      setExpect(gpppdata.modemInitResp());
      QString vol("AT");
      vol += gpppdata.volumeInitString();
      writeline(vol);
      usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec
      vmain = 4;
      return;
    }
  }

  if(vmain == 4) {
    if(!expecting) {
      if(!gpppdata.waitForDialTone() || gpppdata.waitCallback()) {
	QString msg = i18n("Turning off dial tone waiting...");
	messg->setText(msg);
	emit debugMessage(msg);
	setExpect(gpppdata.modemInitResp());
	writeline(gpppdata.modemNoDialToneDetectionStr());
      }
      vmain = 1;
      return;
    }
  }

  // dial the number and wait to connect
  if(vmain == 1) {
    if(!expecting) {

      timeout_timer->stop();
      timeout_timer->start(gpppdata.modemTimeout()*1000);

      if(gpppdata.waitCallback()) {
        QString msg = i18n("Waiting for callback...");
        messg->setText(msg);
        emit debugMessage(msg);
        setExpect(gpppdata.modemRingResp());
        vmain = 102;
        return;
      }

      QStringList &plist = gpppdata.phonenumbers();
      QString bmarg= gpppdata.dialPrefix();
      bmarg += plist.at(dialnumber);
      QString bm = i18n("Dialing %1").arg(bmarg);
      messg->setText(bm);
      emit debugMessage(bm);

      QString pn = gpppdata.modemDialStr();
      pn += gpppdata.dialPrefix();
      pn += plist.at(dialnumber);
      if(++dialnumber >= plist.count())
        dialnumber = 0;
      writeline(pn);

      setExpect(gpppdata.modemConnectResp());
      vmain = 100;
      return;
    }
  }

  // wait for connect, but redial if BUSY or wait for user cancel
  // if NO CARRIER or NO DIALTONE
  if(vmain == 100) {
    if(!expecting) {
      myreadbuffer = gpppdata.modemConnectResp();
      setExpect("\n");
      vmain = 101;
      return;
    }

    if(readbuffer.contains(gpppdata.modemBusyResp())) {
      timeout_timer->stop();
      timeout_timer->start(gpppdata.modemTimeout()*1000);

      messg->setText(i18n("Line busy. Hanging up..."));
      emit debugPutChar('\n');
      Modem::modem->hangup();

      if(gpppdata.busyWait() > 0) {
	QString bm = i18n("Line busy. Waiting: %1 seconds").arg(gpppdata.busyWait());
	messg->setText(bm);
	emit debugMessage(bm);

	pausing = true;

	pausetimer->start(gpppdata.busyWait()*1000, true);
	timeout_timer->stop();
      }

      Modem::modem->setDataMode(false);
      vmain = 0;
      substate = -1;
      gpppdata.setWaitCallback(false);
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoDialtoneResp())) {
      timeout_timer->stop();

      messg->setText(i18n("No Dial Tone"));
      vmain = 20;
      Modem::modem->unlockdevice();
      gpppdata.setWaitCallback(false);
      return;
    }

    if(readbuffer.contains(gpppdata.modemNoCarrierResp())) {
      if (gpppdata.get_redial_on_nocarrier()) {
        timeout_timer->stop();
        timeout_timer->start(gpppdata.modemTimeout()*1000);

        if(gpppdata.busyWait() > 0) {
	  QString bm = i18n("No carrier. Waiting: %1 seconds").arg(gpppdata.busyWait());
	  messg->setText(bm);
	  emit debugMessage(bm);

	  pausing = true;

	  pausetimer->start(gpppdata.busyWait()*1000, true);
	  timeout_timer->stop();
        }

        Modem::modem->setDataMode(false);
        vmain = 0;
        substate = -1;
        return;
      } else {
        timeout_timer->stop();

        messg->setText(i18n("No Carrier"));
        vmain = 20;
        Modem::modem->unlockdevice();
        gpppdata.setWaitCallback(false);
      }
      return;
    }

    if(readbuffer.contains(gpppdata.modemDLPResp())) {
      timeout_timer->stop();

      messg->setText(i18n("Digital Line Protection Detected."));
      vmain = 20;
      Modem::modem->unlockdevice();
      KMessageBox::error(this,
                         i18n("A Digital Line Protection (DLP) error response "
			      "has been detected.\n"
                              "Please disconnect the phone line.\n\n"
                              "Do NOT connect this modem to a digital phone "
			      "line or the modem could get permanently "
			      "damaged"));
      gpppdata.setWaitCallback(false);
      return;
    }


  }

  // wait for newline after CONNECT response (so we get the speed)
  if(vmain == 101) {
    if(!expecting) {
      Modem::modem->setDataMode(true); // modem will no longer respond to AT commands

      emit startAccounting();
      p_kppp->con_win->startClock();

      vmain = 2;
      scriptTimeout=gpppdata.modemTimeout()*1000;
      return;
    }
  }

  // send answer on callback phase
  if(vmain == 102) {
    if(!expecting) {
      writeline(gpppdata.modemAnswerStr());
      setExpect(gpppdata.modemConnectResp());
      vmain = 100;
      return;
    }
  }

  // execute the script
  if(vmain == 2) {
    if(!expecting && !pausing && !scanning) {

      timeout_timer->stop();
      timeout_timer->start(scriptTimeout);

      if(scriptindex < comlist->count()) {
        scriptCommand = (comlist->at(scriptindex));
        scriptArgument = (arglist->at(scriptindex));
      } else {
        kdDebug(5002) << "End of script" << endl;
	vmain = 10;
        return;
      }

      if (scriptCommand == "Scan") {
	QString bm = i18n("Scanning %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

        setScan(scriptArgument);
	scriptindex++;
        return;
      }

      if (scriptCommand == "Save") {
	QString bm = i18n("Saving %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	if (scriptArgument.lower() == "password") {
	  gpppdata.setPassword(scanvar);
	  p_kppp->setPW_Edit(scanvar);
	  if(gpppdata.storePassword())
	    gpppdata.setStoredPassword(scanvar);
	  firstrunPW = true;
	}

	scriptindex++;
        return;
      }


      if (scriptCommand == "Send" || scriptCommand == "SendNoEcho") {
	QString bm = i18n("Sending %1");

	// replace %USERNAME% and %PASSWORD%
	QString arg = scriptArgument;
	QRegExp re1("%USERNAME%");
	QRegExp re2("%PASSWORD%");
	arg = arg.replace(re1, gpppdata.storedUsername());
	arg = arg.replace(re2, gpppdata.storedPassword());

	if (scriptCommand == "Send")
	  bm = bm.arg(scriptArgument);
	else {
	  for(int i = 0; i < scriptArgument.length(); i++)
	    bm = bm.arg("*");
	}

	messg->setText(bm);
	emit debugMessage(bm);

	writeline(scriptArgument);
	scriptindex++;
        return;
      }

      if (scriptCommand == "Expect") {
        QString bm = i18n("Expecting %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	// The incrementing of the scriptindex MUST be before the
	// call to setExpect otherwise the expect will miss a string that is
	// already in the buffer.
	scriptindex++;
	setExpect(scriptArgument);
        return;
      }


      if (scriptCommand == "Pause") {
	QString bm = i18n("Pause %1 seconds").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	pausing = true;

	pausetimer->start(scriptArgument.toInt()*1000, true);
	timeout_timer->stop();

	scriptindex++;
	return;
      }

      if (scriptCommand == "Timeout") {

	timeout_timer->stop();

	QString bm = i18n("Timeout %1 seconds").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	scriptTimeout=scriptArgument.toInt()*1000;
        timeout_timer->start(scriptTimeout);

	scriptindex++;
	return;
      }

      if (scriptCommand == "Hangup") {
	messg->setText(i18n("Hangup"));
	emit debugMessage(i18n("Hangup"));

	writeline(gpppdata.modemHangupStr());
	setExpect(gpppdata.modemHangupResp());

	scriptindex++;
	return;
      }

      if (scriptCommand == "Answer") {

	timeout_timer->stop();

	messg->setText(i18n("Answer"));
	emit debugMessage(i18n("Answer"));

	setExpect(gpppdata.modemRingResp());
	vmain = 150;
	return;
      }

      if (scriptCommand == "ID") {
	QString bm = i18n("ID %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	QString idstring = gpppdata.storedUsername();

	if(!idstring.isEmpty() && firstrunID) {
	  // the user entered an Id on the main kppp dialog
	  writeline(idstring);
	  firstrunID = false;
	  scriptindex++;
	}
	else {
	  // the user didn't enter and Id on the main kppp dialog
	  // let's query for an ID
	     /* if not around yet, then post window... */
	     if (prompt->Consumed()) {
	       if (!(prompt->isVisible())) {
		 prompt->setPrompt(scriptArgument);
		 prompt->setEchoModeNormal();
		 prompt->show();
	       }
	     } else {
	       /* if prompt withdrawn ... then, */
	       if(!(prompt->isVisible())) {
		 writeline(prompt->text());
		 prompt->setConsumed();
		 scriptindex++;
		 return;
	       }
	       /* replace timeout value */
	     }
	}
      }

      if (scriptCommand == "Password") {
	QString bm = i18n("Password %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	QString pwstring = gpppdata.password();

	if(!pwstring.isEmpty() && firstrunPW) {
	  // the user entered a password on the main kppp dialog
	  writeline(pwstring);
	  firstrunPW = false;
	  scriptindex++;
	}
	else {
	  // the user didn't enter a password on the main kppp dialog
	  // let's query for a password
	     /* if not around yet, then post window... */
	     if (prompt->Consumed()) {
	       if (!(prompt->isVisible())) {
		 prompt->setPrompt(scriptArgument);
		 prompt->setEchoModePassword();
		 prompt->show();
	       }
	     } else {
	       /* if prompt withdrawn ... then, */
	       if(!(prompt->isVisible())) {
		 p_kppp->setPW_Edit(prompt->text());
		 writeline(prompt->text());
		 prompt->setConsumed();
		 scriptindex++;
		 return;
	       }
	       /* replace timeout value */
	     }
	}
      }

      if (scriptCommand == "Prompt") {
	QString bm = i18n("Prompting %1");

        // if the scriptindex (aka the prompt text) includes a ## marker
        // this marker should get substituted with the contents of our stored
        // variable (from the subsequent scan).

	QString ts = scriptArgument;
	int vstart = ts.find( "##" );
	if( vstart != -1 ) {
		ts.remove( vstart, 2 );
		ts.insert( vstart, scanvar );
	}

	bm = bm.arg(ts);
	messg->setText(bm);
	emit debugMessage(bm);

	/* if not around yet, then post window... */
	if (prompt->Consumed()) {
	   if (!(prompt->isVisible())) {
		prompt->setPrompt( ts );
		prompt->setEchoModeNormal();
	        prompt->show();
	   }
	} else {
	    /* if prompt withdrawn ... then, */
	    if (!(prompt->isVisible())) {
	    	writeline(prompt->text());
	        prompt->setConsumed();
	        scriptindex++;
        	return;
	    }
	    /* replace timeout value */
	}
      }

      if (scriptCommand == "PWPrompt") {
	QString bm = i18n("PW Prompt %1").arg(scriptArgument);
	messg->setText(bm);
	emit debugMessage(bm);

	/* if not around yet, then post window... */
	if (prompt->Consumed()) {
	   if (!(prompt->isVisible())) {
		prompt->setPrompt(scriptArgument);
		prompt->setEchoModePassword();
	        prompt->show();
	   }
	} else {
	    /* if prompt withdrawn ... then, */
	    if (!(prompt->isVisible())) {
	    	writeline(prompt->text());
	        prompt->setConsumed();
	        scriptindex++;
        	return;
	    }
	    /* replace timeout value */
	}
      }

      if (scriptCommand == "LoopStart") {

        QString bm = i18n("Loop Start %1").arg(scriptArgument);

	// The incrementing of the scriptindex MUST be before the
	// call to setExpect otherwise the expect will miss a string that is
	// already in the buffer.
	scriptindex++;

	if ( loopnest > (MAXLOOPNEST-2) ) {
		bm += i18n("ERROR: Nested too deep, ignored.");
		vmain=20;
		cancelbutton();
	        KMessageBox::error(0, i18n("Loops nested too deeply."));
	} else {
        	setExpect(scriptArgument);
		loopstartindex[loopnest] = scriptindex;
		loopstr[loopnest] = scriptArgument;
		loopend = false;
		loopnest++;
	}
	messg->setText(bm);
	emit debugMessage(bm);

      }

      if (scriptCommand == "LoopEnd") {
        QString bm = i18n("Loop End %1").arg(scriptArgument);
	if ( loopnest <= 0 ) {
		bm = i18n("LoopEnd without matching Start. Line: %1").arg(bm);
		vmain=20;
		cancelbutton();
	        KMessageBox::error(0, bm);
		return;
	} else {
	        // NB! The incrementing of the scriptindex MUST be before the
	        // call to setExpect otherwise the expect will miss a string
	        // that is already in the buffer.
        	scriptindex++;
        	setExpect(scriptArgument);
		loopnest--;
		loopend = true;
	}
	messg->setText(bm);
	emit debugMessage(bm);

      }
    }
  }

  // this is a subroutine for the "Answer" script option

  if(vmain == 150) {
    if(!expecting) {
      writeline(gpppdata.modemAnswerStr());
      setExpect(gpppdata.modemAnswerResp());

      vmain = 2;
      scriptindex++;
      return;
    }
  }

  if(vmain == 30) {
    if (termwindow->isVisible())
      return;
    if (termwindow->pressedContinue())
      vmain = 10;
    else
      cancelbutton();
  }

  if(vmain == 10) {
    if(!expecting) {

      int result;

      timeout_timer->stop();
      if_timeout_timer->stop(); // better be sure.

      // stop reading of data
      Modem::modem->stop();

      if(gpppdata.authMethod() == AUTH_TERMINAL) {
	if (termwindow) {
	  delete termwindow;
	  termwindow = 0L;
	  show();
	} else {
	  termwindow = new LoginTerm(0L, 0L);
	  hide();
	  termwindow->show();
	  vmain = 30;
	  return;
	}
      }

      // Close the tty. This prevents the QTimer::singleShot() in
      // Modem::readtty() from re-enabling the socket notifier.
      // The port is still held open by the helper process.
      Modem::modem->closetty();

      killTimer( main_timer_ID );

      if_timeout_timer->start(gpppdata.pppdTimeout()*1000);
      kdDebug(5002) << "started if timeout timer with " << gpppdata.pppdTimeout()*1000 << endl;

      // find out PPP interface and notify the stats module
      stats->setUnit(pppInterfaceNumber());

      kapp->flushX();
      semaphore = true;
      result = execppp();

      emit debugMessage(i18n("Starting pppd..."));
      kdDebug(5002) << "execppp() returned with return-code " << result << endl;

      if(result) {
        if(!gpppdata.autoDNS())
          adddns();

	// O.K we are done here, let's change over to the if_waiting loop
	// where we wait for the ppp if (interface) to come up.

	emit if_waiting_signal();
      } else {

	// starting pppd wasn't successful. Error messages were
	// handled by execppp();
	if_timeout_timer->stop();

	hide();
	messg->setText("");
	p_kppp->quit_b->setFocus();
	p_kppp->show();
	kapp->processEvents();
	Modem::modem->hangup();
	emit stopAccounting();
	p_kppp->con_win->stopClock();
	Modem::modem->closetty();
        Modem::modem->unlockdevice();

      }

      return;
    }
  }

  // this is a "wait until cancel" entry

  if(vmain == 20) {
    gpppdata.setWaitCallback(false);
  }
}


void ConnectWidget::set_con_speed_string() {
  // Here we are trying to determine the speed at which we are connected.
  // Usually the modem responds after connect with something like
  // CONNECT 115200, so all we need to do is find the number after CONNECT
  // or whatever the modemConnectResp() is.
  p_kppp->con_speed = Modem::modem->parseModemSpeed(myreadbuffer);
}



void ConnectWidget::readChar(unsigned char c) {
  if(semaphore)
    return;

    readbuffer += c;
    myreadbuffer += c;

    // While in scanning mode store each char to the scan buffer
    // for use in the prompt command
    if( scanning )
       scanbuffer += c;

    // add to debug window
    emit debugPutChar(c);

    checkBuffers();
}


void ConnectWidget::checkBuffers() {
  // Let's check if we are finished with scanning:
  // The scanstring have to be in the buffer and the latest character
  // was a carriage return or an linefeed (depending on modem setup)
  if( scanning && scanbuffer.contains(scanstr) &&
      ( scanbuffer.right(1) == "\n" || scanbuffer.right(1) == "\r") ) {
      scanning = false;

      int vstart = scanbuffer.find( scanstr ) + scanstr.length();
      scanvar = scanbuffer.mid( vstart, scanbuffer.length() - vstart);
      scanvar = scanvar.stripWhiteSpace();

      // Show the Variabel content in the debug window
      QString sv = i18n("Scan Var: %1").arg(scanvar);
      emit debugMessage(sv);
  }

  if(expecting) {
    if(readbuffer.contains(expectstr)) {
      expecting = false;
      // keep everything after the expected string
      readbuffer.remove(0, readbuffer.find(expectstr) + expectstr.length());

      QString ts = i18n("Found: %1").arg(expectstr);
      emit debugMessage(ts);

      if (loopend) {
	loopend=false;
      }
    }

    if (loopend && readbuffer.contains(loopstr[loopnest])) {
      expecting = false;
      readbuffer = "";
      QString ts = i18n("Looping: %1").arg(loopstr[loopnest]);
      emit debugMessage(ts);
      scriptindex = loopstartindex[loopnest];
      loopend = false;
      loopnest++;
    }
    // notify event loop if expected string was found
    if(!expecting)
      timerEvent((QTimerEvent *) 0);
  }
}



void ConnectWidget::pause() {
  pausing = false;
  pausetimer->stop();
}


void ConnectWidget::cancelbutton() {
  gpppdata.setWaitCallback(false);
  Modem::modem->stop();
  killTimer(main_timer_ID);
  timeout_timer->stop();
  if_timer->stop();
  if_timeout_timer->stop();

  if (termwindow) {
    delete termwindow;
    termwindow = 0L;
    show();
  }

  messg->setText(i18n("One moment please..."));

  // just to be sure
  Requester::rq->removeSecret(AUTH_PAP);
  Requester::rq->removeSecret(AUTH_CHAP);
  removedns();

  kapp->processEvents();

  Requester::rq->killPPPDaemon();
  Modem::modem->hangup();

  hide();
  messg->setText("");
  p_kppp->quit_b->setFocus();
  p_kppp->show();
  emit stopAccounting();	// just to be sure
  p_kppp->con_win->stopClock();
  Modem::modem->closetty();
  Modem::modem->unlockdevice();

  //abort prompt window...
  if (prompt->isVisible()) {
  	prompt->hide();
  }
  prompt->setConsumed();

  if(p_kppp->quitOnDisconnect())
    kapp->exit(0);
}


void ConnectWidget::script_timed_out() {
  if(vmain == 20) { // we are in the 'wait for the user to cancel' state
    timeout_timer->stop();
    emit stopAccounting();
    p_kppp->con_win->stopClock();
    return;
  }

  if (prompt->isVisible())
    prompt->hide();

  prompt->setConsumed();
  messg->setText(i18n("Script timed out."));
  Modem::modem->hangup();
  emit stopAccounting();
  p_kppp->con_win->stopClock();

  vmain = 0; // let's try again.
  substate = -1;
}


void ConnectWidget::setScan(const QString &n) {
  scanning = true;
  scanstr = n;
  scanbuffer = "";

  QString ts = i18n("Scanning: %1").arg(n);
  emit debugMessage(ts);
}


void ConnectWidget::setExpect(const QString &n) {
  expecting = true;
  expectstr = n;

  QString ts = i18n("Expecting: %1").arg(n);
  ts.replace(QRegExp("\n"), "<LF>");
  emit debugMessage(ts);

  // check if the expected string is in the read buffer already.
  checkBuffers();
}


void ConnectWidget::if_waiting_timed_out() {
  if_timer->stop();
  if_timeout_timer->stop();
  kdDebug(5002) << "if_waiting_timed_out()" << endl;

  gpppdata.setpppdError(E_IF_TIMEOUT);

  // let's kill the stuck pppd
  Requester::rq->killPPPDaemon();

  emit stopAccounting();
  p_kppp->con_win->stopClock();


  // killing ppp will generate a SIGCHLD which will be caught in pppdie()
  // in main.cpp what happens next will depend on the boolean
  // reconnect_on_disconnect which is set in ConnectWidget::init();
}

void ConnectWidget::pppdDied()
{
  if_timer->stop();
  if_timeout_timer->stop();
}

void ConnectWidget::if_waiting_slot() {
  messg->setText(i18n("Logging on to network..."));

  if(!stats->ifIsUp()) {

    if(gpppdata.pppdError() != 0) {
      // we are here if pppd died immediately after starting it.
      pppdDied();
      // error message handled in main.cpp: sigPPPDDied()
      return;
    }

    if_timer->start(100, TRUE); // single shot
    return;
  }

  // O.K the ppp interface is up and running
  // give it a few time to come up completly (0.2 seconds)
  if_timeout_timer->stop();
  if_timer->stop();
  usleep(200000);

  if(gpppdata.autoDNS())
    addpeerdns();

  // Close the debugging window. If we are connected, we
  // are not really interested in debug output
  emit closeDebugWindow();
  p_kppp->statdlg->take_stats(); // start taking ppp statistics
  auto_hostname();

  // signal other applications that we are connected now
  kapp->dcopClient()->emitDCOPSignal("KpppIface", "connected()", QByteArray());

  if(!gpppdata.command_on_connect().isEmpty()) {
    messg->setText(i18n("Running startup command..."));

    // make sure that we don't get any async errors
    kapp->flushX();
    execute_command(gpppdata.command_on_connect());
    messg->setText(i18n("Done"));
  }

  // remove the authentication file
  Requester::rq->removeSecret(AUTH_PAP);
  Requester::rq->removeSecret(AUTH_CHAP);

  emit debugMessage(i18n("Done"));
  set_con_speed_string();

  p_kppp->con_win->setConnectionSpeed(p_kppp->con_speed);
  hide();
  messg->setText("");

  // prepare the con_win so as to have the right size for
  // accounting / non-accounting mode
  if(p_kppp->acct != 0)
    p_kppp->con_win->accounting(p_kppp->acct->running());
  else
    p_kppp->con_win->accounting(false);

  if (gpppdata.get_dock_into_panel()) {
    DockWidget::dock_widget->show();
    DockWidget::dock_widget->take_stats();
    hide();
  }
  else {
    p_kppp->con_win->show();

    if(gpppdata.get_iconify_on_connect()) {
      p_kppp->con_win->showMinimized();
    }
  }

  Modem::modem->closetty();
}


bool ConnectWidget::execppp() {
  QString command;

  command = "pppd";

  // as of version 2.3.6 pppd falls back to the real user rights when
  // opening a device given in a command line. To avoid permission conflicts
  // we'll simply leave this argument away. pppd will then use the default tty
  // which is the serial port we connected stdin/stdout to in opener.cpp.
  //  command += " ";
  //  command += gpppdata.modemDevice();

  command += " " + gpppdata.speed();

  command += " -detach";

  if(gpppdata.ipaddr() != "0.0.0.0" ||
     gpppdata.gateway() != "0.0.0.0") {
    if(gpppdata.ipaddr() != "0.0.0.0") {
      command += " ";
      command += gpppdata.ipaddr();
      command +=  ":";
    }
    else {
      command += " ";
      command += ":";
    }

    if(gpppdata.gateway() != "0.0.0.0")
      command += gpppdata.gateway();
  }

  if(gpppdata.subnetmask() != "0.0.0.0")
    command += " netmask " + gpppdata.subnetmask();

  // the english/i18n mix below is ugly but we want to keep working
  // after someone changed the code to use i18n'ed config values
  QString flowCtrl = gpppdata.flowcontrol();
  if(flowCtrl != "None" && flowCtrl != i18n("None")) {
    if(flowCtrl == "CRTSCTS" || flowCtrl == "Hardware [CRTSCTS]" ||
       flowCtrl == i18n("Hardware [CRTSCTS]"))
      command += " crtscts";
    else
      command += " xonxoff";
  }

  if(gpppdata.defaultroute())
    command += " defaultroute";

  if(gpppdata.autoDNS())
    command += " usepeerdns";

  QStringList &arglist = gpppdata.pppdArgument();
  for ( QStringList::Iterator it = arglist.begin();
        it != arglist.end();
        ++it )
  {
    command += " " + *it;
  }

  // Callback settings
  if(gpppdata.callbackType() && !gpppdata.waitCallback()) {
    if(!gpppdata.pppdVersionMin(2, 4, 2)) {
      command += " +callback";
      if(gpppdata.callbackType() == CBTYPE_USER)
        command += " callback " + gpppdata.callbackPhone();
    } else {
      command += " callback ";
      command += gpppdata.callbackType() == CBTYPE_ADMIN ?
                 QString("0") : gpppdata.callbackPhone();
    }
  } else
    gpppdata.setWaitCallback(false);

  // PAP settings
  if(gpppdata.authMethod() == AUTH_PAP) {
    command += " -chap user ";
    command = command + "\"" + gpppdata.storedUsername() + "\"";
  }

  // CHAP settings
  if(gpppdata.authMethod() == AUTH_CHAP) {
    command += " -pap user ";
    command = command + "\"" + gpppdata.storedUsername() + "\"";
  }

  // PAP/CHAP settings
  if(gpppdata.authMethod() == AUTH_PAPCHAP) {
    command += " user ";
    command = command + "\"" + gpppdata.storedUsername() + "\"";
  }

  // check for debug
  if(gpppdata.getPPPDebug())
    command += " debug";

  if (command.length() > MAX_CMDLEN) {
    KMessageBox::error(this, i18n(
			      "pppd command + command-line arguments exceed "
			      "2024 characters in length."
			      ));

    return false; // nonsensically long command which would bust my buffer buf.
  }

  kapp->flushX();

  return Requester::rq->execPPPDaemon(command);
}


void ConnectWidget::closeEvent( QCloseEvent *e ) {
  e->ignore();
  emit cancelbutton();
}


void ConnectWidget::setMsg(const QString &msg) {
  messg->setText(msg);
}

void ConnectWidget::writeline(const QString &s) {
  Modem::modem->writeLine(s.local8Bit());
}

// Set the hostname and domain from DNS Server
void auto_hostname() {
  struct in_addr local_ip;
  struct hostent *hostname_entry;
  QString new_hostname;
  int    dot;
  char   tmp_str[100]; // buffer overflow safe

  gethostname(tmp_str, sizeof(tmp_str));
  tmp_str[sizeof(tmp_str)-1]=0; // panic
  old_hostname=tmp_str; // copy to QString

  if (!p_kppp->stats->local_ip_address.isEmpty() && gpppdata.autoname()) {
    local_ip.s_addr=inet_addr(p_kppp->stats->local_ip_address.ascii());
    hostname_entry=gethostbyaddr((const char *)&local_ip,sizeof(in_addr),AF_INET);

    if (hostname_entry != 0L) {
      new_hostname=hostname_entry->h_name;
      dot=new_hostname.find('.');
      new_hostname=new_hostname.remove(dot,new_hostname.length()-dot);
      Requester::rq->setHostname(new_hostname);
      modified_hostname = TRUE;

      new_hostname=hostname_entry->h_name;
      new_hostname.remove(0,dot+1);

      add_domain(new_hostname);
    }
  }

}

// Replace the DNS domain entry in the /etc/resolv.conf file and
// disable the nameserver entries if option is enabled
void add_domain(const QString &domain) {

  int fd;
  char c;
  QString resolv[MAX_RESOLVCONF_LINES];

  if (domain.isEmpty())
    return;

  if((fd = Requester::rq->openResolv(O_RDONLY)) >= 0) {

    int i=0;
    while((read(fd, &c, 1) == 1) && (i < MAX_RESOLVCONF_LINES)) {
      if(c == '\n') {
	i++;
      }
      else {
	resolv[i] += c;
      }
    }
    close(fd);
    if ((c != '\n') && (i < MAX_RESOLVCONF_LINES)) i++;

    if((fd = Requester::rq->openResolv(O_WRONLY|O_TRUNC)) >= 0) {
      Q3CString tmp = "domain " + domain.local8Bit() +
		     " \t\t#kppp temp entry\n";
      write(fd, tmp.data(), tmp.length());

      for(int j=0; j < i; j++) {
	if((resolv[j].contains("domain") ||
	      ( resolv[j].contains("nameserver")
		&& !resolv[j].contains("#kppp temp entry")
		&& gpppdata.exDNSDisabled()))
	        && !resolv[j].contains("#entry disabled by kppp")) {
	  Q3CString tmp = "# " + resolv[j].local8Bit() +
			 " \t#entry disabled by kppp\n";
	  write(fd, tmp, tmp.length());
	}
	else {
	  Q3CString tmp = resolv[j].local8Bit() + "\n";
	  write(fd, tmp, tmp.length());
	}
      }
    }
    close(fd);
  }
}


// adds the DNS entries in the /etc/resolv.conf file
void adddns()
{
  int fd;

  if ((fd = Requester::rq->openResolv(O_WRONLY|O_APPEND)) >= 0) {
    QStringList &dnslist = gpppdata.dns();
    for ( QStringList::Iterator it = dnslist.begin();
          it != dnslist.end();
          ++it )
    {
      Q3CString dns = "nameserver " + (*it).local8Bit() +
		     " \t#kppp temp entry\n";
      write(fd, dns.data(), dns.length());
    }
    close(fd);
  }
  add_domain(gpppdata.domain());
}

void addpeerdns() {
  int fd, fd2;

  if((fd = Requester::rq->openResolv(O_WRONLY|O_APPEND)) >= 0) {
    if((fd2 = open("/etc/ppp/resolv.conf", O_RDONLY)) >= 0) {
      char c;
      int i = 0;
      while(i++ < 100 && read(fd2, &c, 1) == 1) {
        if(c == '\n')
          write(fd, "\t#kppp temp entry\n", 18);
        else
          write(fd, &c, 1);
      }
      close(fd2);
    } else
      fprintf(stderr, "failed to read from /etc/ppp/resolv.conf\n");
    close(fd);
  }
  add_domain(gpppdata.domain());
}

// remove the dns entries from the /etc/resolv.conf file
void removedns() {

  int fd;
  char c;
  QString resolv[MAX_RESOLVCONF_LINES];

  if((fd = Requester::rq->openResolv(O_RDONLY)) >= 0) {

    int i=0;
    while(read(fd, &c, 1) == 1 && i < MAX_RESOLVCONF_LINES) {
      if(c == '\n') {
	i++;
      }
      else {
	resolv[i] += c;
      }
    }
    close(fd);

    if((fd = Requester::rq->openResolv(O_WRONLY|O_TRUNC)) >= 0) {
      for(int j=0; j < i; j++) {
	if(resolv[j].contains("#kppp temp entry")) continue;
	if(resolv[j].contains("#entry disabled by kppp")) {
          Q3CString tmp = resolv[j].local8Bit();
	  write(fd, tmp.data()+2, tmp.length() - 27);
	  write(fd, "\n", 1);
	}
	else {
	  Q3CString tmp = resolv[j].local8Bit() + "\n";
	  write(fd, tmp, tmp.length());
	}
      }
    }
    close(fd);

  }

  if (  modified_hostname ) {
    Requester::rq->setHostname(old_hostname);
    modified_hostname = FALSE;
  }

}

#include "connect.moc"
