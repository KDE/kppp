/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
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

#include <qlayout.h>
#include <kquickhelp.h>
#include <qregexp.h>
#include <kapp.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kbuttonbox.h>
#include <klocale.h>
#include <kconfig.h>
#include <kwm.h>

#include "edit.h"
#include "pppdata.h"
#include "termios.h"
#include "newwidget.h"
#include "iplined.h"

DialWidget::DialWidget( QWidget *parent, bool isnewaccount, const char *name )
  : KGroupBox(i18n("Dial Setup"), parent, name)
{
  const int GRIDROWS = 6;

  QGridLayout *tl = new QGridLayout(peer(), GRIDROWS, 2, 10, 10);

  connect_label = new QLabel(i18n("Connection Name:"), peer());
  tl->addWidget(connect_label, 0, 0);

  connectname_l = new QLineEdit(peer());
  connectname_l->setMaxLength(ACCNAME_SIZE);
  tl->addWidget(connectname_l, 0, 1);
  KQuickHelp::add(connect_label,
  KQuickHelp::add(connectname_l,
		  i18n("Type in a unique name for this connection")));

  
  number_label = new QLabel(i18n("Phone Number:"), peer());
  number_label->setAlignment(AlignTop|AlignLeft);
  tl->addWidget(number_label, 1, 0);  

  QHBoxLayout *lpn = new QHBoxLayout(5);  
  tl->addLayout(lpn, 1, 1);
  numbers = new QListBox(peer());
  numbers->setMinimumSize(120, 70);
  lpn->addWidget(numbers);
  QVBoxLayout *lpn1 = new QVBoxLayout;
  lpn->addLayout(lpn1);
  add = new QPushButton(i18n("Add"), peer());
  del = new QPushButton(i18n("Remove"), peer());

  up = new QPushButton(peer());
  QPixmap pm = KGlobal::iconLoader()->loadIcon("up.xpm");
  up->setPixmap(pm);
  down = new QPushButton(peer());
  pm = KGlobal::iconLoader()->loadIcon("down.xpm");
  down->setPixmap(pm);
  lpn1->addWidget(add);
  lpn1->addWidget(del);
  lpn1->addStretch(1);
  lpn1->addWidget(up);
  lpn1->addWidget(down);
  connect(add, SIGNAL(clicked()),
	  this, SLOT(addNumber()));
  connect(del, SIGNAL(clicked()),
	  this, SLOT(delNumber()));
  connect(up, SIGNAL(clicked()),
	  this, SLOT(upNumber()));
  connect(down, SIGNAL(clicked()),
	  this, SLOT(downNumber()));
  connect(numbers, SIGNAL(highlighted(int)),
	  this, SLOT(selectionChanged(int)));
  numbersChanged();
  
  KQuickHelp::add(number_label,
  KQuickHelp::add(numbers,
		  i18n("Specifies the phone numbers to dial. You\n"
		       "can supply multiple numbers here, simply\n"
		       "click on \"Add\". You can arrange the\n"
		       "order the numbers are tried by using the\n"
		       "arrow buttons.\n\n"
		       "When a number is busy or fails, <i>kppp</i>will \n"
		       "try the next number and so on")));
  
  auth_l = new QLabel(i18n("Authentication:"), peer());
  tl->addWidget(auth_l, 3, 0);

  auth = new QComboBox(peer());
  auth->insertItem(i18n("Script-based"));
  auth->insertItem(i18n("PAP"));
  auth->insertItem(i18n("Terminal-based"));
  auth->insertItem(i18n("CHAP"));
  tl->addWidget(auth, 3, 1);
  KQuickHelp::add(auth_l,
  KQuickHelp::add(auth,
		  i18n("Specifies the method used to identify yourself to\n"
		       "the PPP server. Most universities still use\n"
		       "<b>Terminal</b>- or <b>Script</b>-based authtentication,\n"
		       "while most ISP use <b>PAP</b> and/or <b>CHAP</b>. If\n"
		       "unsure, contact your ISP.\n"
		       "\n"
		       "If you can choose between PAP and CHAP,\n"
		       "choose CHAP, because it's much safer.")));

  store_password = new QCheckBox(i18n("Store password"), peer());
  store_password->setChecked(true);
  tl->addMultiCellWidget(store_password, 4, 4, 0, 1, AlignRight);
  KQuickHelp::add(store_password,
		  i18n("When this is turned on, your ISP password\n"
		       "will be saved in <i>kppp</i>'s config file, so\n"
		       "you do not need to type it in everytime.\n"
		       "\n"
		       "<b><red>Warning:<black> your password will be stored as\n"
		       "plain text in the config file, which is\n"
		       "readable only to you. Make sure nobody\n"
		       "gains access to this file!"));

  pppdargs = new QPushButton(i18n("Customize pppd arguments..."), peer());
  connect(pppdargs, SIGNAL(clicked()), SLOT(pppdargsbutton()));  
  tl->addMultiCellWidget(pppdargs, 5, 5, 0, 1, AlignCenter);

  // Set defaults if editing an existing connection
  if(!isnewaccount) {
    connectname_l->setText(gpppdata.accname());
     
    // insert the phone numbers into the listbox
    QString n = gpppdata.phonenumber();
    QString tmp = "";
    uint idx = 0;
    while(idx != n.length()) {
      if(n[idx] == ':') {
	if(tmp.length() > 0)
	  numbers->insertItem(tmp);
	tmp = "";
      } else
	tmp += n[idx];
      idx++;
    }
    if(tmp.length() > 0)
      numbers->insertItem(tmp);

    auth->setCurrentItem(gpppdata.authMethod());
    store_password->setChecked(gpppdata.storePassword());
  } else {
    // select PAP as default
    auth->setCurrentItem(1);
  }

  numbersChanged();
  tl->activate();
}


bool DialWidget::save() {
  //first check to make sure that the account name is unique!
  if(strlen(connectname_l->text()) == 0 ||
     !gpppdata.isUniqueAccname(connectname_l->text())) {
    return false;
  } else {
    gpppdata.setAccname(connectname_l->text());

    QString number = "";
    for(uint i = 0; i < numbers->count(); i++) {
      if(i != 0)
	number += ":";
      number += numbers->text(i);
    }

    gpppdata.setPhonenumber(number);
    gpppdata.setAuthMethod(auth->currentItem());
    gpppdata.setStorePassword(store_password->isChecked());
    return true;
  }
}


void DialWidget::numbersChanged() {
  int sel = numbers->currentItem();

  del->setEnabled(sel != -1);
  up->setEnabled(sel != -1 && sel != 0);
  down->setEnabled(sel != -1 && sel != (int)numbers->count()-1); 
}


void DialWidget::selectionChanged(int) {
  numbersChanged();
}


void DialWidget::addNumber() {
  PhoneNumberDialog dlg(this);
  if(dlg.exec()) {
    numbers->insertItem(dlg.phoneNumber());
    numbersChanged();
  }
}


void DialWidget::delNumber() {
  if(numbers->currentItem() != -1) {
    numbers->removeItem(numbers->currentItem());
    numbersChanged();
  }
}


void DialWidget::upNumber() {
  int idx = numbers->currentItem();
  if(idx != -1) {
    QString item = numbers->text(idx);
    numbers->removeItem(idx);
    numbers->insertItem(item, idx-1);
    numbers->setCurrentItem(idx-1);
    numbersChanged();
  }
}


void DialWidget::downNumber() {
  int idx = numbers->currentItem();
  if(idx != -1) {
    QString item = numbers->text(idx);
    numbers->removeItem(idx);
    numbers->insertItem(item, idx+1);
    numbers->setCurrentItem(idx+1);
    numbersChanged();
  }
}


void DialWidget::pppdargsbutton() {
  PPPdArguments pa(this);
  pa.exec();
}



/////////////////////////////////////////////////////////////////////////////
// ExecWidget
/////////////////////////////////////////////////////////////////////////////
ExecWidget::ExecWidget(QWidget *parent, bool isnewaccount, const char *name) :
  KGroupBox(i18n("Execute programs"), parent, name)
{
  QVBoxLayout *tl = new QVBoxLayout(peer(), 10, 10);

  QLabel *l = new QLabel(\
i18n("Here you can select commands to run at certain stages of the\n"
     "connection. The commands are run with your real user id, so\n"
     "you cannot run any commands here requirering root permissions\n"
     "(except if you are root of course).\n\n"
     "Make sure to supply the whole path to the program otherwise\n"
     "kppp might be unable to find it!"), peer());
  l->setMinimumHeight(l->sizeHint().height());
  tl->addWidget(l);
  tl->addStretch(1);

  QGridLayout *l1 = new QGridLayout(4, 2, 10);
  tl->addLayout(l1);
  l1->setColStretch(0, 0);
  l1->setColStretch(1, 1);  

  before_connect_l = new QLabel(i18n("Before connect:"), peer());
  before_connect_l->setAlignment(AlignVCenter);
  l1->addWidget(before_connect_l, 0, 0);
  before_connect = new QLineEdit(peer());
  before_connect->setMaxLength(COMMAND_SIZE);
  l1->addWidget(before_connect, 0, 1);
  KQuickHelp::add(before_connect_l,
  KQuickHelp::add(before_connect,
		  i18n("Allows you to run a program <b>before</b> a connection\n"
		       "is established. It is called immediatly before\n"
		       "dialling is begun.\n\n"
		       "Might be useful, e.g. to stop HylaFAX blocking the\n"
		       "modem.")));

  command_label = new QLabel(i18n("Upon connect:"), peer());
  command_label->setAlignment(AlignVCenter);
  l1->addWidget(command_label, 1, 0);
  command = new QLineEdit(peer());
  command->setMaxLength(COMMAND_SIZE);
  l1->addWidget(command, 1, 1);
  KQuickHelp::add(command_label,
  KQuickHelp::add(command,
		  i18n("Allows you to run a program <b>after</b> a connection\n"
		       "is established. When your program is called, all\n"
		       "all preparations for an internet connection are\n"
		       "finished.\n"
		       "\n"
		       "Very usefull for fetching mail and news")));

  predisconnect_label = new QLabel(i18n("Before disconnect:"),
				 peer());
  predisconnect_label->setAlignment(AlignVCenter);
  l1->addWidget(predisconnect_label, 2, 0);
  predisconnect = new QLineEdit(peer());
  predisconnect->setMaxLength(COMMAND_SIZE);
  l1->addWidget(predisconnect, 2, 1);
  KQuickHelp::add(predisconnect_label,
  KQuickHelp::add(predisconnect,
		  i18n("Allows you to run a program <b>before</b> a connection\n"
		       "is closed. The connection will stay open until\n"
		       "the program exits.")));
		  
  discommand_label = new QLabel(i18n("Upon disconnect:"),
			      peer());
  discommand_label->setAlignment(AlignVCenter);
  l1->addWidget(discommand_label, 3, 0);

  discommand = new QLineEdit(peer());
  discommand->setMaxLength(COMMAND_SIZE);
  l1->addWidget(discommand, 3, 1);
  KQuickHelp::add(discommand_label,
  KQuickHelp::add(discommand,
		  i18n("Allows you to run a program <b>after</b> a connection\n"
		       "has been closed.")));

  // extra space between entries
  l1->addRowSpacing(1, 5);
  l1->addRowSpacing(3, 5);

  tl->addStretch(1);
  tl->activate();

  // Set defaults if editing an existing connection
  if(!isnewaccount) {
    before_connect->setText(gpppdata.command_before_connect());
    command->setText(gpppdata.command_on_connect());
    discommand->setText(gpppdata.command_on_disconnect());
    predisconnect->setText(gpppdata.command_before_disconnect());
  }
}


bool ExecWidget::save() {
  gpppdata.setCommand_before_connect(before_connect->text());
  gpppdata.setCommand_on_connect(command->text());
  gpppdata.setCommand_before_disconnect(predisconnect->text());
  gpppdata.setCommand_on_disconnect(discommand->text());  
  return true;
}



/////////////////////////////////////////////////////////////////////////////
//
// IPWidget
//
/////////////////////////////////////////////////////////////////////////////
IPWidget::IPWidget( QWidget *parent, bool isnewaccount, const char *name )
  : KGroupBox(i18n("IP Setup"), parent, name)
{
  box = new QGroupBox(peer());

  ipaddress_l = new IPLineEdit(peer());

  ipaddress_label = new QLabel(i18n("IP Address:"), peer());
  KQuickHelp::add(ipaddress_label,
  KQuickHelp::add(ipaddress_l,
		  i18n("If your computer has a permanent internet\n"
		       "address, you must supply your IP address here")));

  subnetmask_l = new IPLineEdit(peer());

  sub_label = new QLabel(i18n("Subnet Mask:"), peer());
  KQuickHelp::add(sub_label,
  KQuickHelp::add(subnetmask_l,
		  i18n("If your computer has a static Internet address,\n"
		       "you must supply a network mask here. In almost\n"
		       "all cases this netmask will be <b>255.255.255.0</b>,\n"
		       "but your mileage may vary.\n"
		       "\n"
		       "If unsure, contact your Internet Service Provider")));


  rb = new QButtonGroup(peer());
  rb->hide();
  connect(rb, SIGNAL(clicked(int)), 
	  SLOT(hitIPSelect(int)));  

  dynamicadd_rb = new QRadioButton(peer());
  dynamicadd_rb->setText(i18n("Dynamic IP Address"));
  KQuickHelp::add(dynamicadd_rb,
		  i18n("Select this option when your computer gets an\n"
		       "internet address (IP) everytime a\n"
		       "connection is made.\n"
		       "\n"
		       "Almost every Internet Service Provider uses\n"
		       "this method, so this should be turned on."));

  staticadd_rb = new QRadioButton(peer());
  staticadd_rb->setText(i18n("Static IP Address"));
  rb->insert(dynamicadd_rb, 0);
  rb->insert(staticadd_rb, 1);
  KQuickHelp::add(staticadd_rb,
		  i18n("Select this option when your computer has a\n"
		       "fixed internet address (IP). Most computers\n"
		       "don't have this, so you should probably select\n"
		       "dynamic IP addressing unless you know what you\n"
		       "are doing"));

  autoname = new QCheckBox(i18n("Auto-configure hostname from this IP"), peer());
  autoname->setChecked(gpppdata.autoname());
  connect(autoname,SIGNAL(toggled(bool)),
	  this,SLOT(autoname_t(bool)));
  KQuickHelp::add(autoname,
		  i18n("Whenever you connect, this reconfigures\n"
		       "your hostname to match the IP address you\n"
		       "got from the PPP server. This may be useful\n"
		       "if you need to use a protocol which depends\n"
		       "on this information, but it can also cause several\n"
		       "<link kppp-7.html#autohostname>problems</link>.\n"
		       "\n"
		       "Don't enable this unless you really need it"));

  //load info from gpppdata
  if(!isnewaccount) {
    if(strcmp(gpppdata.ipaddr(),"0.0.0.0")==0 && 
       strcmp(gpppdata.subnetmask(),"0.0.0.0")==0) {
      dynamicadd_rb->setChecked(true);
      hitIPSelect(0);
      autoname->setChecked(gpppdata.autoname());
    }
    else {
      ipaddress_l->setText(gpppdata.ipaddr());
      subnetmask_l->setText(gpppdata.subnetmask());
      staticadd_rb->setChecked(true);
      autoname->setChecked(false);
    }
  }
  else {
    dynamicadd_rb->setChecked(true);
    hitIPSelect(0);
  }

}

void IPWidget::resizeEvent(QResizeEvent *e) {
  KGroupBox::resizeEvent(e);

  // calculate the best with for the frame
  int minw = QMAX(ipaddress_label->sizeHint().width() + 6,
		 sub_label->sizeHint().width() + 6) +
    QMAX(ipaddress_l->sizeHint().width(),
	subnetmask_l->sizeHint().width()) + 10 + 20;

  // egcs wants this to prevent a warning
  int minh = 0;
  minh = 2 * ipaddress_l->sizeHint().height() + 
    fontMetrics().lineSpacing() + 20;

  // resize the frame
  int box_x = (width() - minw)/2;
  int box_y = (height() - minh)/2 - 20;
  box->setGeometry(box_x -15, box_y, minw +30, minh +15);

  // now move the lineedits into the frame
  ipaddress_l->resize(ipaddress_l->sizeHint());
  ipaddress_l->move(box->geometry().right() - ipaddress_l->width() - 15,
		    box_y + fontMetrics().lineSpacing()/2 + 10);
  subnetmask_l->resize(ipaddress_l->sizeHint());
  subnetmask_l->move(ipaddress_l->geometry().left(),
		     ipaddress_l->geometry().bottom() + 10);

  // the labels
  ipaddress_label->resize(ipaddress_label->sizeHint().width(),
			  ipaddress_l->height());
  ipaddress_label->move(box_x + 15, ipaddress_l->geometry().top());		
  sub_label->resize(sub_label->sizeHint().width(),
		    ipaddress_l->height());
  sub_label->move(box_x + 15, subnetmask_l->geometry().top());

  // move the radiobuttons
  staticadd_rb->resize(staticadd_rb->sizeHint());
  dynamicadd_rb->resize(dynamicadd_rb->sizeHint());  
  staticadd_rb->move(box_x + 25, 
		     box_y - staticadd_rb->sizeHint().height()/2);
  dynamicadd_rb->move(box_x + 25,
		      staticadd_rb->geometry().top() - 
		      dynamicadd_rb->sizeHint().height() - 20);

  autoname->resize(autoname->sizeHint());
  autoname->move((width() - autoname->width())/2,
		 (box->geometry().bottom() + height())/2);
}


void IPWidget::autoname_t(bool on) {
  static bool was_warned = false;

  // big-fat warning when selecting the auto configure hostname option
  if(on && !was_warned) {
    QMessageBox::warning(this, 
			 i18n("Warning"),
			 i18n("Selecting this option might cause some weird\n"
			      "problems with the X-server and applications\n"
			      "while kppp is connected. Don´t use it until\n"
			      "you know what you are doing!\n\n"
			      "For more information take a look into the\n"
			      "handbook (or help) in the section \"Frequently\n"
			      "asked questions\""),
			 i18n("OK"));
    was_warned = true;
  }
}


void IPWidget::save() {
  if(dynamicadd_rb->isChecked()) {
    gpppdata.setIpaddr("0.0.0.0");
    gpppdata.setSubnetmask("0.0.0.0");
  } else {
    gpppdata.setIpaddr(ipaddress_l->text());
    gpppdata.setSubnetmask(subnetmask_l->text());
  }
  gpppdata.setAutoname(autoname->isChecked()); 
}


void IPWidget::hitIPSelect( int i ) {
  if(i == 0) {
    ipaddress_label->setEnabled(false);
    sub_label->setEnabled(false);
    ipaddress_l->setEnabled(false);
    subnetmask_l->setEnabled(false);
  }
  else {
    ipaddress_label->setEnabled(true);
    sub_label->setEnabled(true);
    ipaddress_l->setEnabled(true);
    subnetmask_l->setEnabled(true);
  }
}



DNSWidget::DNSWidget( QWidget *parent, bool isnewaccount, const char *name )
  : KGroupBox(i18n("DNS Servers"), parent, name)
{
  QGridLayout *tl = new QGridLayout(peer(), 7, 2, 10, 10);

  dnsdomain_label = new QLabel(i18n("Domain Name:"), peer());
  tl->addWidget(dnsdomain_label, 0, 0);

  dnsdomain = new QLineEdit(peer());
  dnsdomain->setMaxLength(DOMAIN_SIZE);
  tl->addWidget(dnsdomain, 0, 1);
  KQuickHelp::add(dnsdomain_label,
  KQuickHelp::add(dnsdomain, 
		  i18n("If you enter a domain name here, this domain\n"
		       "name is used for your computer while you are\n"
		       "connected. When the connection is closed, the\n"
		       "original domain name of your computer is\n"
		       "restored.\n"
		       "\n"
		       "If you leave this field blank, no changes are\n"
		       "made to the domain name.")));

  dns_label = new QLabel(i18n("DNS IP Address:"), peer());
  tl->addWidget(dns_label, 2, 0);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2, 2, 1);
  dnsipaddr = new IPLineEdit(peer());
  connect(dnsipaddr, SIGNAL(returnPressed()), 
	  SLOT(adddns()));
  connect(dnsipaddr, SIGNAL(textChanged(const QString &)), 
	  SLOT(DNS_Edit_Changed(const QString &)));
  l2->addWidget(dnsipaddr, 1);
  l2->addStretch(1);
  KQuickHelp::add(dns_label, 
  KQuickHelp::add(dnsipaddr, 
		  i18n("Allows you to specify a new DNS server to be\n"
		       "used while you are connected. When the\n"
		       "connection is closed, this DNS entry will be\n"
		       "removed again.\n"
		       "\n"
		       "To add a DNS server, type in the IP address of\n"
		       "of the DNS server here and click on <b>Add</b>")));

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 3, 1);
  add = new QPushButton(i18n("Add"), peer());
  connect(add, SIGNAL(clicked()), SLOT(adddns()));
  int width = add->sizeHint().width();
  width = QMAX(width,60);
  add->setMinimumWidth(width);
  l1->addWidget(add);
  l1->addStretch(1);
  KQuickHelp::add(add,
		  i18n("Click this button to add the DNS server\n"
		       "specified in the field above. The entry\n"
		       "will then be added to the list below"));
		  
  remove = new QPushButton(i18n("Remove"), peer());
  connect(remove, SIGNAL(clicked()), SLOT(removedns()));
  width = remove->sizeHint().width();
  width = QMAX(width,60);
  remove->setMinimumWidth(width);
  l1->addWidget(remove);
  KQuickHelp::add(remove,
		  i18n("Click this button to remove the selected DNS\n"
		       "server entry from the list below"));

  servers_label = new QLabel(i18n("DNS Address List:"), peer());
  servers_label->setAlignment(AlignTop|AlignLeft);
  tl->addWidget(servers_label, 4, 0);

  dnsservers = new QListBox(peer());
  dnsservers->setMinimumSize(150, 100);
  connect(dnsservers, SIGNAL(highlighted(int)),
	  SLOT(DNS_Entry_Selected(int)));
  tl->addWidget(dnsservers, 4, 1);
  KQuickHelp::add(servers_label,
  KQuickHelp::add(dnsservers,
		  i18n("This shows all defined DNS servers to use\n"
		       "while you are connected. Use the <b>Add</b> and\n"
		       "<b>Remove</b> buttons to modify the list")));

  exdnsdisabled_toggle = new QCheckBox(i18n( \
"Disable existing DNS Servers during Connection"),
				     peer());
  exdnsdisabled_toggle->setChecked(gpppdata.exDNSDisabled());
  tl->addMultiCellWidget(exdnsdisabled_toggle, 6, 6, 0, 1, AlignCenter);
  KQuickHelp::add(exdnsdisabled_toggle,
		  i18n("When this option is selected, all DNS\n"
		       "servers specified in <i>/etc/resolv.conf</i> are\n"
		       "temporary disabled while the dialup connection\n"
		       "is established. After the connection is\n"
		       "closed, the servers will be re-enabled\n"
		       "\n"
		       "Typically, there is no reason to use this\n"
		       "option, but it may become useful under \n"
		       "some circumstances"));


  // restore data if editing
  if(!isnewaccount) {
    QStrList &dnslist = gpppdata.dns();
    for(char *dns = dnslist.first(); dns; dns = dnslist.next())
      dnsservers->insertItem(dns);
    dnsdomain->setText(gpppdata.domain());
  }

  // disable buttons
  DNS_Edit_Changed("");
  remove->setEnabled(false);

  tl->activate();
}

void DNSWidget::DNS_Edit_Changed(const QString &text) {
  QRegExp r("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");
  add->setEnabled(text.find(r) != -1);
}

void DNSWidget::DNS_Entry_Selected(int) {
  remove->setEnabled(true);
}

void DNSWidget::save() {
  QStrList serverlist;
  for(uint i=0; i < dnsservers->count(); i++)
    serverlist.append(dnsservers->text(i));
  gpppdata.setDns(serverlist);
  
  // strip leading dot
  QString s(dnsdomain->text());
  if(s.left(1) == ".")
    gpppdata.setDomain(s.mid(1));
  else
    gpppdata.setDomain(dnsdomain->text());

  gpppdata.setExDNSDisabled(exdnsdisabled_toggle->isChecked());
}


void DNSWidget::adddns() {
  if(dnsservers->count() < MAX_DNS_ENTRIES) {
    dnsservers->insertItem(dnsipaddr->text());
    dnsipaddr->setText("");
  }
}


void DNSWidget::removedns() {
  int i;
  i = dnsservers->currentItem();
  if(i != -1)
    dnsservers->removeItem(i);
}


//
// GatewayWidget
//
GatewayWidget::GatewayWidget( QWidget *parent, bool isnewaccount, const char *name )
  : KGroupBox(i18n("Gateway Setup"), parent, name)
{
  box = new QGroupBox(peer());

  rb = new QButtonGroup(peer());
  rb->hide();
  connect(rb, SIGNAL(clicked(int)), SLOT(hitGatewaySelect(int)));

  defaultgateway = new QRadioButton(peer());
  defaultgateway->setText(i18n("Default Gateway"));
  rb->insert(defaultgateway, 0);
  KQuickHelp::add(defaultgateway,
		  i18n("This makes the PPP peer computer (the computer\n"
		       "you are connected to with your modem) to act as\n"
		       "a gateway. Your computer will send all packets not\n"
		       "going to a computer inside your local net to this\n"
		       "computer, which will route this packets.\n"
		       "\n"
		       "This is the default for most ISPs, so you should\n"
		       "probably leave this option on"));
		  

  staticgateway = new QRadioButton(peer());
  staticgateway->setText(i18n("Static Gateway"));
  rb->insert(staticgateway, 1);
  KQuickHelp::add(staticgateway,
		  i18n("Allows you to specify which computer you want\n"
		       "to use as gateway (see <i>Default Gateway</i> above"));

  gatewayaddr = new IPLineEdit(peer());

  gate_label = new QLabel(i18n("Gateway\nIP Address:"), peer());

  defaultroute = new QCheckBox(i18n("Assign the Default Route to this Gateway"),
			     peer());
  KQuickHelp::add(defaultroute,
		  i18n("If this option is enabled, all packets not\n"
		       "going to the local net are routed through\n"
		       "the PPP connection.\n"
		       "\n"
		       "Normally, you should turn this on"));

  //load info from gpppdata
  if(!isnewaccount) {
    if(strcmp(gpppdata.gateway(),"0.0.0.0")==0 ) {
      defaultgateway->setChecked(true);
      hitGatewaySelect(0);
    }
    else {
      gatewayaddr->setText(gpppdata.gateway());
      staticgateway->setChecked(true);
    }
    defaultroute->setChecked(gpppdata.defaultroute());
  }
  else {
    defaultgateway->setChecked(true);
    hitGatewaySelect(0);
    defaultroute->setChecked(true);
  }
}

void GatewayWidget::resizeEvent(QResizeEvent *e) {
  KGroupBox::resizeEvent(e);

  // calculate the best with for the frame
  int minw = gate_label->sizeHint().width() + 6 + 
    gatewayaddr->sizeHint().width() + 40;

  // egcs wants this to prevent a warning
  int minh = 0;
  minh = QMAX(gatewayaddr->sizeHint().height(),
	      gate_label->sizeHint().height()) +
    fontMetrics().lineSpacing() + 20;

  // resize the frame
  int box_x = (width() - minw)/2;
  int box_y = (height() - minh)/2 - 20;
  box->setGeometry(box_x, box_y, minw, minh);

  // now move the lineedits into the frame
  gatewayaddr->resize(gatewayaddr->sizeHint());
  gatewayaddr->move(box->geometry().right() - gatewayaddr->width() - 15,
		    box_y + fontMetrics().lineSpacing()/2 + 10);

  // the labels
  gate_label->resize(gate_label->sizeHint());
  gate_label->move(box_x + 15, gatewayaddr->geometry().top());		

  // move the radiobuttons
  staticgateway->resize(staticgateway->sizeHint());
  defaultgateway->resize(defaultgateway->sizeHint());  
  staticgateway->move(box_x + 25, 
		     box_y - staticgateway->sizeHint().height()/2);
  defaultgateway->move(box_x + 25,
		      staticgateway->geometry().top() - 
		      defaultgateway->sizeHint().height() - 20);

  defaultroute->resize(defaultroute->sizeHint());
  defaultroute->move((width() - defaultroute->width())/2,
		 (box->geometry().bottom() + height())/2);
}


void GatewayWidget::save() {
  gpppdata.setGateway(gatewayaddr->text());
  gpppdata.setDefaultroute(defaultroute->isChecked()); 
}


void GatewayWidget::hitGatewaySelect( int i ) {
  if(i == 0) {
    gatewayaddr->setText("0.0.0.0");
    gatewayaddr->setEnabled(false);
    gate_label->setEnabled(false);
  }
  else {
    gatewayaddr->setEnabled(true);
    gatewayaddr->setText("");
    gate_label->setEnabled(true);
  }
}



ScriptWidget::ScriptWidget( QWidget *parent, bool isnewaccount, const char *name )
  : KGroupBox(i18n("Edit Script"), parent, name)
{ 
  QVBoxLayout *tl = new QVBoxLayout(peer(), 20, 10);
  se = new ScriptEdit(peer());
  connect(se, SIGNAL(returnPressed()), SLOT(addButton()));
  tl->addWidget(se);
  
  // insert equal-sized buttons
  KButtonBox *bbox = new KButtonBox(peer());
  add = bbox->addButton(i18n("Add"));
  connect(add, SIGNAL(clicked()), SLOT(addButton()));
  bbox->addStretch(1);
  insert = bbox->addButton(i18n("Insert"));
  connect(insert, SIGNAL(clicked()), SLOT(insertButton()));
  bbox->addStretch(1);
  remove = bbox->addButton(i18n("Remove"));
  connect(remove, SIGNAL(clicked()), SLOT(removeButton()));
  bbox->layout();
  tl->addWidget(bbox);

  QHBoxLayout *l12 = new QHBoxLayout(0);
  tl->addLayout(l12);
  stl = new QListBox(peer());
  stl->setSmoothScrolling(false);
  stl->setAutoScrollBar(false);
  connect(stl, SIGNAL(highlighted(int)), SLOT(stlhighlighted(int)));
  stl->setMinimumSize(QSize(70, 140));

  sl = new QListBox(peer());
  sl->setSmoothScrolling(false);
  sl->setAutoScrollBar(false);
  connect(sl, SIGNAL(highlighted(int)), SLOT(slhighlighted(int)));
  sl->setMinimumSize(QSize(150, 140));

  slb = new QScrollBar(peer());
  slb->setFixedWidth(slb->sizeHint().width());
  connect(slb, SIGNAL(valueChanged(int)), SLOT(scrolling(int)));

  l12->addWidget(stl, 1);
  l12->addWidget(sl, 3);
  l12->addWidget(slb, 0);

  //load data from gpppdata
  if(!isnewaccount) {
    QStrList &comlist = gpppdata.scriptType();
    QStrList &arglist = gpppdata.script();
    for(char *com = comlist.first(), *arg = arglist.first();
        com && arg; com = comlist.next(), arg = arglist.next()) {
      stl->insertItem(com);
      sl->insertItem(arg);
    }
  }

  insert->setEnabled(false);
  remove->setEnabled(false);
  adjustScrollBar();
  tl->activate();
}

bool ScriptWidget::check() {
  uint lstart = 0;
  uint lend  = 0;
  uint errcnt = 0;

  if(sl->count() > 0)   {
    for( uint i=0; i <= sl->count()-1; i++) {
	if ( 0 == strcmp( "LoopStart", stl->text(i)) )  {
		lstart++;
        }
	if ( 0 == strcmp( "LoopEnd", stl->text(i)) )  {
		lend++;
        }
	if ( lend > lstart ) errcnt++;
    }
    return ( (errcnt == 0 ) && (lstart == lend) );
  } 
  return true;
}


void ScriptWidget::save() {
  QStrList typelist, arglist;
  for(uint i=0; i < sl->count(); i++) {
    typelist.append(stl->text(i));
    arglist.append(sl->text(i));
  }
  gpppdata.setScriptType(typelist);
  gpppdata.setScript(arglist);
}                        



void ScriptWidget::adjustScrollBar() {
  if((int)sl->count() <= sl->numItemsVisible())
    slb->setRange(0, 0);
  else
    slb->setRange(0, (sl->count() - sl->numItemsVisible())+1);
}


void ScriptWidget::scrolling(int i) {
  sl->setTopItem(i);
  stl->setTopItem(i);
}


void ScriptWidget::slhighlighted(int i) {
  insert->setEnabled(true);
  remove->setEnabled(true);
  stl->setCurrentItem(i);
}


void ScriptWidget::stlhighlighted(int i) {
  insert->setEnabled(true);
  remove->setEnabled(true);
  sl->setCurrentItem(i);
}


void ScriptWidget::addButton() {
  //don't allow more than the maximum script entries
  if(sl->count() == MAX_SCRIPT_ENTRIES-1)
    return;

  switch(se->type()) {
    case ScriptEdit::Expect:
      stl->insertItem("Expect");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Send:
      stl->insertItem("Send");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::SendNoEcho:
      stl->insertItem("SendNoEcho");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Pause:
      stl->insertItem("Pause");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Hangup:
      stl->insertItem("Hangup");
      sl->insertItem("");
      break;

    case ScriptEdit::Answer:
      stl->insertItem("Answer");
      sl->insertItem("");
      break;

    case ScriptEdit::Timeout:
      stl->insertItem("Timeout");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Password:
      stl->insertItem("Password");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::ID:
      stl->insertItem("ID");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Prompt:
      stl->insertItem("Prompt");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::PWPrompt:
      stl->insertItem("PWPrompt");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::LoopStart:
      stl->insertItem("LoopStart");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::LoopEnd:
      stl->insertItem("LoopEnd");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Scan:
      stl->insertItem("Scan");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Save:
      stl->insertItem("Save");
      sl->insertItem(se->text());
      break;

    case ScriptEdit::Mode:
      stl->insertItem("Mode");
      sl->insertItem(se->text());
      break;

    default:
      break;
  }

  //get the scrollbar adjusted, and scroll the list so we can see what
  //we're adding to
  adjustScrollBar();
  slb->setValue(slb->maxValue());

  //clear the text in the entry box
  se->setText("");
}


void ScriptWidget::insertButton() {
  //exit if there is no highlighted item, or we've reached the
  //maximum entries in the script list
  if(sl->currentItem() < 0 || (sl->count() == MAX_SCRIPT_ENTRIES-1))
    return;

  switch(se->type()) {
    case ScriptEdit::Expect:
      stl->insertItem("Expect", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Send:
      stl->insertItem("Send", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::SendNoEcho:
      stl->insertItem("SendNoEcho", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Pause:
      stl->insertItem("Pause", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Hangup:
      stl->insertItem("Hangup", stl->currentItem());
      sl->insertItem("", sl->currentItem());
      break;

    case ScriptEdit::Answer:
      stl->insertItem("Answer", stl->currentItem());
      sl->insertItem("", sl->currentItem());
      break;

    case ScriptEdit::Timeout:
      stl->insertItem("Timeout", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Password:
      stl->insertItem("Password", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::ID:
      stl->insertItem("ID", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Prompt:
      stl->insertItem("Prompt", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::PWPrompt:
      stl->insertItem("PWPrompt", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::LoopStart:
      stl->insertItem("LoopStart", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::LoopEnd:
      stl->insertItem("LoopEnd", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Scan:
      stl->insertItem("Scan", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Save:
      stl->insertItem("Save", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    case ScriptEdit::Mode:
      stl->insertItem("Mode", stl->currentItem());
      sl->insertItem(se->text(), sl->currentItem());
      break;

    default:
      break;
  }
  adjustScrollBar();
  se->setText("");
}


void ScriptWidget::removeButton() {
 if(sl->currentItem() >= 0) {
   int stlc = stl->currentItem();
   sl->removeItem(sl->currentItem());
   stl->removeItem(stlc);
   adjustScrollBar();
   insert->setEnabled(sl->currentItem() != -1);
   remove->setEnabled(sl->currentItem() != -1);
 }
}



/////////////////////////////////////////////////////////////////////////////
//
// Used to specify a new phone number
//
/////////////////////////////////////////////////////////////////////////////
PhoneNumberDialog::PhoneNumberDialog(QWidget *parent) : QDialog(parent, 0, true) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  setCaption(i18n("Add a phone number..."));
  KWM::setMiniIcon(winId(), kapp->getMiniIcon());
  tl->addSpacing(5);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1);
  QLabel *l = new QLabel(i18n("Phone number:"), this);
  l1->addWidget(l);
  le = newLineEdit(14, this);
  l1->addWidget(le);
  connect(le, SIGNAL(textChanged(const QString &)),
	  this, SLOT(textChanged(const QString &)));

  tl->addSpacing(5);
  KButtonBox *bbox = new KButtonBox(this);
  bbox->addStretch(1);
  ok = bbox->addButton(i18n("OK"));
  ok->setDefault(true);
  connect(ok, SIGNAL(clicked()),
	  this, SLOT(accept()));
  cancel = bbox->addButton(i18n("Cancel"));
  connect(cancel, SIGNAL(clicked()),
	  this, SLOT(reject()));
  bbox->layout();
  tl->addWidget(bbox);
  setFixedSize(sizeHint());

  le->setFocus();
  textChanged("");
}


QString PhoneNumberDialog::phoneNumber() {
  QString s = le->text();

  return s;
}


void PhoneNumberDialog::textChanged(const QString &s) {
  ok->setEnabled(s.length() > 0);
}


#include "edit.moc"
