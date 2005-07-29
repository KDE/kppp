/* -*- C++ -*-
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 * This file contributed by: Mario Weilguni, <mweilguni@sime.com>
 * Thanks Mario!
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

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <qdir.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <Q3CString>

#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>
#include <time.h>

#include "accounting.h"
#include "pppdata.h"
#include "pppstats.h"

// defines the maximum duration until the current costs
// are saved again (to prevent loss due to "kill")
// specifying -1 disables the features
#define UPDATE_TIME    (5*60*1000)

extern PPPData gpppdata;

/////////////////////////////////////////////////////////////////////////////
//
// Helper functions
//
/////////////////////////////////////////////////////////////////////////////
static QString timet2qstring(time_t t) {
  QString s;

  s.sprintf("%lu", t);
  return s;
}


/////////////////////////////////////////////////////////////////////////////
//
// The base class for the accounting system provides a base set of usefull
// and common functions, but does not do any accounting by itself. The 
// accounting is accomplished withing it's derived classes
//
/////////////////////////////////////////////////////////////////////////////
AccountingBase::AccountingBase(QObject *parent) :
  QObject(parent),
  _total(0),
  _session(0)
{
  QDate dt = QDate::currentDate();
  LogFileName = QString("%1-%2.log")
    .arg(QDate::shortMonthName(dt.month()))
    .arg(dt.year(), 4);

  LogFileName = KGlobal::dirs()->saveLocation("appdata", "Log")
    + "/" + LogFileName;

  kdDebug(5002) << "LogFileName: " << LogFileName << endl;
}

AccountingBase::~AccountingBase() {
  if(running())
    slotStop();
}


double AccountingBase::total() const {
  return _total + _session;
}



double AccountingBase::session() const {
  return _session;
}


// set costs back to zero ( typically once per month)
void AccountingBase::resetCosts(const QString & accountname){
  QString prev_account = gpppdata.accname();

  gpppdata.setAccount(accountname);
  gpppdata.setTotalCosts("");

  gpppdata.setAccount(prev_account);
}


void AccountingBase::resetVolume(const QString & accountname){
  QString prev_account = gpppdata.accname();

  gpppdata.setAccount(accountname);
  gpppdata.setTotalBytes(0);

  gpppdata.setAccount(prev_account);
}


void AccountingBase::logMessage(QString s, bool newline) {
  int old_umask = umask(0077);

  QFile f(LogFileName);

  bool result = f.open(QIODevice::ReadWrite);
  if(result) {
    // move to eof, and place \n if necessary
    if(f.size() > 0) {
      if(newline) {
	f.at(f.size());
	char c = 0;
	f.readBlock(&c, 1);
	if(c != '\n')
	  f.writeBlock("\n", 1);
      } else
	f.at(f.size());
    }

    Q3CString tmp = s.local8Bit();
    f.writeBlock(tmp, tmp.length());
    f.close();
  }

  // restore umask
  umask(old_umask);
}


QString AccountingBase::getCosts(const QString & accountname) {
  QString prev_account = gpppdata.accname();

  gpppdata.setAccount(accountname);
  QString val = gpppdata.totalCosts();
  // ### currency from rule file
  //  QString val = KGlobal::locale()->formatMoney(gpppdata.totalCosts().toDouble(), currency);

  gpppdata.setAccount(prev_account);

  return val;
}



bool AccountingBase::saveCosts() {
  if(!_name.isNull() && _name.length() > 0) {
    QString val;
    val.setNum(total());

    gpppdata.setTotalCosts(val);
    gpppdata.save();

    return TRUE;
  } else
    return FALSE;
}


bool AccountingBase::loadCosts() {
  QString val = gpppdata.totalCosts();

  if(val.isNull()) // QString will segfault if isnull and toDouble called
    _total = 0.0;
  else {
    bool ok;
    _total = val.toDouble(&ok);
    if(!ok)
      _total = 0.0;
  }

  return TRUE;
}


QString AccountingBase::getAccountingFile(const QString &accountname) {
  QString f = "kppp/Rules/";
  f += accountname;
  QString d = locate("data", f);

  if(d.isNull())
    return "";
  else
    return d;
}


/////////////////////////////////////////////////////////////////////////////
//
// Accounting class for ruleset files
//
/////////////////////////////////////////////////////////////////////////////
Accounting::Accounting(QObject *parent, PPPStats *st) : 
  AccountingBase(parent),
  acct_timer_id(0),
  update_timer_id(0),
  stats(st)
{
}


bool Accounting::running() const {
  return (bool)(acct_timer_id != 0);
}


void Accounting::timerEvent(QTimerEvent *t) {
  if(t->timerId() == acct_timer_id) {

    double newCosts;
    double newLen;
    double connect_time = difftime(time(0), start_time);

    rules.getActiveRule(QDateTime::currentDateTime(), connect_time, newCosts, newLen);
    if(newLen < 1) { // changed to < 1
      slotStop();
      return; // no default rule found
    }

    // check if we have a new rule. If yes,
    // kill the timer and restart it with new
    // duration
    if((newCosts != _lastcosts) || (newLen != _lastlen)) {

      kdDebug(5002).form("SWITCHING RULES, new costs = %0.2f, new len = %0.2f\n",
	     newCosts, newLen);

      killTimer(acct_timer_id);
      if(newLen > 0)
	acct_timer_id = startTimer((int)(newLen * 1000.0));

      _lastlen = newLen;
      _lastcosts = newCosts;
    }

    // emit changed() signal if necessary
    if(newCosts != 0) {
      _session += newCosts;
       emit changed(rules.currencyString(total()),
		    rules.currencyString(session()));


    }
  } // if(t->timerId() == acct_timer_id)...

  if(t->timerId() == update_timer_id) {
    // just to be sure, save the current costs
    // every n seconds (see UPDATE_TIME)
    saveCosts();
  }
}


void Accounting::slotStart() {
  if(!running()) {
    loadCosts();
    _lastcosts = 0.0;
    _lastlen   = 0.0;
    _session = rules.perConnectionCosts();
    rules.setStartTime(QDateTime::currentDateTime());
    acct_timer_id = startTimer(1);
     if(UPDATE_TIME > 0)
       update_timer_id = startTimer(UPDATE_TIME);

    start_time = time(0); 
    QString s;
    s = timet2qstring(start_time);
    s += ":";
    s += gpppdata.accname();
    s += ":";
    s += rules.currencySymbol();

    logMessage(s, TRUE);
  }
}


void Accounting::slotStop() {
  if(running()) {
    killTimer(acct_timer_id);
    if(update_timer_id != 0)
      killTimer(update_timer_id);
    acct_timer_id = 0;
    update_timer_id = 0;

    QString s;
    s.sprintf(":%s:%0.4e:%0.4e:%u:%u\n",
	      timet2qstring(time(0)).utf8().data(),
	      session(),
	      total(),
	      stats->ibytes,
	      stats->obytes);

    logMessage(s, FALSE);
    saveCosts();
  }
}


bool Accounting::loadRuleSet(const QString & name) {

  if (name.isEmpty()) {
    rules.load(""); // delete old rules
    return TRUE;
  }
  
  QString d = AccountingBase::getAccountingFile(name);

  QFileInfo fg(d);
   if(fg.exists()) {
     int ret = rules.load(d);
     _name = rules.name();
     return (bool)(ret == 0);
   }

 return FALSE;
}


double Accounting::total() const {
  if(rules.minimumCosts() <= _session)
    return _total + _session;
  else
    return _total + rules.minimumCosts();
}



double Accounting::session() const {
  if(rules.minimumCosts() <= _session)
    return _session;
  else
    return rules.minimumCosts();
}




ExecutableAccounting::ExecutableAccounting(PPPStats *st, QObject *parent) : 
  AccountingBase(parent),
  proc(0),
  stats(st)
{
}


bool ExecutableAccounting::running() const {
  return (proc != 0) || proc->isRunning();
}


bool ExecutableAccounting::loadRuleSet(const QString &) {
  QString s = AccountingBase::getAccountingFile(gpppdata.accountingFile());
  return (access(QFile::encodeName(s), X_OK) == 0);
}


void ExecutableAccounting::gotData(KProcess */*proc*/, char *buffer, int /*buflen*/) {
  QString field[8];
  int nFields = 0;
  int pos, last_pos = 0;

  // split string
  QString b(buffer);
  pos = b.find(':');
  while(pos != -1 && nFields < 8) {
    field[nFields++] = b.mid(last_pos, pos-last_pos);
    last_pos = pos+1;
    pos = b.find(':', last_pos);
  }

  for(int i = 0; i < nFields;i++)
    fprintf(stderr, "FIELD[%d] = %s\n", i, field[i].local8Bit().data());
  
  QString __total, __session;
  QString s(buffer);
  int del1, del2, del3;

  del1 = s.find(':');
  del2 = s.find(':', del1+1);
  del3 = s.find(':', del2+1);
  if(del1 == -1 || del2 == -1 || del3 == -1) {
    // TODO: do something usefull here
    return;
  }

  provider = s.left(del1);
  currency = s.mid(del1, del2-del1);
  __total = s.mid(del2, del2-del1);
  __session = s.mid(del3, s.length()-del3+1);

  bool ok1, ok2;
  _total = __total.toDouble(&ok1);
  _session = __session.toDouble(&ok2);
  
  if(!ok1 || !ok2) {
    // TODO: do something usefull here
    return;
  }

  printf("PROVIDER=%s, CURRENCY=%s, TOTAL=%0.3e, SESSION=%0.3e\n", 
	 provider.local8Bit().data(),
	 currency.local8Bit().data(),
	 _total,
	 _session);
}


void ExecutableAccounting::slotStart() {
  if(proc != 0)
    slotStop(); // just to make sure

  loadCosts();
  QString s = AccountingBase::getAccountingFile(gpppdata.accountingFile());
  proc = new KProcess;

  QString s_total;
  s_total.sprintf("%0.8f", total());
  *proc << s << s_total;
  connect(proc, SIGNAL(receivedStdout(KProcess *, char *, int)),
	  this, SLOT(gotData(KProcess *, char *, int)));
  proc->start();

  time_t start_time = time(0); 
  s = timet2qstring(start_time);
  s += ":";
  s += gpppdata.accname();
  s += ":";
  s += currency;
  
  logMessage(s, TRUE);
}


void ExecutableAccounting::slotStop() {
  if(proc != 0) {
    proc->kill();
    delete proc;
    proc = 0;

    QString s;
    s.sprintf(":%s:%0.4e:%0.4e:%u:%u\n",
	      timet2qstring(time(0)).local8Bit().data(),
	      session(),
	      total(),
	      stats->ibytes,
	      stats->obytes);

    logMessage(s, FALSE);
    saveCosts();
  }
}

#include "accounting.moc"

