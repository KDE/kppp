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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qmap.h>
#include "pppdata.h"
#include "runtests.h"
#include "kpppconfig.h"
#include "devices.h"
#include <kapp.h>
#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>

PPPData gpppdata;


PPPData::PPPData()
  :  config(0L),
     highcount(-1),        // start out with no entries
     caccount(-1),         // set the current account index also
     suidprocessid(-1),    // process ID of setuid child 
     pppdisrunning(false),
     pppderror(0)       
{
}


//
// open configuration file 
//
bool PPPData::open() {

  config = kapp->getConfig();

  if (config->getConfigState() == KConfig::NoAccess) {
    KMessageBox::error(0L,
                       i18n("The application-specific config file could "
                       "not be\nopened neither read-write nor read-only.\n\n"
                       "The superuser might have to change its ownership\n"
                       "by issuing the following command in your home directory:\n\n"
                       "chown {YourUsername} .kde/share/config/kppprc"),
			 kapp->name());
    return false;
  }

  // don't expand shell variables
  config->setDollarExpansion(false);

  highcount = readNumConfig(GENERAL_GRP, NUMACCOUNTS_KEY, 0) - 1;

  if (highcount > MAX_ACCOUNTS)
    highcount = MAX_ACCOUNTS;

  if(highcount >= 0) {
    if(defaultAccount().isEmpty()) == 0) {
      setAccountbyIndex(0);
      setDefaultAccount(accname());
    }
  }

  // start out with internal debugging disabled
  // the user is still free to specify `debug' on his own
  setPPPDebug(false);

  return true;      
}


//
// save configuration
//
void PPPData::save() {

  if (config) {
    writeConfig(GENERAL_GRP, NUMACCOUNTS_KEY, count());
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
    if (!config->hasKey(key))
      config->writeEntry(key, QString(defvalue));
    return config->readEntry(key);
  } else
    return defvalue;
  
}


int PPPData::readNumConfig(const QString &group, const QString &key,
			   int defvalue) {
  if (config) {
    config->setGroup(group);
    
    // make default entry
    if (!config->hasKey(key)) 
      config->writeEntry(key, defvalue);
    return config->readNumEntry(key);
  } else
    return defvalue;

}


bool PPPData::readListConfig(const QString &group, const QString &key,
                             QStrList &list, char sep) {
  list.clear();
  if (config) {
    config->setGroup(group);
    config->readListEntry(key, list, sep);
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
                              QStrList &list, char sep) {
  if (config) {
    config->setGroup(group);
    config->writeEntry(key, list, sep);
  }
}


//
// functions to set/return general information
//
const QString PPPData::Password() {
  return password;
}


void PPPData::setPassword(const QString &pw) {
  password = pw;
}


const QString PPPData::defaultAccount() {
  return readConfig(GENERAL_GRP, DEFAULTACCOUNT_KEY);
}


void PPPData::setDefaultAccount(const QString &n) {
  writeConfig(GENERAL_GRP, DEFAULTACCOUNT_KEY, n);
    
  //now set the current account index to the default account
  setAccount(defaultAccount());
}


const bool PPPData::get_show_clock_on_caption() {
  return (bool) readNumConfig(GENERAL_GRP, SHOWCLOCK_KEY, true);
}


void PPPData::set_show_clock_on_caption(bool set) {
  writeConfig(GENERAL_GRP, SHOWCLOCK_KEY, (int) set);
}


const bool PPPData::get_xserver_exit_disconnect() {
  return (bool) readNumConfig(GENERAL_GRP, DISCONNECT_KEY, true);
}


void PPPData::setPPPDebug(bool set) {
  writeConfig(GENERAL_GRP, PPP_DEBUG_OPTION, (int)set);
}


const bool PPPData::getPPPDebug() {
  return (bool)readNumConfig(GENERAL_GRP, PPP_DEBUG_OPTION, false);
}


void PPPData::set_xserver_exit_disconnect(bool set) {
  writeConfig(GENERAL_GRP, DISCONNECT_KEY, (int) set);
}


const bool PPPData::quit_on_disconnect() {
  return (bool) readNumConfig(GENERAL_GRP, QUITONDISCONNECT_KEY, false);
}


void PPPData::set_quit_on_disconnect(bool set) {
  writeConfig(GENERAL_GRP, QUITONDISCONNECT_KEY, (int) set);
}


const bool PPPData::get_show_log_window() {
  return (bool) readNumConfig (GENERAL_GRP, SHOWLOGWIN_KEY, false);
}


void PPPData::set_show_log_window(bool set) {
  writeConfig(GENERAL_GRP, SHOWLOGWIN_KEY, (int) set);
}


const bool PPPData::automatic_redial() {
  return (bool) readNumConfig(GENERAL_GRP, AUTOREDIAL_KEY, FALSE);
}


void PPPData::set_automatic_redial(bool set) {
  writeConfig(GENERAL_GRP, AUTOREDIAL_KEY, (int) set);
}


const bool PPPData::get_iconify_on_connect() {
  return (bool) readNumConfig(GENERAL_GRP, ICONIFY_ON_CONNECT_KEY, TRUE);
}


void PPPData::set_iconify_on_connect(bool set) {
  writeConfig(GENERAL_GRP, ICONIFY_ON_CONNECT_KEY, (int) set);
}


const bool PPPData::get_dock_into_panel() {
  return (bool) readNumConfig(GENERAL_GRP, DOCKING_KEY, false);
}


void PPPData::set_dock_into_panel(bool set) {
  writeConfig(GENERAL_GRP, DOCKING_KEY, (int) set);
}


int PPPData::pppdTimeout() {
  return readNumConfig(GENERAL_GRP, PPPDTIMEOUT_KEY, PPPD_TIMEOUT);
}


void PPPData::setpppdTimeout(int n) {
  writeConfig(GENERAL_GRP, PPPDTIMEOUT_KEY, n);
}


const QString PPPData::modemDevice() {
  return readConfig (MODEM_GRP, MODEMDEV_KEY, devices[DEV_DEFAULT]);
}


void PPPData::setModemDevice(const QString &n) {
  writeConfig(MODEM_GRP, MODEMDEV_KEY, n);
}


const QString PPPData::flowcontrol() {
  return readConfig(MODEM_GRP, FLOWCONTROL_KEY, "CRTSCTS");
}


void PPPData::setFlowcontrol(const QString &n) {
  writeConfig(MODEM_GRP, FLOWCONTROL_KEY, n);
}


const QString PPPData::speed() {
  QString s = readConfig(MODEM_GRP, SPEED_KEY, "57600");
  // undo the damage of a bug in former versions. It left an empty Speed=
  // entry in kppprc. kppp did set the serial port to 57600 as default but
  // pppd wouldn't receive the speed via the command line.
  if(s.toUInt() == 0)
    s = "57600";
  return s;
}


void PPPData::setSpeed(const QString &n) {
  writeConfig(MODEM_GRP, SPEED_KEY, n);
}


void PPPData::setUseCDLine(const int n) {
  writeConfig(MODEM_GRP,USECDLINE_KEY,n);
}


int PPPData::UseCDLine() {
  return  readNumConfig(MODEM_GRP,USECDLINE_KEY,0);
}


const QString  PPPData::modemEscapeStr() {
  return readConfig(MODEM_GRP,ESCAPESTR_KEY,"+++");
}


void PPPData::setModemEscapeStr(const QString &n) {
  writeConfig(MODEM_GRP,ESCAPESTR_KEY,n);
}


const QString  PPPData::modemEscapeResp() {
  return readConfig(MODEM_GRP,ESCAPERESP_KEY,"OK");
}


void PPPData::setModemEscapeResp(const QString &n) {
  writeConfig(MODEM_GRP,ESCAPERESP_KEY,n);
}


int  PPPData::modemEscapeGuardTime() {
  return readNumConfig(MODEM_GRP,ESCAPEGUARDTIME_KEY,50);
}


void PPPData::setModemEscapeGuardTime(int n) {
  writeConfig(MODEM_GRP,ESCAPEGUARDTIME_KEY,n);
}


const bool PPPData::modemLockFile() {
  return readNumConfig(MODEM_GRP, LOCKFILE_KEY, 1);
}


void PPPData::setModemLockFile(bool set) {
  writeConfig(MODEM_GRP, LOCKFILE_KEY, set);
}


int PPPData::modemTimeout() {
  return readNumConfig(MODEM_GRP, TIMEOUT_KEY, MODEM_TIMEOUT);
}


void PPPData::setModemTimeout(int n) {
  writeConfig(MODEM_GRP, TIMEOUT_KEY, n);
}


int PPPData::busyWait() {
  return readNumConfig(MODEM_GRP, BUSYWAIT_KEY, BUSY_WAIT);
}


void PPPData::setbusyWait(int n) {
  writeConfig(MODEM_GRP, BUSYWAIT_KEY, n);
}


//
//Advanced "Modem" dialog
//
const QString PPPData::modemInitStr() {
  return readConfig(MODEM_GRP, INITSTR_KEY, "ATZ");
}


void PPPData::setModemInitStr(const QString &n) {
  writeConfig(MODEM_GRP, INITSTR_KEY, n);
} 


const QString PPPData::modemInitResp() {
  return readConfig(MODEM_GRP, INITRESP_KEY, "OK");
}


void PPPData::setModemInitResp(const QString &n) {
  writeConfig(MODEM_GRP, INITRESP_KEY, n);
} 


const int PPPData::modemPreInitDelay() {
  return readNumConfig(MODEM_GRP, PREINITDELAY_KEY, 50);
}


void PPPData::setModemPreInitDelay(const int n) {
  writeConfig(MODEM_GRP, PREINITDELAY_KEY, n);
}


const int PPPData::modemInitDelay() {
  return readNumConfig(MODEM_GRP, INITDELAY_KEY, 50);
}


void PPPData::setModemInitDelay(const int n) {
  writeConfig(MODEM_GRP, INITDELAY_KEY, n);
}


const QString PPPData::modemDialStr() {
  return readConfig(MODEM_GRP, DIALSTR_KEY, "ATDT");
}


void PPPData::setModemDialStr(const QString &n) {
  writeConfig(MODEM_GRP, DIALSTR_KEY, n);
} 


const QString PPPData::modemConnectResp() {
  return readConfig(MODEM_GRP, CONNECTRESP_KEY, "CONNECT");
}


void PPPData::setModemConnectResp(const QString &n) {
  writeConfig(MODEM_GRP, CONNECTRESP_KEY, n);
}


const QString PPPData::modemBusyResp() {
  return readConfig(MODEM_GRP, BUSYRESP_KEY, "BUSY");
}


void PPPData::setModemBusyResp(const QString &n) {
  writeConfig(MODEM_GRP, BUSYRESP_KEY, n);
}


const QString PPPData::modemNoCarrierResp() {
  return readConfig(MODEM_GRP, NOCARRIERRESP_KEY, "NO CARRIER");
}


void PPPData::setModemNoCarrierResp(const QString &n) {
  writeConfig(MODEM_GRP, NOCARRIERRESP_KEY, n);
}


const QString PPPData::modemNoDialtoneResp() {
  return readConfig(MODEM_GRP, NODIALTONERESP_KEY, "NO DIALTONE");
}


void PPPData::setModemNoDialtoneResp(const QString &n) {
  writeConfig(MODEM_GRP, NODIALTONERESP_KEY, n);
}


const QString PPPData::modemHangupStr() {
  return readConfig(MODEM_GRP, HANGUPSTR_KEY, "+++ATH");
}

void PPPData::setModemHangupStr(const QString &n) {
  writeConfig(MODEM_GRP, HANGUPSTR_KEY, n);
} 


const QString PPPData::modemHangupResp() {
  return readConfig(MODEM_GRP, HANGUPRESP_KEY, "OK");
}

void PPPData::setModemHangupResp(const QString &n) {
  writeConfig(MODEM_GRP, HANGUPRESP_KEY, n);
}


const QString PPPData::modemAnswerStr() {
  return readConfig(MODEM_GRP, ANSWERSTR_KEY, "ATA");
}


const QString PPPData::volumeOff() {
  return readConfig(MODEM_GRP, VOLUME_OFF, "M0L0");
}


void PPPData::setVolumeOff(const QString &s) {
  writeConfig(MODEM_GRP, VOLUME_OFF, s);
}


const QString PPPData::volumeMedium() {
 return readConfig(MODEM_GRP, VOLUME_MEDIUM, "M1L1");
}


void PPPData::setVolumeMedium(const QString &s) {
  writeConfig(MODEM_GRP, VOLUME_MEDIUM, s);
}


const QString PPPData::volumeHigh() {
  return readConfig(MODEM_GRP, VOLUME_HIGH, "M1L4");
}


void PPPData::setVolumeHigh(const QString &s) {
 writeConfig(MODEM_GRP, VOLUME_HIGH, s);
}


const QString PPPData::volumeInitString() {
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
  return readNumConfig(MODEM_GRP, VOLUME_KEY, 0);
}


void PPPData::setVolume(int i) {
  writeConfig(MODEM_GRP, VOLUME_KEY, i);
}


void PPPData::setModemAnswerStr(const QString &n) {
  writeConfig(MODEM_GRP, ANSWERSTR_KEY, n);
} 


const QString PPPData::modemRingResp() {
  return readConfig(MODEM_GRP, RINGRESP_KEY, "RING");
}


void PPPData::setModemRingResp(const QString &n) {
  writeConfig(MODEM_GRP, RINGRESP_KEY, n);
}


const QString PPPData::modemAnswerResp() {
  return readConfig(MODEM_GRP, ANSWERRESP_KEY, "CONNECT");
}


void PPPData::setModemAnswerResp(const QString &n) {
  writeConfig(MODEM_GRP, ANSWERRESP_KEY, n);
}


const QString PPPData::enter() {
  return readConfig(MODEM_GRP, ENTER_KEY, "CR/LF");
}


void PPPData::setEnter(const QString &n) {
  writeConfig(MODEM_GRP, ENTER_KEY, n);
}


//
// functions to set/return account information
//

//returns number of accounts
int PPPData::count() const {
  return highcount + 1;
}


bool PPPData::setAccount(const QString &aname) {
  for(int i = 0; i <= highcount; i++) {
    setAccountbyIndex(i);
    if(accname() == aname) {
      caccount = i;
      return true;
    }
  }
  return false;
}


bool PPPData::setAccountbyIndex(int i) {
  if(i >= 0 && i <= highcount) {
    caccount = i;
    cgroup.sprintf("%s%i", ACCOUNT_GRP, i); 
    return true;
  }
  return false;
}


bool PPPData::isUniqueAccname(const QString &n) {
  int current = caccount;
  for(int i=0; i <= highcount; i++) {
    setAccountbyIndex(i);
    if(accname() == n && i != current) {
      setAccountbyIndex(current);
      return false;
    }
  }
  setAccountbyIndex(current);
  return true;
}


bool PPPData::deleteAccount() {
  if(caccount < 0) 
    return false;

  QMap <QString, QString> map;
  QMap <QString, QString>::Iterator it;

  // set all entries of the current account to "" 
  map = config->entryMap(cgroup);
  it = map.begin();
  while (it != map.end()) {
    config->writeEntry(it.key(), "");
    it++;
  }

  // shift the succeeding accounts
  for(int i = caccount+1; i <= highcount; i++) {
    setAccountbyIndex(i);
    map = config->entryMap(cgroup);
    it = map.begin();
    setAccountbyIndex(i-1);
    config->setGroup(cgroup);
    while (it != map.end()) {
      config->writeEntry(it.key(), *it);
      it++;
    }
  }

  // make sure the top account is cleared
  setAccountbyIndex(highcount);
  map = config->entryMap(cgroup);
  it = map.begin();
  config->setGroup(cgroup);
  while (it.key() != QString::null) {
    config->writeEntry(it.key(), "");
    it++;
  }

  highcount--;
  if(caccount > highcount)
    caccount = highcount;
  
  setAccountbyIndex(caccount);
  
  return true;
} 


bool PPPData::deleteAccount(const QString &aname) {
  if(!setAccount(aname))
    return false;

  deleteAccount();

  return true;
}


int PPPData::newaccount() {

  if(!config || highcount >= MAX_ACCOUNTS)
    return -1;
  
  highcount++;
  setAccountbyIndex(highcount);

  setpppdArgumentDefaults();

  return caccount;
}

int PPPData::copyaccount(int i) {

  if(highcount >= MAX_ACCOUNTS)
    return -1;
  
  setAccountbyIndex(i);

  QMap <QString, QString> map = config->entryMap(cgroup);
  QMap <QString, QString>::Iterator it = map.begin();

  QString newname = accname();
  newname += "_copy";

  newaccount();

  while (it != map.end()) {
    config->writeEntry(it.key(), *it);
    it++;
  }

  setAccname(newname);

  return caccount;
}


QString PPPData::accname() {
  return readConfig(cgroup, NAME_KEY);
}

void PPPData::setAccname(const QString &n) {
  if(cgroup) {
    //change the default account name along with the account name
    if(accname == defaultAccount)
      setDefaultAccount(n);
    writeConfig(cgroup, NAME_KEY, n);
  }
}


#define SEPARATOR_CHAR ':'
QStrList &PPPData::phonenumbers() {

  readListConfig(cgroup, PHONENUMBER_KEY, phonelist, SEPARATOR_CHAR);
  return phonelist;

}


const QString PPPData::phonenumber() {
  return readConfig(cgroup, PHONENUMBER_KEY);
}


void PPPData::setPhonenumber(const QString &n) {
  writeConfig(cgroup, PHONENUMBER_KEY, n);	
}


const QString PPPData::dialPrefix() {
  return readConfig(cgroup, DIAL_PREFIX_KEY, "");
}


void PPPData::setDialPrefix(const QString &s) {
  writeConfig(cgroup, DIAL_PREFIX_KEY, s);
}


const int PPPData::authMethod() {
    return readNumConfig(cgroup, AUTH_KEY, 0);
}


void PPPData::setAuthMethod(int value) {
  writeConfig(cgroup, AUTH_KEY, value);
}


const QString  PPPData::storedUsername() {
  return readConfig(cgroup, STORED_USERNAME_KEY, "");
}


void PPPData::setStoredUsername(const QString &b) {
  writeConfig(cgroup, STORED_USERNAME_KEY, b);
}


const QString  PPPData::storedPassword() {
  return readConfig(cgroup, STORED_PASSWORD_KEY, "");
}


void PPPData::setStoredPassword(const QString &b) {
  writeConfig(cgroup, STORED_PASSWORD_KEY, b);
}


const bool PPPData::storePassword() {
  return (bool)readNumConfig(cgroup, STORE_PASSWORD_KEY, 1);
}


const QString PPPData::command_before_connect() {
  return readConfig(cgroup, BEFORE_CONNECT_KEY);
}


void PPPData::setCommand_before_connect(const QString &n) {
  writeConfig(cgroup, BEFORE_CONNECT_KEY, n);
}


void PPPData::setStorePassword(bool b) {
  writeConfig(cgroup, STORE_PASSWORD_KEY, (int)b);
}


const QString PPPData::command_on_connect() {
  return readConfig(cgroup, COMMAND_KEY);
}


void PPPData::setCommand_on_connect(const QString &n) {
  writeConfig(cgroup, COMMAND_KEY, n);
}


const QString PPPData::command_on_disconnect() {
  return readConfig(cgroup, DISCONNECT_COMMAND_KEY);
}


void PPPData::setCommand_on_disconnect(const QString &n) {
  writeConfig(cgroup, DISCONNECT_COMMAND_KEY, n);
}


const QString PPPData::command_before_disconnect() {
  return readConfig(cgroup, BEFORE_DISCONNECT_KEY);
}


void PPPData::setCommand_before_disconnect(const QString &n) {
  writeConfig(cgroup, BEFORE_DISCONNECT_KEY, n);
}


const QString PPPData::ipaddr() {
  return readConfig(cgroup, IPADDR_KEY);
}


void PPPData::setIpaddr(const QString &n) {
  writeConfig(cgroup, IPADDR_KEY, n);
}


const QString PPPData::subnetmask() {
  return readConfig(cgroup, SUBNETMASK_KEY);
}


void PPPData::setSubnetmask(const QString &n) {
  writeConfig(cgroup, SUBNETMASK_KEY, n);
}


const bool PPPData::autoname() {
  return (bool) readNumConfig(cgroup, AUTONAME_KEY, false);
}


void PPPData::setAutoname(bool set) {
  writeConfig(cgroup, AUTONAME_KEY, (int) set);
}


const bool PPPData::AcctEnabled() {
  return (bool) readNumConfig(cgroup, ACCTENABLED_KEY, false);
}


void PPPData::setAcctEnabled(bool set) {
  writeConfig(cgroup, ACCTENABLED_KEY, (int) set);
}


const int PPPData::VolAcctEnabled() {
  return readNumConfig(cgroup, VOLACCTENABLED_KEY, 0);
}


void PPPData::setVolAcctEnabled(int set) {
  writeConfig(cgroup, VOLACCTENABLED_KEY, set);
}


const QString PPPData::gateway() {
  return readConfig(cgroup, GATEWAY_KEY);
}


void PPPData::setGateway(const QString &n ) {
  writeConfig(cgroup, GATEWAY_KEY, n);
}


const bool PPPData::defaultroute() {
  // default route is by default 'on'.
  return (bool) readNumConfig(cgroup, DEFAULTROUTE_KEY, true);  
}


void PPPData::setDefaultroute(bool set) {
  writeConfig(cgroup, DEFAULTROUTE_KEY, (int) set);
}


void PPPData::setExDNSDisabled(bool set) {
  writeConfig(cgroup, EXDNSDISABLED_KEY, (int) set);
}


const bool PPPData::exDNSDisabled() {
  return (bool) readNumConfig(cgroup, EXDNSDISABLED_KEY,0);
}


QStrList &PPPData::dns() {
  static QStrList dnslist;
  
  readListConfig(cgroup, DNS_KEY, dnslist);
  while(dnslist.count() > MAX_DNS_ENTRIES)
    dnslist.removeLast();

  return dnslist;
}


void PPPData::setDns(QStrList &list) {
  writeListConfig(cgroup, DNS_KEY, list);
}


const QString PPPData::domain() {
  return readConfig(cgroup, DOMAIN_KEY);
}


void PPPData::setDomain(const QString &n ) {
  writeConfig(cgroup, DOMAIN_KEY, n);
}


QStrList &PPPData::scriptType() {
  static QStrList typelist;
  
  readListConfig(cgroup, SCRIPTCOM_KEY, typelist);
  while(typelist.count() > MAX_SCRIPT_ENTRIES)
    typelist.removeLast();

  return typelist;
}


void PPPData::setScriptType(QStrList &list) {
  writeListConfig(cgroup, SCRIPTCOM_KEY, list);
}


QStrList &PPPData::script() {
  static QStrList scriptlist;
  
  readListConfig(cgroup, SCRIPTARG_KEY, scriptlist);
  while(scriptlist.count() > MAX_SCRIPT_ENTRIES)
    scriptlist.removeLast();

  return scriptlist;
}


void PPPData::setScript(QStrList &list) {
  writeListConfig(cgroup, SCRIPTARG_KEY, list);
}


const QString PPPData::accountingFile() {
  return readConfig(cgroup, ACCTFILE_KEY);
}


void PPPData::setAccountingFile(const QString &n) {
  writeConfig(cgroup, ACCTFILE_KEY, n);
}


const QString PPPData::totalCosts() {
  return readConfig(cgroup, TOTALCOSTS_KEY);
}


void PPPData::setTotalCosts(const QString &n) {
  writeConfig(cgroup, TOTALCOSTS_KEY, n);
}


int PPPData::totalBytes() {
  return readNumConfig(cgroup, TOTALBYTES_KEY, 0);
}

void PPPData::setTotalBytes(int n) {
  writeConfig(cgroup, TOTALBYTES_KEY, n);
}


QStrList &PPPData::pppdArgument() {
  static QStrList arglist;
  
  while(arglist.count() > MAX_PPPD_ARGUMENTS)
    arglist.removeLast();
  readListConfig(cgroup, PPPDARG_KEY, arglist);

  return arglist;
}


void PPPData::setpppdArgument(QStrList &args) {
  writeListConfig(cgroup, PPPDARG_KEY, args);
}


void PPPData::setpppdArgumentDefaults() {
  QStrList arg;
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
  return (bool) readNumConfig(GRAPH_GRP, GENABLED, true);
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

pid_t PPPData::suidChildPid() const {
  return suidprocessid;
}

void PPPData::setSuidChildPid(pid_t id) {
  suidprocessid = id;
}

int PPPData::pppdError() const {
  return pppderror;
}

void PPPData::setpppdError(int err) {
  pppderror = err;
}
