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

#include "edit.h"
#include "termios.h"
#include <qlayout.h>
#include <kquickhelp.h>
#include <qregexp.h>

#include "macros.h"

DialWidget::DialWidget( QWidget *parent, bool isnewaccount, const char *name )
  : QWidget(parent, name)
{
  const int GRIDROWS = 11;

  QGridLayout *tl = new QGridLayout(this, GRIDROWS, 4, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this);
  box->setTitle(i18n("Dial Setup"));
  tl->addMultiCellWidget(box, 0, GRIDROWS-1, 0, 3);

  connect_label = new QLabel(this);
  connect_label->setText(i18n("Connection Name:"));
  MIN_SIZE(connect_label);
  tl->addWidget(connect_label, 1, 1);

  connectname_l = new QLineEdit(this);
  connectname_l->setMaxLength(ACCNAME_SIZE);
  FIXED_HEIGHT(connectname_l);
  MIN_WIDTH(connectname_l);
  tl->addWidget(connectname_l, 1, 2);
  KQuickHelp::add(connect_label,
  KQuickHelp::add(connectname_l,
		  "Type in a unique name for this connection"));

  
  number_label = new QLabel(this);
  number_label->setText(i18n("Phone Number:"));
  MIN_SIZE(number_label);
  tl->addWidget(number_label, 2, 1);

  number_l = new QLineEdit(this);
  number_l->setMaxLength(PHONENUMBER_SIZE);
  KQuickHelp::add(number_label,
  KQuickHelp::add(number_l,
		  "Specifies the phone number to dial. You\n"
		  "can supply multiple numbers separated by\n"
		  "a colon. When a number is busy or fails,\n"
		  "<i>kppp</i>will try the next number and so on"));
  
  FIXED_WIDTH(number_l);
  FIXED_HEIGHT(number_l);
  tl->addWidget(number_l, 2, 2);

  auth_l = new QLabel(this);
  auth_l->setText(i18n("Authentication:"));
  MIN_SIZE(auth_l);
  tl->addWidget(auth_l, 4, 1);

  auth = new QComboBox(this);
  auth->insertItem(i18n("Script-based"));
  auth->insertItem(i18n("PAP"));
  auth->insertItem(i18n("Terminal-based"));
  auth->insertItem(i18n("CHAP"));
  connect(auth, SIGNAL(activated(int)),
	  this, SLOT(authSelected(int)));
  MIN_WIDTH(auth);
  FIXED_HEIGHT(auth);
  tl->addWidget(auth, 4, 2);
  KQuickHelp::add(auth_l,
  KQuickHelp::add(auth,
		  "Specifies the method to identify yourself to\n"
		  "the PPP server. Most universities still use\n"
		  "<b>Terminal</b>- or <b>Script</b>-based authtentication,\n"
		  "while most ISP use <b>PAP</b> and/or <b>CHAP</b>. If\n"
		  "unsure, contact your ISP.\n"
		  "\n"
		  "If you can choose between PAP and CHAP,\n"
		  "choose CHAP, because it's much safer."));

  store_password = new QCheckBox(this);
  store_password->setText(i18n("Store password"));
  MIN_SIZE(store_password);
  store_password->setChecked(true);
  tl->addMultiCellWidget(store_password, 5, 5, 1, 2);

  command_label = new QLabel(this);
  command_label->setText(i18n("Execute program\nupon connect:"));
  command_label->setAlignment(AlignVCenter);
  MIN_SIZE(command_label);
  tl->addWidget(command_label, 6, 1);

  command = new QLineEdit(this);
  command->setMaxLength(COMMAND_SIZE);
  FIXED_HEIGHT(command);
  MIN_WIDTH(command);
  tl->addWidget(command, 6, 2);
  KQuickHelp::add(command_label,
  KQuickHelp::add(command,
		  "Allows you to run a program <b>after</b> a connection\n"
		  "is established. When your program is called, all\n"
		  "all preparations for an internet connection are\n"
		  "finished.\n"
		  "\n"
		  "Very usefull for fetching mail and news"));


  predisconnect_label = new QLabel(this);
  predisconnect_label->setText(i18n("Execute program\nbefore disconnect:"));
  predisconnect_label->setAlignment(AlignVCenter);
  MIN_SIZE(predisconnect_label);
  tl->addWidget(predisconnect_label, 7, 1);

  predisconnect = new QLineEdit(this);
  predisconnect->setMaxLength(COMMAND_SIZE);
  FIXED_HEIGHT(predisconnect);
  MIN_WIDTH(predisconnect);
  tl->addWidget(predisconnect, 7, 2);
  KQuickHelp::add(predisconnect_label,
  KQuickHelp::add(predisconnect,
		  "Allows you to run a program <b>before</b> a connection\n"
		  "is closed. The connection will stay open until\n"
		  "the program exits."));

  discommand_label = new QLabel(this);
  discommand_label->setText(i18n("Execute program\nupon disconnect:"));
  discommand_label->setAlignment(AlignVCenter);
  MIN_SIZE(discommand_label);
  tl->addWidget(discommand_label, 8, 1);

  discommand = new QLineEdit(this);
  discommand->setMaxLength(COMMAND_SIZE);
  FIXED_HEIGHT(discommand);
  MIN_WIDTH(discommand);
  tl->addWidget(discommand, 8, 2);
  KQuickHelp::add(discommand_label,
  KQuickHelp::add(discommand,
		  "Allows you to run a program <b>after</b> a connection\n"
		  "is closed."));


  pppd_label = new QLabel(this);
  pppd_label->setText(i18n("Edit pppd arguments:"));
  MIN_SIZE(pppd_label);
  tl->addWidget(pppd_label, 9, 1);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2, 9, 2);
  pppdargs = new QPushButton(i18n("Arguments"), this);
  connect(pppdargs, SIGNAL(clicked()), SLOT(pppdargsbutton()));
  MIN_SIZE(pppdargs);
  FIXED_HEIGHT(pppdargs);
  l2->addWidget(pppdargs);
  l2->addStretch(3);

  // Set defaults if editing an existing connection
  if(!isnewaccount) {
    connectname_l->setText(gpppdata.accname());
    number_l->setText(gpppdata.phonenumber());
    command->setText(gpppdata.command_on_connect());
    discommand->setText(gpppdata.command_on_disconnect());
    predisconnect->setText(gpppdata.command_before_disconnect());
    auth->setCurrentItem(gpppdata.authMethod());
    store_password->setChecked(gpppdata.storePassword());
  } else {
    auth->setCurrentItem(1);
  }

  tl->activate();
}


void DialWidget::authSelected(int i) {
  emit authChanged(i);
}


void DialWidget::show() {
  authSelected(auth->currentItem());
  QWidget::show();
}


bool DialWidget::save() {
  //first check to make sure that the account name is unique!
  if(strcmp(connectname_l->text(), "") == 0 ||
     !gpppdata.isUniqueAccname(connectname_l->text())) {
    return false;
  } else {
    gpppdata.setAccname(connectname_l->text());
    gpppdata.setPhonenumber(number_l->text());
    gpppdata.setCommand_on_connect(command->text());
    gpppdata.setCommand_before_disconnect(predisconnect->text());
    gpppdata.setCommand_on_disconnect(discommand->text());
    gpppdata.setAuthMethod(auth->currentItem());
    gpppdata.setStorePassword(store_password->isChecked());
    return true;
  }
}


void DialWidget::pppdargsbutton() {
  PPPdArguments pa(this);
  pa.exec();
}


//
// IPWidget
//
IPWidget::IPWidget( QWidget *parent, bool isnewaccount, const char *name )
  : QWidget(parent, name)
{

  box1 = new QGroupBox(this);
  box1->setTitle(i18n("IP Setup"));

  box = new QGroupBox(this);

  ipaddress_l = new IPLineEdit(this);

  ipaddress_label = new QLabel(this);
  ipaddress_label->setText(i18n("IP Address:"));
  KQuickHelp::add(ipaddress_label,
  KQuickHelp::add(ipaddress_l,
		  "If your computer has a static Internet address,\n"
		  "you must supply this adress here"));

  subnetmask_l = new IPLineEdit(this);

  sub_label = new QLabel(this);
  sub_label->setText(i18n("Subnet Mask:"));
  KQuickHelp::add(sub_label,
  KQuickHelp::add(subnetmask_l,
		  "If your computer has a static Internet address,\n"
		  "you must supply a network mask here. In almost\n"
		  "all cases this netmask will be <b>255.255.255.0</b>,\n"
		  "but your mileage may vary.\n"
		  "\n"
		  "If unsure, contact your Internet Service Provider"));


  rb = new QButtonGroup(this);
  rb->hide();
  connect(rb, SIGNAL(clicked(int)), 
	  SLOT(hitIPSelect(int)));  

  dynamicadd_rb = new QRadioButton(this);
  dynamicadd_rb->setText(i18n("Dynamic IP Address"));
  KQuickHelp::add(dynamicadd_rb,
		  "Select this option when your computer gets a\n"
		  "new internet address (IP) everytime when a\n"
		  "connection is made.\n"
		  "\n"
		  "Allmost every Internet Service uses this\n"
		  "method, so this should be turned on.");

  staticadd_rb = new QRadioButton(this);
  staticadd_rb->setText(i18n("Static IP Address"));
  rb->insert(dynamicadd_rb, 0);
  rb->insert(staticadd_rb, 1);
  KQuickHelp::add(staticadd_rb,
		  "Select this option when your computer has a\n"
		  "fixed internet address (IP). Most computers\n"
		  "don't have this, so you should better select\n"
		  "dynamic IP addresses unless you know what you\n"
		  "are doing");

  autoname=new QCheckBox(i18n("Auto-configure hostname from this IP"), this);
  KQuickHelp::add(autoname,
		  "Whenever you connect, this reconfigures\n"
		  "your hostname to match the IP address you\n"
		  "got from the PPP server. This may be useful\n"
		  "if you need to use a protocol which depends\n"
		  "on this information, but it brings several\n"
		  "<link kppp-7.html#autohostname>problems</link> with.\n"
		  "\n"
		  "Don't enable unless you really need it");
  
  autoname->adjustSize();
  autoname->setChecked(gpppdata.autoname());
  connect(autoname,SIGNAL(toggled(bool)),
	  this,SLOT(autoname_t(bool)));

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

void IPWidget::resizeEvent(QResizeEvent *) {
  box1->setGeometry(10, 10, width() - 20, height() - 20);

  // calculate the best with for the frame
  int minw = QMAX(ipaddress_label->sizeHint().width() + 6,
		 sub_label->sizeHint().width() + 6) +
    QMAX(ipaddress_l->sizeHint().width(),
	subnetmask_l->sizeHint().width()) + 10 + 20;

  int minh = 2 * ipaddress_l->sizeHint().height() + 
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
  on=on;
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
  : QWidget(parent, name)
{
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this);
  box->setTitle(i18n("DNS Servers"));
  tl->addMultiCellWidget(box, 0, 2, 0, 2);
  tl->setRowStretch(1, 1);
  tl->setColStretch(1, 1);
  tl->addColSpacing(0, 15);
  tl->addColSpacing(2, 15);
  tl->addRowSpacing(2, 10);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);
  l1->addSpacing(10);

  QGridLayout *l11 = new QGridLayout(5, 2);
  l1->addLayout(l11);

  dnsdomain_label = new QLabel(this);
  dnsdomain_label->setText(i18n("Domain Name:"));
  MIN_SIZE(dnsdomain_label);
  l11->addWidget(dnsdomain_label, 0, 0);

  dnsdomain = new QLineEdit(this);
  dnsdomain->setMaxLength(DOMAIN_SIZE);
  FIXED_HEIGHT(dnsdomain);
  MIN_WIDTH(dnsdomain);
  l11->addWidget(dnsdomain, 0, 1);
  l11->addRowSpacing(1, 15);
  KQuickHelp::add(dnsdomain_label,
  KQuickHelp::add(dnsdomain, 
		  "If you enter a domain name here, this domain\n"
		  "name is used for your computer while your are\n"
		  "connected. When the connection is closed, the\n"
		  "original domain name of your computer is\n"
		  "restored.\n"
		  "\n"
		  "If you leave this field blank, no changes are\n"
		  "made to the domain name."));

  dns_label = new QLabel(this);
  dns_label->setText(i18n("DNS IP Address:"));
  MIN_SIZE(dns_label);
  l11->addWidget(dns_label, 2, 0);

  QHBoxLayout *l110 = new QHBoxLayout;
  l11->addLayout(l110, 2, 1);
  dnsipaddr = new IPLineEdit(this);
  connect(dnsipaddr, SIGNAL(returnPressed()), 
	  SLOT(adddns()));
  connect(dnsipaddr, SIGNAL(textChanged(const char *)), 
	  SLOT(DNS_Edit_Changed(const char *)));
  FIXED_HEIGHT(dnsipaddr);
  l110->addWidget(dnsipaddr, 4);
  l110->addStretch(3);
  KQuickHelp::add(dns_label, 
  KQuickHelp::add(dnsipaddr, 
		  "Allows you to specify a new DNS server which\n"
		  "is used while you are connected. When the\n"
		  "connection is closed, this DNS entry will be\n"
		  "removed again.\n"
		  "\n"
		  "To add a DNS server type in the IP address of\n"
		  "of the DNS server here and click on <b>Add</b>"));

  QHBoxLayout *l111 = new QHBoxLayout;
  l11->addLayout(l111, 3, 1);
  add = new QPushButton(i18n("Add"), this);
  connect(add, SIGNAL(clicked()), SLOT(adddns()));
  FIXED_HEIGHT(add);
  int width = add->sizeHint().width();
  width = QMAX(width,60);
  add->setMinimumWidth(width);
  l111->addWidget(add);
  l111->addStretch(1);
  KQuickHelp::add(add,
		  "Click this button to add the DNS server\n"
		  "specified in the field above. The entry\n"
		  "will then be added to the list below");

  remove = new QPushButton(i18n("Remove"), this);
  connect(remove, SIGNAL(clicked()), SLOT(removedns()));
  FIXED_HEIGHT(remove);
  width = remove->sizeHint().width();
  width = QMAX(width,60);
  remove->setMinimumWidth(width);
  l111->addWidget(remove);
  KQuickHelp::add(remove,
		  "Click this button to remove the DNS server\n"
		  "from the list below");

  servers_label = new QLabel(this);
  servers_label->setText(i18n("DNS Address List:"));
  servers_label->setAlignment(AlignTop|AlignLeft);
  MIN_SIZE(servers_label);
  l11->addWidget(servers_label, 4, 0);
 
  dnsservers = new QListBox(this);
  dnsservers->setMinimumSize(150, 100);
  connect(dnsservers, SIGNAL(highlighted(int)),
	  SLOT(DNS_Entry_Selected(int)));
  l11->addWidget(dnsservers, 4, 1);
  KQuickHelp::add(servers_label,
  KQuickHelp::add(dnsservers,
		  "This shows all defined DNS servers to use\n"
		  "while you are connected. Use the <b>Add</b> and\n"
		  "<b>Remove</b> buttons to modify the list"));

  exdnsdisabled_toggle = new QCheckBox(i18n(
     "Disable existing DNS Servers during Connection"),
				       this);
  MIN_SIZE(exdnsdisabled_toggle);
  exdnsdisabled_toggle->setChecked(gpppdata.exDNSDisabled());
  l1->addStretch(2);
  l1->addWidget(exdnsdisabled_toggle);
  l1->addStretch(1);
  KQuickHelp::add(exdnsdisabled_toggle,
		  "When this option is selected, all DNS\n"
		  "servers specified in <i>/etc/resolv.conf</i> are\n"
		  "temporary disabled while a connection\n"
		  "is established. After the connection is\n"
		  "closed, the servers will be re-enabled\n"
		  "\n"
		  "Typically, there is no reason to use this\n"
		  "option, but it may become usefull under \n"
		  "some circumstances");
 
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

void DNSWidget::DNS_Edit_Changed(const char *text) {
  QRegExp r("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");
  QString s(text);
  add->setEnabled(s.find(r) != -1);
}

void DNSWidget::DNS_Entry_Selected(int idx) {
  remove->setEnabled(true);
}

void DNSWidget::save() {
  QStrList serverlist;
  for(uint i=0; i < dnsservers->count(); i++)
    serverlist.append(dnsservers->text(i));
  gpppdata.setDns(serverlist);

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
  : QWidget(parent, name)
{
  box1 = new QGroupBox(this);
  box1->setTitle(i18n("Gateway Setup"));

  box = new QGroupBox(this);

  rb = new QButtonGroup(this);
  rb->hide();
  connect(rb, SIGNAL(clicked(int)), SLOT(hitGatewaySelect(int)));

  defaultgateway = new QRadioButton(this);
  defaultgateway->setText(i18n("Default Gateway"));
  rb->insert(defaultgateway, 0);
  KQuickHelp::add(defaultgateway,
		  "This makes the PPP peer computer (the computer\n"
		  "you are connected to with your modem) to act as\n"
		  "gateway. Your computer will send all packets not\n"
		  "going to a computer inside your local net to this\n"
		  "computer, which will route this packets.\n"
		  "\n"
		  "This is default for most ISP, so better leave this\n"
		  "option on");
		  

  staticgateway = new QRadioButton(this);
  staticgateway->setText(i18n("Static Gateway"));
  rb->insert(staticgateway, 1);
  KQuickHelp::add(staticgateway,
		  "Allows you to specify which computer you want\n"
		  "to use as gateway (see <i>Default Gateway</i> above");

  gatewayaddr = new IPLineEdit(this);

  gate_label = new QLabel(this);
  gate_label->setText(i18n("Gateway\nIP Address:"));

  defaultroute=new QCheckBox(i18n("Assign the Default Route to this Gateway"),
			     this);
  defaultroute->adjustSize();
  KQuickHelp::add(defaultroute,
		  "If this option is enabled, all packets not\n"
		  "going to the local net are routed through\n"
		  "the PPP connection.\n"
		  "\n"
		  "Normally, you should turn this on");

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

void GatewayWidget::resizeEvent(QResizeEvent *) {
  box1->setGeometry(10, 10, width() - 20, height() - 20);

  // calculate the best with for the frame
  int minw = gate_label->sizeHint().width() + 6 + 
    gatewayaddr->sizeHint().width() + 40;

  int minh = 
    QMAX(gatewayaddr->sizeHint().height(),
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
  : QWidget(parent, name)
{ 
  const int GRIDROWS = 3;

  QGridLayout *tl = new QGridLayout(this, GRIDROWS, 3, 10, 10);
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10);
  box = new QGroupBox(this);
  box->setTitle(i18n("Edit Script"));
  tl->addMultiCellWidget(box, 0, GRIDROWS-1, 0, 2);

  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);
  //  default_script = 
  //    new QCheckBox(i18n("Use default script"), this);
  //  MIN_SIZE(default_script);
  //  l1->addWidget(default_script);
  //  l1->addStretch(1);

  se = new ScriptEdit(this);
  connect(se, SIGNAL(returnPressed()), SLOT(addButton()));
  l1->addWidget(se);

  add = new QPushButton(i18n("Add"), this);
  connect(add, SIGNAL(clicked()), SLOT(addButton()));
  FIXED_HEIGHT(add);
  int width = add->sizeHint().width();
  width = QMAX(width,60);
  add->setMinimumWidth(width);


  insert = new QPushButton(i18n("Insert"), this);
  connect(insert, SIGNAL(clicked()), SLOT(insertButton()));
  FIXED_HEIGHT(insert);
  width = insert->sizeHint().width();
  width = QMAX(width,60);
  insert->setMinimumWidth(width);

  remove = new QPushButton(i18n("Remove"), this);
  connect(remove, SIGNAL(clicked()), SLOT(removeButton()));
  FIXED_HEIGHT(remove);
  width = remove->sizeHint().width();
  width = QMAX(width,60);
  remove->setMinimumWidth(width);

  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);
  l11->addWidget(add);
  l11->addStretch(1);
  l11->addWidget(insert);
  l11->addStretch(1);
  l11->addWidget(remove);
  
  QHBoxLayout *l12 = new QHBoxLayout(0);
  l1->addLayout(l12);
  stl = new QListBox(this);
  stl->setSmoothScrolling(false);
  stl->setAutoScrollBar(false);
  connect(stl, SIGNAL(highlighted(int)), SLOT(stlhighlighted(int)));
  stl->setMinimumSize(QSize(70, 140));

  sl = new QListBox(this);
  sl->setSmoothScrolling(false);
  sl->setAutoScrollBar(false);
  connect(sl, SIGNAL(highlighted(int)), SLOT(slhighlighted(int)));
  sl->setMinimumSize(QSize(150, 140));

  slb = new QScrollBar(this);
  slb->setFixedWidth(slb->sizeHint().width());
  connect(slb, SIGNAL(valueChanged(int)), SLOT(scrolling(int)));

  l12->addWidget(stl, 1);
  l12->addWidget(sl, 3);
  l12->addWidget(slb, 0);

  //  default_script->setChecked(true);
  //  connect(default_script, SIGNAL(toggled(bool)),
  //	  this, SLOT(default_script_toggled(bool)));
  //  default_script_toggled(true);

  //load data from gpppdata
  if(!isnewaccount) {
    QStrList &comlist = gpppdata.scriptType();
    QStrList &arglist = gpppdata.script();
    for(char *com = comlist.first(), *arg = arglist.first();
        com && arg; com = comlist.next(), arg = arglist.next()) {
      stl->insertItem(com);
      sl->insertItem(arg);
    }

    //    default_script->setChecked( (bool) (comlist.count()==0));
  }

  adjustScrollBar();

  tl->activate();
}


void ScriptWidget::authSelected(int i) {
  bool b = (i != 1) && (i != 3);

  se->setEnabled(b);
  add->setEnabled(b);
  insert->setEnabled(b);
  remove->setEnabled(b);
  stl->setEnabled(b);
  sl->setEnabled(b);
}


// void ScriptWidget::default_script_toggled(bool b) {
//  se->setEnabled(!b);
//  add->setEnabled(!b);
//  insert->setEnabled(!b);
//  remove->setEnabled(!b);
//  stl->setEnabled(!b);
//  sl->setEnabled(!b);
// }

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
  stl->setCurrentItem(i);
}


void ScriptWidget::stlhighlighted(int i) {
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
 }
}

#include "edit.moc"
