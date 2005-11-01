/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "pppdata.h"
#include "runtests.h"
#include "devices.h"
#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kglobalsettings.h>
#include <assert.h>
#include <kglobal.h>

PPPData gpppdata;


PPPData::PPPData()
  :  config(0L),
     accounthighcount(-1),        // start out with no account entries
     caccount(-1),         // set the current account index also
     modemhighcount(-1),        // start out with no modem entries
     cmodem(-1),         // set the current modem index also
     suidprocessid(-1),    // process ID of setuid child
     pppdisrunning(false),
     pppderror(0),
     waitcallback(false)
{
}


//
// open configuration file
//
bool PPPData::open() {

  config = KGlobal::config();

  if (config->getConfigState() == KConfig::NoAccess) {
    KMessageBox::error(0L,
                       i18n("The application-specific config file could not "
                       "be opened in either read-write or read-only mode.\n"
                       "The superuser might have to change its ownership "
                       "by issuing the following command in your home directory:\n"
                       "chown {YourUsername} .kde/share/config/kppprc"),
			 kapp->name());
    return false;
  }

  // don't expand shell variables
  config->setDollarExpansion(false);

  accounthighcount = readNumConfig(GENERAL_GRP, NUMACCOUNTS_KEY, 0) - 1;

  if (accounthighcount > MAX_ACCOUNTS)
    accounthighcount = MAX_ACCOUNTS;

  if(accounthighcount >= 0 && defaultAccount().isEmpty()) {
    setAccountByIndex(0);
    setDefaultAccount(accname());
  } else if(!setAccount(defaultAccount()))
    setDefaultAccount(accname());

  modemhighcount = readNumConfig(GENERAL_GRP, NUMMODEMS_KEY, 0) - 1;

  if (modemhighcount > MAX_MODEMS)
    modemhighcount = MAX_MODEMS;

  // if there aren't no ModemX setted and exists the [Modem] group,
  // probably it's the first time we are using this new version
  // with multiple modem profiles.
  // So we copy the old [Modem] to the new [Modem0]
  if(modemhighcount < 0 && defaultModem().isEmpty() && config->hasGroup("Modem"))
  {
  	config->setGroup("Modem");

    QMap <QString, QString> map = config->entryMap("Modem");
    QMap <QString, QString>::ConstIterator it = map.begin();

    newmodem();

    while (it != map.end()) {
	  config->setGroup(cmodemgroup);
      config->writeEntry(it.key(), *it);
      it++;
    }

	QString newname("Modem0");
    setModname(newname);
  }

  if(modemhighcount >= 0 && defaultModem().isEmpty()) {
    setModemByIndex(0);
    setDefaultModem(modname());
  } else if(!setModem(defaultModem()))
    setDefaultModem(modname());


  // start out with internal debugging disabled
  // the user is still free to specify `debug' on his own
  setPPPDebug(false);

  ::pppdVersion(&pppdVer, &pppdMod, &pppdPatch);

  return true;
}


//
// save configuration
//
void PPPData::save() {

  if (config) {
    writeConfig(GENERAL_GRP, NUMACCOUNTS_KEY, accountCount());
    writeConfig(GENERAL_GRP, NUMMODEMS_KEY, modemCount());
    config->sync();
  }

}


//
// cancel changes
//
void PPPData::cancel() {

  if (config) {
    config->rollback();
    config->reparseConfiguration();
  }

}


// currently differentiates between READWRITE and NONE only
int PPPData::access() const {

  return config->getConfigState();
}


// functions to read/write date to configuration file
QString PPPData::readConfig(const QString &group, const QString &key,
                            const QString &defvalue = "")
{
  if (config) {
    config->setGroup(group);
    return config->readEntry(key, defvalue);
  } else
    return defvalue;
}


int PPPData::readNumConfig(const QString &group, const QString &key,
			   int defvalue) {
  if (config) {
    config->setGroup(group);
    return config->readNumEntry(key, defvalue);
  } else
    return defvalue;

}


bool PPPData::readListConfig(const QString &group, const QString &key,
                             QStringList &list, char sep) {
  list.clear();
  if (config) {
    config->setGroup(group);
    list = config->readListEntry(key, sep);
    return true;
  } else
    return false;
}


void PPPData::writeConfig(const QString &group, const QString &key,
			  const QString &value) {
  if (config) {
    config->setGroup(group);
    config->writeEntry(key, value);
  }
}


void PPPData::writeConfig(const QString &group, const QString &key, int value) {
  if (config) {
    config->setGroup(group);
    config->writeEntry(key, value);
  }
}


void PPPData::writeListConfig(const QString &group, const QString &key,
                              QStringList &list, char sep) {
  if (config) {
    config->setGroup(group);
    config->writeEntry(key, list, sep);
  }
}


//
// functions to set/return general information
//
QString PPPData::password() const {
  return passwd;
}


void PPPData::setPassword(const QString &pw) {
  passwd = pw;
}


const QString PPPData::defaultAccount() {
  return readConfig(GENERAL_GRP, DEFAULTACCOUNT_KEY);
}


void PPPData::setDefaultAccount(const QString &n) {
  writeConfig(GENERAL_GRP, DEFAULTACCOUNT_KEY, n);

  //now set the current account index to the default account
  setAccount(defaultAccount());
}


const QString PPPData::defaultModem() {
  return readConfig(GENERAL_GRP, DEFAULTMODEM_KEY);
}


void PPPData::setDefaultModem(const QString &n) {
  writeConfig(GENERAL_GRP, DEFAULTMODEM_KEY, n);

  //now set the current modem index to the default modem
  setModem(defaultModem());
}

bool PPPData::get_show_clock_on_caption() {
  return (bool) readNumConfig(GENERAL_GRP, SHOWCLOCK_KEY, true);
}


void PPPData::set_show_clock_on_caption(bool set) {
  writeConfig(GENERAL_GRP, SHOWCLOCK_KEY, (int) set);
}


bool PPPData::get_xserver_exit_disconnect() {
  return (bool) readNumConfig(GENERAL_GRP, DISCONNECT_KEY, true);
}

bool PPPData::get_redial_on_nocarrier() {
  return (bool) readNumConfig(GENERAL_GRP, REDIALONNOCARR_KEY, false);
}


void PPPData::setPPPDebug(bool set) {
  writeConfig(GENERAL_GRP, PPP_DEBUG_OPTION, (int)set);
}


bool PPPData::getPPPDebug() {
  return (bool)readNumConfig(GENERAL_GRP, PPP_DEBUG_OPTION, false);
}


void PPPData::set_xserver_exit_disconnect(bool set) {
  writeConfig(GENERAL_GRP, DISCONNECT_KEY, (int) set);
}

void PPPData::set_redial_on_nocarrier(bool set) {
  writeConfig(GENERAL_GRP, REDIALONNOCARR_KEY, (int) set);
}


bool PPPData::quit_on_disconnect() {
  return (bool) readNumConfig(GENERAL_GRP, QUITONDISCONNECT_KEY, false);
}


void PPPData::set_quit_on_disconnect(bool set) {
  writeConfig(GENERAL_GRP, QUITONDISCONNECT_KEY, (int) set);
}


bool PPPData::get_show_log_window() {
  return (bool) readNumConfig (GENERAL_GRP, SHOWLOGWIN_KEY, false);
}


void PPPData::set_show_log_window(bool set) {
  writeConfig(GENERAL_GRP, SHOWLOGWIN_KEY, (int) set);
}


bool PPPData::automatic_redial() {
  return (bool) readNumConfig(GENERAL_GRP, AUTOREDIAL_KEY, FALSE);
}


void PPPData::set_automatic_redial(bool set) {
  writeConfig(GENERAL_GRP, AUTOREDIAL_KEY, (int) set);
}


bool PPPData::get_iconify_on_connect() {
  return (bool) readNumConfig(GENERAL_GRP, ICONIFY_ON_CONNECT_KEY, TRUE);
}


void PPPData::set_iconify_on_connect(bool set) {
  writeConfig(GENERAL_GRP, ICONIFY_ON_CONNECT_KEY, (int) set);
}


bool PPPData::get_dock_into_panel() {
  return (bool) readNumConfig(GENERAL_GRP, DOCKING_KEY, false);
}


void PPPData::set_dock_into_panel(bool set) {
  writeConfig(GENERAL_GRP, DOCKING_KEY, (int) set);
}


QString PPPData::pppdVersion() {
  return QString("%1.%2.%3").arg(pppdVer).arg(pppdMod).arg(pppdPatch);
}

bool PPPData::pppdVersionMin(int ver, int mod, int patch) {
  // check if pppd version fulfills minimum requirement
  return (pppdVer > ver
          || (pppdVer == ver && pppdMod > mod)
          || (pppdVer == ver && pppdMod == mod && pppdPatch >= patch));
}

int PPPData::pppdTimeout() {
  return readNumConfig(GENERAL_GRP, PPPDTIMEOUT_KEY, PPPD_TIMEOUT);
}


void PPPData::setpppdTimeout(int n) {
  writeConfig(GENERAL_GRP, PPPDTIMEOUT_KEY, n);
}

//
// functions to set/return modem information
//


//returns number of modems
int PPPData::modemCount() const {
  return modemhighcount + 1;
}


bool PPPData::setModem(const QString &mname) {
  for(int i = 0; i <= modemhighcount; i++) {
    setModemByIndex(i);
    if(modname() == mname) {
      cmodem = i;
      return true;
    }
  }
  return false;
}


bool PPPData::setModemByIndex(int i) {
  if(i >= 0 && i <= modemhighcount) {
    cmodem = i;
    cmodemgroup.sprintf("%s%i", MODEM_GRP, i);
    return true;
  }
  return false;
}


bool PPPData::isUniqueModname(const QString &n) {
  int current = cmodem;
  for(int i=0; i <= modemhighcount; i++) {
    setModemByIndex(i);
    if(modname() == n && i != current) {
      setModemByIndex(current);
      return false;
    }
  }
  setModemByIndex(current);
  return true;
}


bool PPPData::deleteModem() {
  if(cmodem < 0)
    return false;

  QMap <QString, QString> map;
  QMap <QString, QString>::Iterator it;

  // set all entries of the current modem to ""
  map = config->entryMap(cmodemgroup);
  it = map.begin();
  while (it != map.end()) {
    config->writeEntry(it.key(), "");
    it++;
  }

  // shift the succeeding modems
  for(int i = cmodem+1; i <= modemhighcount; i++) {
    setModemByIndex(i);
    map = config->entryMap(cmodemgroup);
    it = map.begin();
    setModemByIndex(i-1);
    config->setGroup(cmodemgroup);
    while (it != map.end()) {
      config->writeEntry(it.key(), *it);
      it++;
    }
  }

  // make sure the top modem is cleared
  setModemByIndex(modemhighcount);
  map = config->entryMap(cmodemgroup);
  it = map.begin();
  config->setGroup(cmodemgroup);
  while (!it.key().isNull()) {
    config->writeEntry(it.key(), "");
    it++;
  }

  modemhighcount--;
  if(cmodem > modemhighcount)
    cmodem = modemhighcount;

  setModemByIndex(cmodem);

  return true;
}

bool PPPData::deleteModem(const QString &mname) {
  if(!setModem(mname))
    return false;

  deleteModem();

  return true;
}


int PPPData::newmodem() {

  if(!config || modemhighcount >= MAX_MODEMS)
    return -1;

  modemhighcount++;
  setModemByIndex(modemhighcount);

  setpppdArgumentDefaults();

  return cmodem;
}

int PPPData::copymodem(int i) {

  config->setGroup(cmodemgroup);

  if(modemhighcount >= MAX_MODEMS)
    return -1;

  setModemByIndex(i);

  QMap <QString, QString> map = config->entryMap(cmodemgroup);
  QMap <QString, QString>::ConstIterator it = map.begin();

  QString newname = i18n("%1_copy").arg(modname());

  newmodem();

  while (it != map.end()) {
	config->setGroup(cmodemgroup);
    config->writeEntry(it.key(), *it);
    it++;
  }

  setModname(newname);

  return cmodem;
}


const QString PPPData::modname() {
  return readConfig(cmodemgroup, MOD_NAME_KEY);
}

void PPPData::setModname(const QString &n) {
  if(!cmodemgroup.isNull()) {
    // are we manipulating the default modem's name ? then change it, too.
    bool def = modname() == defaultModem();
    writeConfig(cmodemgroup, MOD_NAME_KEY, n);
    if (def)
      setDefaultModem(n);
  }
}




const QString PPPData::modemDevice() {
  return readConfig (cmodemgroup, MODEMDEV_KEY, devices[DEV_DEFAULT]);
}


void PPPData::setModemDevice(const QString &n) {
  writeConfig(cmodemgroup, MODEMDEV_KEY, n);
}


QString PPPData::flowcontrol() {
  // keep default value in sync with general.cpp
  return readConfig(cmodemgroup, FLOWCONTROL_KEY, i18n("Hardware [CRTSCTS]"));
}


void PPPData::setFlowcontrol(const QString &n) {
  writeConfig(cmodemgroup, FLOWCONTROL_KEY, n);
}


const QString PPPData::speed() {
  QString s = readConfig(cmodemgroup, SPEED_KEY, "57600");
  // undo the damage of a bug in former versions. It left an empty Speed=
  // entry in kppprc. kppp did set the serial port to 57600 as default but
  // pppd wouldn't receive the speed via the command line.
  if(s.toUInt() == 0)
    s = "57600";
  return s;
}


void PPPData::setSpeed(const QString &n) {
  writeConfig(cmodemgroup, SPEED_KEY, n);
}


#if 0
void PPPData::setUseCDLine(const int n) {
  writeConfig(cmodemgroup,USECDLINE_KEY,n);
}


int PPPData::UseCDLine() {
  return  readNumConfig(cmodemgroup,USECDLINE_KEY,0);
}
#endif

const QString  PPPData::modemEscapeStr() {
  return readConfig(cmodemgroup,ESCAPESTR_KEY,"+++");
}


void PPPData::setModemEscapeStr(const QString &n) {
  writeConfig(cmodemgroup,ESCAPESTR_KEY,n);
}


const QString  PPPData::modemEscapeResp() {
  return readConfig(cmodemgroup,ESCAPERESP_KEY,"OK");
}


void PPPData::setModemEscapeResp(const QString &n) {
  writeConfig(cmodemgroup,ESCAPERESP_KEY,n);
}


int  PPPData::modemEscapeGuardTime() {
  return readNumConfig(cmodemgroup,ESCAPEGUARDTIME_KEY,50);
}


void PPPData::setModemEscapeGuardTime(int n) {
  writeConfig(cmodemgroup,ESCAPEGUARDTIME_KEY,n);
}


bool PPPData::modemLockFile() {
  return readNumConfig(cmodemgroup, LOCKFILE_KEY, 1);
}


void PPPData::setModemLockFile(bool set) {
  writeConfig(cmodemgroup, LOCKFILE_KEY, set);
}


int PPPData::modemTimeout() {
  return readNumConfig(cmodemgroup, TIMEOUT_KEY, MODEM_TIMEOUT);
}


void PPPData::setModemTimeout(int n) {
  writeConfig(cmodemgroup, TIMEOUT_KEY, n);
}


int PPPData::modemToneDuration() {
  return readNumConfig(cmodemgroup, TONEDURATION_KEY,MODEM_TONEDURATION);
}


void PPPData::setModemToneDuration(int n) {
  writeConfig(cmodemgroup, TONEDURATION_KEY, n);
}


int PPPData::busyWait() {
  return readNumConfig(cmodemgroup, BUSYWAIT_KEY, BUSY_WAIT);
}


void PPPData::setbusyWait(int n) {
  writeConfig(cmodemgroup, BUSYWAIT_KEY, n);
}


//
//Advanced "Modem" dialog
//
// defaults: InitString=ATZ, InitString1="" etc.
const QString PPPData::modemInitStr(int i) {
  assert(i >= 0 && i < NumInitStrings);
  if(i == 0)
    return readConfig(cmodemgroup, INITSTR_KEY, "ATZ");
  else
    return readConfig(cmodemgroup, INITSTR_KEY + QString::number(i), "");
}


void PPPData::setModemInitStr(int i, const QString &n) {
  assert(i >= 0 && i < NumInitStrings);
  QString k = INITSTR_KEY + (i > 0 ? QString::number(i) : "");
  writeConfig(cmodemgroup, k, n);
}


const QString PPPData::modemInitResp() {
  return readConfig(cmodemgroup, INITRESP_KEY, "OK");
}


void PPPData::setModemInitResp(const QString &n) {
  writeConfig(cmodemgroup, INITRESP_KEY, n);
}


int PPPData::modemPreInitDelay() {
  return readNumConfig(cmodemgroup, PREINITDELAY_KEY, 50);
}


void PPPData::setModemPreInitDelay(int n) {
  writeConfig(cmodemgroup, PREINITDELAY_KEY, n);
}


int PPPData::modemInitDelay() {
  return readNumConfig(cmodemgroup, INITDELAY_KEY, 50);
}


void PPPData::setModemInitDelay(int n) {
  writeConfig(cmodemgroup, INITDELAY_KEY, n);
}

QString PPPData::modemNoDialToneDetectionStr() {
  return readConfig(cmodemgroup, NODTDETECT_KEY, "ATX3");
}

void PPPData::setModemNoDialToneDetectionStr(const QString &n) {
  writeConfig(cmodemgroup, NODTDETECT_KEY, n);
}

const QString PPPData::modemDialStr() {
  return readConfig(cmodemgroup, DIALSTR_KEY, "ATDT");
}


void PPPData::setModemDialStr(const QString &n) {
  writeConfig(cmodemgroup, DIALSTR_KEY, n);
}


const QString PPPData::modemConnectResp() {
  return readConfig(cmodemgroup, CONNECTRESP_KEY, "CONNECT");
}


void PPPData::setModemConnectResp(const QString &n) {
  writeConfig(cmodemgroup, CONNECTRESP_KEY, n);
}


const QString PPPData::modemBusyResp() {
  return readConfig(cmodemgroup, BUSYRESP_KEY, "BUSY");
}


void PPPData::setModemBusyResp(const QString &n) {
  writeConfig(cmodemgroup, BUSYRESP_KEY, n);
}


const QString PPPData::modemNoCarrierResp() {
  return readConfig(cmodemgroup, NOCARRIERRESP_KEY, "NO CARRIER");
}


void PPPData::setModemNoCarrierResp(const QString &n) {
  writeConfig(cmodemgroup, NOCARRIERRESP_KEY, n);
}


const QString PPPData::modemNoDialtoneResp() {
  return readConfig(cmodemgroup, NODIALTONERESP_KEY, "NO DIALTONE");
}


void PPPData::setModemNoDialtoneResp(const QString &n) {
  writeConfig(cmodemgroup, NODIALTONERESP_KEY, n);
}


const QString PPPData::modemHangupStr() {
  return readConfig(cmodemgroup, HANGUPSTR_KEY, "+++ATH");
}

void PPPData::setModemHangupStr(const QString &n) {
  writeConfig(cmodemgroup, HANGUPSTR_KEY, n);
}


const QString PPPData::modemHangupResp() {
  return readConfig(cmodemgroup, HANGUPRESP_KEY, "OK");
}

void PPPData::setModemHangupResp(const QString &n) {
  writeConfig(cmodemgroup, HANGUPRESP_KEY, n);
}


QString PPPData::modemDLPResp() {
  return readConfig(cmodemgroup, DLPRESP_KEY, "DIGITAL LINE DETECTED");
}

void PPPData::setModemDLPResp(const QString &n) {
  writeConfig(cmodemgroup, DLPRESP_KEY, n);
}




const QString PPPData::modemAnswerStr() {
  return readConfig(cmodemgroup, ANSWERSTR_KEY, "ATA");
}


QString PPPData::volumeOff() {
  return readConfig(cmodemgroup, VOLUME_OFF, "M0L0");
}


void PPPData::setVolumeOff(const QString &s) {
  writeConfig(cmodemgroup, VOLUME_OFF, s);
}


QString PPPData::volumeMedium() {
 return readConfig(cmodemgroup, VOLUME_MEDIUM, "M1L1");
}


void PPPData::setVolumeMedium(const QString &s) {
  writeConfig(cmodemgroup, VOLUME_MEDIUM, s);
}


QString PPPData::volumeHigh() {
  QString tmp = readConfig(cmodemgroup, VOLUME_HIGH, "M1L3");
  if(tmp == "M1L4")
    tmp = "M1L3";
  return tmp;
}


void PPPData::setVolumeHigh(const QString &s) {
 writeConfig(cmodemgroup, VOLUME_HIGH, s);
}


QString PPPData::volumeInitString() {
  QString s;

  switch(volume()) {
  case 0:
    s = volumeOff();
    break;
  case 1:
    s = volumeMedium();
    break;
  case 2:
    s = volumeHigh();
    break;
  default:
    s = volumeMedium();
  }

  return s;
}


int PPPData::volume() {
  return readNumConfig(cmodemgroup, VOLUME_KEY, 1);
}


void PPPData::setVolume(int i) {
  writeConfig(cmodemgroup, VOLUME_KEY, i);
}

int PPPData::waitForDialTone() {
  return readNumConfig(cmodemgroup, DIALTONEWAIT_KEY, 1);
}

void PPPData::setWaitForDialTone(int i) {
  writeConfig(cmodemgroup, DIALTONEWAIT_KEY, i);
}

void PPPData::setModemAnswerStr(const QString &n) {
  writeConfig(cmodemgroup, ANSWERSTR_KEY, n);
}


const QString PPPData::modemRingResp() {
  return readConfig(cmodemgroup, RINGRESP_KEY, "RING");
}


void PPPData::setModemRingResp(const QString &n) {
  writeConfig(cmodemgroup, RINGRESP_KEY, n);
}


const QString PPPData::modemAnswerResp() {
  return readConfig(cmodemgroup, ANSWERRESP_KEY, "CONNECT");
}


void PPPData::setModemAnswerResp(const QString &n) {
  writeConfig(cmodemgroup, ANSWERRESP_KEY, n);
}


const QString PPPData::enter() {
  return readConfig(cmodemgroup, ENTER_KEY, "CR");
}


void PPPData::setEnter(const QString &n) {
  writeConfig(cmodemgroup, ENTER_KEY, n);
}


//
// functions to set/return account information
//

//returns number of accounts
int PPPData::accountCount() const {
  return accounthighcount + 1;
}


bool PPPData::setAccount(const QString &aname) {
  for(int i = 0; i <= accounthighcount; i++) {
    setAccountByIndex(i);
    if(accname() == aname) {
      caccount = i;
      return true;
    }
  }
  return false;
}


bool PPPData::setAccountByIndex(int i) {
  if(i >= 0 && i <= accounthighcount) {
    caccount = i;
    caccountgroup.sprintf("%s%i", ACCOUNT_GRP, i);
    return true;
  }
  return false;
}


bool PPPData::isUniqueAccname(const QString &n) {
  if(n.contains(':'))
    return false;
  int current = caccount;
  for(int i=0; i <= accounthighcount; i++) {
    setAccountByIndex(i);
    if(accname() == n && i != current) {
      setAccountByIndex(current);
      return false;
    }
  }
  setAccountByIndex(current);
  return true;
}


bool PPPData::deleteAccount() {
  if(caccount < 0)
    return false;

  QMap <QString, QString> map;
  QMap <QString, QString>::Iterator it;

  // set all entries of the current account to ""
  map = config->entryMap(caccountgroup);
  it = map.begin();
  while (it != map.end()) {
    config->writeEntry(it.key(), "");
    ++it;
  }

  // shift the succeeding accounts
  for(int i = caccount+1; i <= accounthighcount; i++) {
    setAccountByIndex(i);
    map = config->entryMap(caccountgroup);
    it = map.begin();
    setAccountByIndex(i-1);
    config->setGroup(caccountgroup);
    while (it != map.end()) {
      config->writeEntry(it.key(), *it);
      ++it;
    }
  }

  // make sure the top account is cleared
  setAccountByIndex(accounthighcount);
  map = config->entryMap(caccountgroup);
  it = map.begin();
  config->setGroup(caccountgroup);
  while (it != map.end() && !it.key().isNull()) {
    config->writeEntry(it.key(), "");
    ++it;
  }

  accounthighcount--;
  if(caccount > accounthighcount)
    caccount = accounthighcount;

  setAccountByIndex(caccount);

  return true;
}

bool PPPData::deleteAccount(const QString &aname) {
  if(!setAccount(aname))
    return false;

  deleteAccount();

  return true;
}


int PPPData::newaccount() {

  if(!config || accounthighcount >= MAX_ACCOUNTS)
    return -1;

  accounthighcount++;
  setAccountByIndex(accounthighcount);

  setpppdArgumentDefaults();

  return caccount;
}

int PPPData::copyaccount(int i) {

  config->setGroup(caccountgroup);

  if(accounthighcount >= MAX_ACCOUNTS)
    return -1;

  setAccountByIndex(i);

  QMap <QString, QString> map = config->entryMap(caccountgroup);
  QMap <QString, QString>::ConstIterator it = map.begin();

  QString newname = i18n("%1_copy").arg(accname());

  newaccount();

  while (it != map.end()) {
    config->setGroup(caccountgroup);
    config->writeEntry(it.key(), *it);
    it++;
  }

  setAccname(newname);

  return caccount;
}


const QString PPPData::accname() {
  return readConfig(caccountgroup, ACC_NAME_KEY);
}

void PPPData::setAccname(const QString &n) {
  if(!caccountgroup.isNull()) {
    // are we manipulating the default account's name ? then change it, too.
    bool def = accname() == defaultAccount();
    writeConfig(caccountgroup, ACC_NAME_KEY, n);
    if (def)
      setDefaultAccount(n);
  }
}


#define SEPARATOR_CHAR ':'
QStringList &PPPData::phonenumbers() {

  readListConfig(caccountgroup, PHONENUMBER_KEY, phonelist, SEPARATOR_CHAR);
  return phonelist;

}


const QString PPPData::phonenumber() {
  return readConfig(caccountgroup, PHONENUMBER_KEY);
}


void PPPData::setPhonenumber(const QString &n) {
  writeConfig(caccountgroup, PHONENUMBER_KEY, n);
}


const QString PPPData::dialPrefix() {
  return readConfig(caccountgroup, DIAL_PREFIX_KEY, "");
}


void PPPData::setDialPrefix(const QString &s) {
  writeConfig(caccountgroup, DIAL_PREFIX_KEY, s);
}


int PPPData::authMethod() {
    return readNumConfig(caccountgroup, AUTH_KEY, 0);
}


void PPPData::setAuthMethod(int value) {
  writeConfig(caccountgroup, AUTH_KEY, value);
}


const QString  PPPData::storedUsername() {
  return readConfig(caccountgroup, STORED_USERNAME_KEY, "");
}


void PPPData::setStoredUsername(const QString &b) {
  writeConfig(caccountgroup, STORED_USERNAME_KEY, b);
}


const QString  PPPData::storedPassword() {
  return readConfig(caccountgroup, STORED_PASSWORD_KEY, "");
}


void PPPData::setStoredPassword(const QString &b) {
  writeConfig(caccountgroup, STORED_PASSWORD_KEY, b);
}


bool PPPData::storePassword() {
  return (bool)readNumConfig(caccountgroup, STORE_PASSWORD_KEY, 1);
}

int PPPData::callbackType() {
  return readNumConfig(caccountgroup, CALLBACK_TYPE_KEY, 0);
}

void PPPData::setCallbackType(int value) {
  writeConfig(caccountgroup, CALLBACK_TYPE_KEY, value);
}

QString PPPData::callbackPhone() {
  return readConfig(caccountgroup, CALLBACK_PHONE_KEY, "");
}

void PPPData::setCallbackPhone(const QString &b) {
  writeConfig(caccountgroup, CALLBACK_PHONE_KEY, b);
}

bool PPPData::waitCallback() {
  return waitcallback;
}

void PPPData::setWaitCallback(bool value) {
  waitcallback = value;
}

const QString PPPData::command_before_connect() {
  return readConfig(caccountgroup, BEFORE_CONNECT_KEY);
}


void PPPData::setCommand_before_connect(const QString &n) {
  writeConfig(caccountgroup, BEFORE_CONNECT_KEY, n);
}


void PPPData::setStorePassword(bool b) {
  writeConfig(caccountgroup, STORE_PASSWORD_KEY, (int)b);
}


const QString PPPData::command_on_connect() {
  return readConfig(caccountgroup, COMMAND_KEY);
}


void PPPData::setCommand_on_connect(const QString &n) {
  writeConfig(caccountgroup, COMMAND_KEY, n);
}


const QString PPPData::command_on_disconnect() {
  return readConfig(caccountgroup, DISCONNECT_COMMAND_KEY);
}


void PPPData::setCommand_on_disconnect(const QString &n) {
  writeConfig(caccountgroup, DISCONNECT_COMMAND_KEY, n);
}


const QString PPPData::command_before_disconnect() {
  return readConfig(caccountgroup, BEFORE_DISCONNECT_KEY);
}


void PPPData::setCommand_before_disconnect(const QString &n) {
  writeConfig(caccountgroup, BEFORE_DISCONNECT_KEY, n);
}


const QString PPPData::ipaddr() {
  return readConfig(caccountgroup, IPADDR_KEY);
}


void PPPData::setIpaddr(const QString &n) {
  writeConfig(caccountgroup, IPADDR_KEY, n);
}


const QString PPPData::subnetmask() {
  return readConfig(caccountgroup, SUBNETMASK_KEY);
}


void PPPData::setSubnetmask(const QString &n) {
  writeConfig(caccountgroup, SUBNETMASK_KEY, n);
}


bool PPPData::autoname() {
  return (bool) readNumConfig(caccountgroup, AUTONAME_KEY, false);
}


void PPPData::setAutoname(bool set) {
  writeConfig(caccountgroup, AUTONAME_KEY, (int) set);
}


bool PPPData::AcctEnabled() {
  return (bool) readNumConfig(caccountgroup, ACCTENABLED_KEY, false);
}


void PPPData::setAcctEnabled(bool set) {
  writeConfig(caccountgroup, ACCTENABLED_KEY, (int) set);
}


int PPPData::VolAcctEnabled() {
  return readNumConfig(caccountgroup, VOLACCTENABLED_KEY, 0);
}


void PPPData::setVolAcctEnabled(int set) {
  writeConfig(caccountgroup, VOLACCTENABLED_KEY, set);
}


const QString PPPData::gateway() {
  return readConfig(caccountgroup, GATEWAY_KEY);
}


void PPPData::setGateway(const QString &n ) {
  writeConfig(caccountgroup, GATEWAY_KEY, n);
}


bool PPPData::defaultroute() {
  // default route is by default 'on'.
  return (bool) readNumConfig(caccountgroup, DEFAULTROUTE_KEY, true);
}


void PPPData::setDefaultroute(bool set) {
  writeConfig(caccountgroup, DEFAULTROUTE_KEY, (int) set);
}


bool PPPData::autoDNS() {
  bool set = (bool) readNumConfig(caccountgroup, AUTODNS_KEY, true);
  return (set && gpppdata.pppdVersionMin(2, 3, 7));
}


void PPPData::setAutoDNS(bool set) {
  writeConfig(caccountgroup, AUTODNS_KEY, (int) set);
}


void PPPData::setExDNSDisabled(bool set) {
  writeConfig(caccountgroup, EXDNSDISABLED_KEY, (int) set);
}


bool PPPData::exDNSDisabled() {
  return (bool) readNumConfig(caccountgroup, EXDNSDISABLED_KEY,0);
}


QStringList &PPPData::dns() {
  static QStringList dnslist;

  readListConfig(caccountgroup, DNS_KEY, dnslist);
  while(dnslist.count() > MAX_DNS_ENTRIES)
    dnslist.remove(dnslist.last());

  return dnslist;
}


void PPPData::setDns(QStringList &list) {
  writeListConfig(caccountgroup, DNS_KEY, list);
}


const QString PPPData::domain() {
  return readConfig(caccountgroup, DOMAIN_KEY);
}


void PPPData::setDomain(const QString &n ) {
  writeConfig(caccountgroup, DOMAIN_KEY, n);
}


QStringList &PPPData::scriptType() {
  static QStringList typelist;

  readListConfig(caccountgroup, SCRIPTCOM_KEY, typelist);
  while(typelist.count() > MAX_SCRIPT_ENTRIES)
    typelist.remove(typelist.last());

  return typelist;
}


void PPPData::setScriptType(QStringList &list) {
  writeListConfig(caccountgroup, SCRIPTCOM_KEY, list);
}


QStringList &PPPData::script() {
  static QStringList scriptlist;

  readListConfig(caccountgroup, SCRIPTARG_KEY, scriptlist);
  while(scriptlist.count() > MAX_SCRIPT_ENTRIES)
    scriptlist.remove(scriptlist.last());

  return scriptlist;
}


void PPPData::setScript(QStringList &list) {
  writeListConfig(caccountgroup, SCRIPTARG_KEY, list);
}


const QString PPPData::accountingFile() {
  return readConfig(caccountgroup, ACCTFILE_KEY);
}


void PPPData::setAccountingFile(const QString &n) {
  writeConfig(caccountgroup, ACCTFILE_KEY, n);
}


const QString PPPData::totalCosts() {
  return readConfig(caccountgroup, TOTALCOSTS_KEY);
}


void PPPData::setTotalCosts(const QString &n) {
  writeConfig(caccountgroup, TOTALCOSTS_KEY, n);
}


int PPPData::totalBytes() {
  return readNumConfig(caccountgroup, TOTALBYTES_KEY, 0);
}

void PPPData::setTotalBytes(int n) {
  writeConfig(caccountgroup, TOTALBYTES_KEY, n);
}


QStringList &PPPData::pppdArgument() {
  static QStringList arglist;

  while(arglist.count() > MAX_PPPD_ARGUMENTS)
    arglist.remove(arglist.last());
  readListConfig(caccountgroup, PPPDARG_KEY, arglist);

  return arglist;
}


void PPPData::setpppdArgument(QStringList &args) {
  writeListConfig(caccountgroup, PPPDARG_KEY, args);
}


void PPPData::setpppdArgumentDefaults() {
  QStringList arg;
  setpppdArgument(arg);
}


// graphing widget
void PPPData::setGraphingOptions(bool enable,
				 QColor bg,
				 QColor text,
				 QColor in,
				 QColor out)
{
  if(config) {
    config->setGroup(GRAPH_GRP);
    config->writeEntry(GENABLED, enable);
    config->writeEntry(GCOLOR_BG, bg);
    config->writeEntry(GCOLOR_TEXT, text);
    config->writeEntry(GCOLOR_IN, in);
    config->writeEntry(GCOLOR_OUT, out);
  }
}

void PPPData::graphingOptions(bool &enable,
			      QColor &bg,
			      QColor &text,
			      QColor &in,
			      QColor &out)
{
  QColor c;

  if(config) {
    config->setGroup(GRAPH_GRP);
    enable = config->readBoolEntry(GENABLED, true);
    c = Qt::white;
    bg = config->readColorEntry(GCOLOR_BG, &c);
    c = Qt::black;
    text = config->readColorEntry(GCOLOR_TEXT, &c);
    c = Qt::blue;
    in = config->readColorEntry(GCOLOR_IN, &c);
    c = Qt::red;
    out = config->readColorEntry(GCOLOR_OUT, &c);
  }
}


bool PPPData::graphingEnabled() {
  if(config) {
    config->setGroup(GRAPH_GRP);
    return config->readBoolEntry(GENABLED, true);
  }
  else return true;
}



//
//functions to change/set the child pppd process info
//
bool PPPData::pppdRunning() const {
  return pppdisrunning;
}

void PPPData::setpppdRunning(bool set) {
  pppdisrunning = set;
}

int PPPData::pppdError() const {
  return pppderror;
}

void PPPData::setpppdError(int err) {
  pppderror = err;
}


//
// window position
//
void PPPData::winPosConWin(int& p_x, int& p_y) {
  QRect desk = KGlobalSettings::splashScreenDesktopGeometry();
  p_x = readNumConfig(WINPOS_GRP, WINPOS_CONWIN_X, desk.center().x()-160);
  p_y = readNumConfig(WINPOS_GRP, WINPOS_CONWIN_Y, desk.center().y()-55);
}

void PPPData::setWinPosConWin(int p_x, int p_y) {
  writeConfig(WINPOS_GRP, WINPOS_CONWIN_X, p_x);
  writeConfig(WINPOS_GRP, WINPOS_CONWIN_Y, p_y);
}

void PPPData::winPosStatWin(int& p_x, int& p_y) {
  QRect desk = KGlobalSettings::splashScreenDesktopGeometry();
  p_x = readNumConfig(WINPOS_GRP, WINPOS_STATWIN_X, desk.center().x()-160);
  p_y = readNumConfig(WINPOS_GRP, WINPOS_STATWIN_Y, desk.center().y()-55);
}

void PPPData::setWinPosStatWin(int p_x, int p_y) {
  writeConfig(WINPOS_GRP, WINPOS_STATWIN_X, p_x);
  writeConfig(WINPOS_GRP, WINPOS_STATWIN_Y, p_y);
}
