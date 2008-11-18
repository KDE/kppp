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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <string.h>
#include <termios.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <kmessagebox.h>

#include <qregexp.h>
#include <kiconloader.h>
#include <kdialogbuttonbox.h>
#include <klocale.h>
#include <kdialog.h>
#include <kwindowsystem.h>

#include "edit.h"
#include "pppdata.h"
#include "newwidget.h"
#include "iplined.h"
#include "auth.h"

DialWidget::DialWidget( QWidget *parent, bool isnewaccount )
  : QWidget(parent)
{
  parent->setMinimumWidth(440);
  QVBoxLayout *topLayout = new QVBoxLayout(parent);
  topLayout->setSpacing(KDialog::spacingHint());
  topLayout->addWidget(this);

  QGridLayout *tl = new QGridLayout(this );
  tl->setSpacing( KDialog::spacingHint() );

  connect_label = new QLabel(i18n("Connection &name:"), this);
  tl->addWidget(connect_label, 0, 0);

  connectname_l = new QLineEdit(this);
  connectname_l->setMaxLength(ACCNAME_SIZE);
  connect_label->setBuddy(connectname_l);

  tl->addWidget(connectname_l, 0, 1);
  QString tmp = i18n("Type in a unique name for this connection");

  connect_label->setWhatsThis(tmp);
  connectname_l->setWhatsThis(tmp);


  number_label = new QLabel(i18n("P&hone number:"), this);
  number_label->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  tl->addWidget(number_label, 1, 0);

  QHBoxLayout *lpn = new QHBoxLayout();
  lpn->setSpacing( 5 );
  tl->addLayout(lpn, 1, 1);
  numbers = new Q3ListBox(this);
  number_label->setBuddy(numbers);
  numbers->setMinimumSize(120, 70);
  lpn->addWidget(numbers);
  QVBoxLayout *lpn1 = new QVBoxLayout;
  lpn->addLayout(lpn1);
  add = new QPushButton(i18n("&Add..."), this);
  del = new QPushButton(i18n("&Remove"), this);

  up = new QPushButton(this);
  up->setIcon(KIcon("go-up"));
  down = new QPushButton(this);
  down->setIcon(KIcon("go-down"));
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

  tmp = i18n("<p>Specifies the phone numbers to dial. You\n"
	     "can supply multiple numbers here, simply\n"
	     "click on \"Add\". You can arrange the\n"
	     "order the numbers are tried by using the\n"
	     "arrow buttons.\n\n"
	     "When a number is busy or fails, <i>kppp</i> will \n"
	     "try the next number and so on");

  number_label->setWhatsThis(tmp);
  numbers->setWhatsThis(tmp);

  auth_l = new QLabel(i18n("A&uthentication:"), this);
  tl->addWidget(auth_l, 3, 0);

  auth = new QComboBox(this);
  auth_l->setBuddy(auth);
  auth->addItem(i18n("Script-based"));
  auth->addItem(i18n("PAP"));
  auth->addItem(i18n("Terminal-based"));
  auth->addItem(i18n("CHAP"));
  auth->addItem(i18n("PAP/CHAP"));
  tl->addWidget(auth, 3, 1);
  tmp = i18n("<p>Specifies the method used to identify yourself to\n"
	     "the PPP server. Most universities still use\n"
	     "<b>Terminal</b>- or <b>Script</b>-based authentication,\n"
	     "while most ISP use <b>PAP</b> and/or <b>CHAP</b>. If\n"
	     "unsure, contact your ISP.\n"
	     "\n"
	     "If you can choose between PAP and CHAP,\n"
	     "choose CHAP, because it's much safer. If you don't know\n"
	     "whether PAP or CHAP is right, choose PAP/CHAP.");

  auth_l->setWhatsThis(tmp);
  auth->setWhatsThis(tmp);

  store_password = new QCheckBox(i18n("Store &password"), this);
  store_password->setChecked(true);
  tl->addWidget(store_password, 4, 0, 1, 2, Qt::AlignRight);
  store_password->setWhatsThis(
		  i18n("<p>When this is turned on, your ISP password\n"
		       "will be saved in <i>kppp</i>'s config file, so\n"
		       "you do not need to type it in every time.\n"
		       "\n"
		       "<b><font color=\"red\">Warning:</font> your password will be stored as\n"
		       "plain text in the config file, which is\n"
		       "readable only to you. Make sure nobody\n"
		       "gains access to this file!"));

  cbtype_l = new QLabel(i18n("&Callback type:"), this);
  tl->addWidget(cbtype_l, 5, 0);

  cbtype = new QComboBox(this);
  cbtype_l->setBuddy(cbtype);
  cbtype->addItem(i18n("None"));
  cbtype->addItem(i18n("Administrator-defined"));
  cbtype->addItem(i18n("User-defined"));
  connect(cbtype, SIGNAL(highlighted(int)),
	  this, SLOT(cbtypeChanged(int)));
  tl->addWidget(cbtype, 5, 1);
  tmp = i18n("Callback type");

  cbtype_l->setWhatsThis(tmp);
  cbtype->setWhatsThis(tmp);

  cbphone_l = new QLabel(i18n("Call&back number:"), this);
  tl->addWidget(cbphone_l, 6, 0);

  cbphone = new QLineEdit(this);
  cbphone_l->setBuddy(cbphone);
  cbphone->setMaxLength(140);
  tl->addWidget(cbphone, 6, 1);
  tmp = i18n("Callback phone number");

  cbphone_l->setWhatsThis(tmp);
  cbphone->setWhatsThis(tmp);

  pppdargs = new QPushButton(i18n("Customize &pppd Arguments..."), this);
  connect(pppdargs, SIGNAL(clicked()), SLOT(pppdargsbutton()));
  tl->addWidget(pppdargs, 7, 0, 1, 2, Qt::AlignCenter);

  // Set defaults if editing an existing connection
  if(!isnewaccount) {
    connectname_l->setText(gpppdata.accname());

    // insert the phone numbers into the listbox
    QString n = gpppdata.phonenumber();
    QString tmp = "";
    int idx = 0;
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

    auth->setCurrentIndex(gpppdata.authMethod());
    store_password->setChecked(gpppdata.storePassword());
    cbtype->setCurrentIndex(gpppdata.callbackType());
    cbphone->setText(gpppdata.callbackPhone());
  } else {
    // select PAP/CHAP as default
    auth->setCurrentIndex(AUTH_PAPCHAP);
    // select NONE as default
    cbtype->setCurrentIndex(CBTYPE_NONE);
  }

  emit cbtypeChanged(cbtype->currentIndex());
  numbersChanged();
  tl->activate();
}


bool DialWidget::save() {
  //first check to make sure that the account name is unique!
  if(connectname_l->text().isEmpty() ||
     !gpppdata.isUniqueAccname(connectname_l->text())) {
    return false;
  } else {
    gpppdata.setAccname(connectname_l->text());

    QString number = "";
    for(uint i = 0; i < numbers->count(); i++) {
      if(i != 0)
	number += ':';
      number += numbers->text(i);
    }

    gpppdata.setPhonenumber(number);
    gpppdata.setAuthMethod(auth->currentIndex());
    gpppdata.setStorePassword(store_password->isChecked());
    gpppdata.setCallbackType(cbtype->currentIndex());
    gpppdata.setCallbackPhone(cbphone->text());
    return true;
  }
}


void DialWidget::numbersChanged() {
  int sel = numbers->currentItem();

  del->setEnabled(sel != -1);
  up->setEnabled(sel != -1 && sel != 0);
  down->setEnabled(sel != -1 && sel != (int)numbers->count()-1);
}

void DialWidget::cbtypeChanged(int value) {
  cbphone_l->setEnabled(value == CBTYPE_USER);
  cbphone->setEnabled(value == CBTYPE_USER);
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
ExecWidget::ExecWidget(QWidget *parent, bool isnewaccount) :
  QWidget(parent)
{
  QVBoxLayout *tl = new QVBoxLayout(parent);
  tl->setSpacing(KDialog::spacingHint());
  tl->setMargin(0);

  QLabel *l = new QLabel(\
i18n("Here you can select commands to run at certain stages of the\n"
     "connection. The commands are run with your real user id, so\n"
     "you cannot run any commands here requiring root permissions\n"
     "(unless, of course, you are root).\n\n"
     "Be sure to supply the whole path to the program otherwise\n"
     "kppp might be unable to find it."), parent);
  l->setMinimumHeight(l->sizeHint().height());
  tl->addWidget(l);
  tl->addStretch(1);

  QGridLayout *l1 = new QGridLayout( );
  l1->setMargin( 10 );
  tl->addLayout(l1);
  l1->setColumnStretch(0, 0);
  l1->setColumnStretch(1, 1);

  before_connect_l = new QLabel(i18n("&Before connect:"), parent);
  before_connect_l->setAlignment(Qt::AlignVCenter);
  l1->addWidget(before_connect_l, 0, 0);
  before_connect = new QLineEdit(parent);
  before_connect_l->setBuddy(before_connect);
  before_connect->setMaxLength(COMMAND_SIZE);
  l1->addWidget(before_connect, 0, 1);
  QString tmp = i18n("Allows you to run a program <b>before</b> a connection\n"
	     "is established. It is called immediately before\n"
	     "dialing has begun.\n\n"
	     "This might be useful, e.g. to stop HylaFAX blocking the\n"
	     "modem.");

  before_connect_l->setWhatsThis(tmp);
  before_connect->setWhatsThis(tmp);

  command_label = new QLabel(i18n("&Upon connect:"), parent);
  command_label->setAlignment(Qt::AlignVCenter);
  l1->addWidget(command_label, 1, 0);
  command = new QLineEdit(parent);
  command_label->setBuddy(command);
  command->setMaxLength(COMMAND_SIZE);
  l1->addWidget(command, 1, 1);
  tmp = i18n("Allows you to run a program <b>after</b> a connection\n"
	     "is established. When your program is called, all\n"
	     "preparations for an Internet connection are finished.\n"
	     "\n"
	     "Very useful for fetching mail and news");

  command_label->setWhatsThis(tmp);
  command->setWhatsThis(tmp);

  predisconnect_label = new QLabel(i18n("Before &disconnect:"),
				 parent);
  predisconnect_label->setAlignment(Qt::AlignVCenter);
  l1->addWidget(predisconnect_label, 2, 0);
  predisconnect = new QLineEdit(parent);
  predisconnect_label->setBuddy(predisconnect);
  predisconnect->setMaxLength(COMMAND_SIZE);
  l1->addWidget(predisconnect, 2, 1);
  tmp = i18n("Allows you to run a program <b>before</b> a connection\n"
	     "is closed. The connection will stay open until\n"
	     "the program exits.");

  predisconnect_label->setWhatsThis(tmp);
  predisconnect->setWhatsThis(tmp);

  discommand_label = new QLabel(i18n("U&pon disconnect:"),
			      parent);
  discommand_label->setAlignment(Qt::AlignVCenter);
  l1->addWidget(discommand_label, 3, 0);

  discommand = new QLineEdit(parent);
  discommand_label->setBuddy(discommand);
  discommand->setMaxLength(COMMAND_SIZE);
  l1->addWidget(discommand, 3, 1);
  tmp = i18n("Allows you to run a program <b>after</b> a connection\n"
	     "has been closed.");

  discommand_label->setWhatsThis(tmp);
  discommand->setWhatsThis(tmp);

  // extra space between entries
  l1->addItem(new QSpacerItem(0, 5), 1, 0);
  l1->addItem(new QSpacerItem(0, 5), 3, 0);

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
  : QWidget(parent)
{
  setObjectName(name);
  QVBoxLayout *topLayout = new QVBoxLayout(parent);
  topLayout->setSpacing(KDialog::spacingHint());

  box = new Q3GroupBox(1, Qt::Horizontal,i18n("C&onfiguration"), parent);
  box->setInsideSpacing(KDialog::spacingHint());

  rb = new Q3ButtonGroup(parent);
  rb->hide();
  connect(rb, SIGNAL(clicked(int)),
	  SLOT(hitIPSelect(int)));

  dynamicadd_rb = new QRadioButton(box);
  dynamicadd_rb->setText(i18n("Dynamic IP address"));
  dynamicadd_rb->setWhatsThis(
		  i18n("Select this option when your computer gets an\n"
		       "internet address (IP) every time a\n"
		       "connection is made.\n"
		       "\n"
		       "Almost every Internet Service Provider uses\n"
		       "this method, so this should be turned on."));

  staticadd_rb = new QRadioButton(box);
  staticadd_rb->setText(i18n("Static IP address"));
  rb->insert(dynamicadd_rb, 0);
  rb->insert(staticadd_rb, 1);
  staticadd_rb->setWhatsThis(
		  i18n("Select this option when your computer has a\n"
		       "fixed internet address (IP). Most computers\n"
		       "don't have this, so you should probably select\n"
		       "dynamic IP addressing unless you know what you\n"
		       "are doing."));

  QWidget *ipWidget = new QWidget(box);
  QGridLayout *ipLayout = new QGridLayout(ipWidget);
  ipLayout->setSpacing(KDialog::spacingHint());

  ipaddress_label = new QLabel(i18n("&IP address:"), ipWidget);
  QString tmp = i18n("If your computer has a permanent internet\n"
	     "address, you must supply your IP address here.");
  ipLayout->addWidget(ipaddress_label, 0, 0);

  ipaddress_l = new IPLineEdit(ipWidget);
  ipaddress_label->setBuddy(ipaddress_l);
  ipLayout->addWidget(ipaddress_l, 0, 1);

  ipaddress_label->setWhatsThis(tmp);
  ipaddress_l->setWhatsThis(tmp);

  sub_label = new QLabel(i18n("&Subnet mask:"), ipWidget);
  tmp = i18n("<p>If your computer has a static Internet address,\n"
	     "you must supply a network mask here. In almost\n"
	     "all cases this netmask will be <b>255.255.255.0</b>,\n"
	     "but your mileage may vary.\n"
	     "\n"
	     "If unsure, contact your Internet Service Provider");
  ipLayout->addWidget(sub_label, 1, 0);

  subnetmask_l = new IPLineEdit(ipWidget);
  sub_label->setBuddy(subnetmask_l);
  ipLayout->addWidget(subnetmask_l, 1, 1);

  sub_label->setWhatsThis(tmp);
  subnetmask_l->setWhatsThis(tmp);

  autoname = new QCheckBox(i18n("&Auto-configure hostname from this IP"), parent);
  autoname->setChecked(gpppdata.autoname());
  connect(autoname,SIGNAL(toggled(bool)),
	  this,SLOT(autoname_t(bool)));

  autoname->setWhatsThis(
		  i18n("<p>Whenever you connect, this reconfigures\n"
		       "your hostname to match the IP address you\n"
		       "got from the PPP server. This may be useful\n"
		       "if you need to use a protocol which depends\n"
		       "on this information, but it can also cause several\n"
		       "<a href=\"kppp-7.html#autohostname\">problems</a>.\n"
		       "\n"
		       "Do not enable this unless you really need it."));

  topLayout->addWidget(box);
  topLayout->addWidget(autoname);
  topLayout->addStretch();

  //load info from gpppdata
  if(!isnewaccount) {
    if(gpppdata.ipaddr() == "0.0.0.0" &&
       gpppdata.subnetmask() == "0.0.0.0") {
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

void IPWidget::autoname_t(bool on) {
  static bool was_warned = false;

  // big-fat warning when selecting the auto configure hostname option
  if(on && !was_warned) {
    KMessageBox::information(this,
			 i18n("Selecting this option might cause some weird "
			      "problems with the X-server and applications "
			      "while kppp is connected. Don't use it until "
			      "you know what you are doing!\n"
			      "For more information take a look at the "
			      "handbook (or help) in the section \"Frequently "
			      "asked questions\"."),
			 i18n("Warning"));
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



DNSWidget::DNSWidget( QWidget *parent, bool isnewaccount )
  : QWidget(parent)
{
  QVBoxLayout *topLayout = new QVBoxLayout(parent);
  topLayout->setSpacing(KDialog::spacingHint());
  topLayout->addWidget(this);

  //  box = new QGroupBox(this);
  QGridLayout *tl = new QGridLayout(this );
  tl->setSpacing( KDialog::spacingHint() );

  dnsdomain_label = new QLabel(i18n("Domain &name:"), this);
  tl->addWidget(dnsdomain_label, 0, 0);

  dnsdomain = new QLineEdit(this);
  dnsdomain_label->setBuddy(dnsdomain);
  dnsdomain->setMaxLength(DOMAIN_SIZE);
  tl->addWidget(dnsdomain, 0, 1);
  QString tmp = i18n("If you enter a domain name here, this domain\n"
		     "name is used for your computer while you are\n"
		     "connected. When the connection is closed, the\n"
		     "original domain name of your computer is\n"
		     "restored.\n"
		     "\n"
		     "If you leave this field blank, no changes are\n"
		     "made to the domain name.");

  dnsdomain_label->setWhatsThis(tmp);
  dnsdomain->setWhatsThis(tmp);

  conf_label = new QLabel(i18n("C&onfiguration:"), this);
  tl->addWidget(conf_label, 1, 0);

  bg = new Q3ButtonGroup("Group", this);
  conf_label->setBuddy(bg);
  connect(bg, SIGNAL(clicked(int)), SLOT(DNS_Mode_Selected(int)));
  bg->hide();

  autodns = new QRadioButton(i18n("Automatic"), this);
  bg->insert(autodns, 0);
  tl->addWidget(autodns, 1, 1);
  // no automatic DNS detection for pppd < 2.3.7
  if(!gpppdata.pppdVersionMin(2, 3, 7))
    autodns->setEnabled(false);

  mandns = new QRadioButton(i18n("Manual"), this);
  bg->insert(mandns, 1);
  tl->addWidget(mandns, 2, 1);

  dns_label = new QLabel(i18n("DNS &IP address:"), this);
  tl->addWidget(dns_label, 3, 0);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2, 3, 1);
  dnsipaddr = new IPLineEdit(this);
  dns_label->setBuddy(dnsipaddr);
  connect(dnsipaddr, SIGNAL(returnPressed()),
	  SLOT(adddns()));
  connect(dnsipaddr, SIGNAL(textChanged(const QString &)),
	  SLOT(DNS_Edit_Changed(const QString &)));
  l2->addWidget(dnsipaddr, 1);
  l2->addStretch(1);
  tmp = i18n("<p>Allows you to specify a new DNS server to be\n"
	     "used while you are connected. When the\n"
	     "connection is closed, this DNS entry will be\n"
	     "removed again.\n"
	     "\n"
	     "To add a DNS server, type in the IP address of\n"
	     "the DNS server here and click on <b>Add</b>");

  dns_label->setWhatsThis( tmp);
  dnsipaddr->setWhatsThis( tmp);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 4, 1);
  add = new QPushButton(i18n("&Add"), this);
  connect(add, SIGNAL(clicked()), SLOT(adddns()));
  int width = add->sizeHint().width();
  width = qMax(width,60);
  add->setMinimumWidth(width);
  l1->addWidget(add);
  l1->addStretch(1);
  add->setWhatsThis(
		  i18n("Click this button to add the DNS server\n"
		       "specified in the field above. The entry\n"
		       "will then be added to the list below"));

  remove = new QPushButton(i18n("&Remove"), this);
  connect(remove, SIGNAL(clicked()), SLOT(removedns()));
  width = remove->sizeHint().width();
  width = qMax(width,60);
  remove->setMinimumWidth(width);
  l1->addWidget(remove);
  remove->setWhatsThis(
		  i18n("Click this button to remove the selected DNS\n"
		       "server entry from the list below"));

  servers_label = new QLabel(i18n("DNS address &list:"), this);
  servers_label->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  tl->addWidget(servers_label, 5, 0);

  dnsservers = new Q3ListBox(this);
  servers_label->setBuddy(dnsservers);
  dnsservers->setMinimumSize(150, 80);
  connect(dnsservers, SIGNAL(highlighted(int)),
	  SLOT(DNS_Entry_Selected(int)));
  tl->addWidget(dnsservers, 5, 1);
  tmp = i18n("<p>This shows all defined DNS servers to use\n"
	     "while you are connected. Use the <b>Add</b> and\n"
	     "<b>Remove</b> buttons to modify the list");

  servers_label->setWhatsThis(tmp);
  dnsservers->setWhatsThis(tmp);

  exdnsdisabled_toggle = new QCheckBox(i18n( \
"&Disable existing DNS servers during connection"),
				     this);
  exdnsdisabled_toggle->setChecked(gpppdata.exDNSDisabled());
  tl->addWidget(exdnsdisabled_toggle, 6, 0, 1, 2, Qt::AlignCenter);
  exdnsdisabled_toggle->setWhatsThis(
		  i18n("<p>When this option is selected, all DNS\n"
		       "servers specified in <tt>/etc/resolv.conf</tt> are\n"
		       "temporary disabled while the dialup connection\n"
		       "is established. After the connection is\n"
		       "closed, the servers will be re-enabled\n"
		       "\n"
		       "Typically, there is no reason to use this\n"
		       "option, but it may become useful under \n"
		       "some circumstances."));


  // restore data if editing
  if(!isnewaccount) {
    dnsservers->insertStringList(gpppdata.dns());
    dnsdomain->setText(gpppdata.domain());
  }

  int mode = gpppdata.autoDNS() ? 0 : 1;
  bg->setButton(mode);
  DNS_Mode_Selected(mode);

  tl->activate();
}

void DNSWidget::DNS_Edit_Changed(const QString &text) {
  QRegExp r("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");
  add->setEnabled(text.contains(r));
}

void DNSWidget::DNS_Entry_Selected(int) {
  remove->setEnabled(true);
}

void DNSWidget::DNS_Mode_Selected(int mode) {
  bool on = (mode == 1);
  dns_label->setEnabled(on);
  servers_label->setEnabled(on);
  dnsipaddr->setText("");
  dnsipaddr->setEnabled(on);
  add->setEnabled(false);
  remove->setEnabled(dnsservers->count()>0 && on);
  dnsservers->clearSelection();
  dnsservers->setEnabled(on);
  dnsservers->triggerUpdate(false);
}

void DNSWidget::save() {
  gpppdata.setAutoDNS(bg->id(bg->selected()) == 0);
  QStringList serverlist;
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
  remove->setEnabled(dnsservers->count()>0);
}


//
// GatewayWidget
//
GatewayWidget::GatewayWidget( QWidget *parent, bool isnewaccount )
  : QWidget(parent)
{
  QVBoxLayout *topLayout = new QVBoxLayout(parent);
  topLayout->setSpacing(KDialog::spacingHint());

  box = new Q3GroupBox (1, Qt::Horizontal,i18n("C&onfiguration"), parent);
  box->setInsideSpacing(KDialog::spacingHint());

  rb = new Q3ButtonGroup(parent);
  rb->hide();
  connect(rb, SIGNAL(clicked(int)), SLOT(hitGatewaySelect(int)));

  defaultgateway = new QRadioButton(box);
  defaultgateway->setText(i18n("Default gateway"));
  rb->insert(defaultgateway, 0);
  defaultgateway->setWhatsThis(
		  i18n("This makes the PPP peer computer (the computer\n"
		       "you are connected to with your modem) to act as\n"
		       "a gateway. Your computer will send all packets not\n"
		       "going to a computer inside your local net to this\n"
		       "computer, which will route these packets.\n"
		       "\n"
		       "This is the default for most ISPs, so you should\n"
		       "probably leave this option on."));


  staticgateway = new QRadioButton(box);
  staticgateway->setText(i18n("Static gateway"));
  rb->insert(staticgateway, 1);
  staticgateway->setWhatsThis(
		  i18n("<p>Allows you to specify which computer you want\n"
		       "to use as gateway (see <i>Default Gateway</i> above)"));

  QWidget *gateBox = new QWidget(box);
  QHBoxLayout *hboxLayout1 = new QHBoxLayout(gateBox);
  gate_label = new QLabel(i18n("Gateway &IP address:"), gateBox);
  hboxLayout1->addWidget(gate_label);
  gatewayaddr = new IPLineEdit(gateBox);
  hboxLayout1->addWidget(gatewayaddr);
  gate_label->setBuddy(gatewayaddr);

  defaultroute = new QCheckBox(i18n("&Assign the default route to this gateway"),
			     parent);
  defaultroute->setWhatsThis(
		  i18n("If this option is enabled, all packets not\n"
		       "going to the local net are routed through\n"
		       "the PPP connection.\n"
		       "\n"
		       "Normally, you should turn this on"));

  topLayout->addWidget(box);
  topLayout->addWidget(defaultroute);
  topLayout->addStretch();

  //load info from gpppdata
  if(!isnewaccount) {
    if(gpppdata.gateway() == "0.0.0.0") {
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
  : QWidget(parent)
{
  setObjectName(name);
  QVBoxLayout *tl = new QVBoxLayout(parent);
  tl->setSpacing(KDialog::spacingHint());
  tl->setMargin(0);
  se = new ScriptEdit(parent);
  connect(se, SIGNAL(returnPressed()), SLOT(addButton()));
  tl->addWidget(se);

  // insert equal-sized buttons
  KDialogButtonBox *bbox = new KDialogButtonBox(parent);
  add = bbox->addButton(i18n("&Add"),QDialogButtonBox::ActionRole);
  connect(add, SIGNAL(clicked()), SLOT(addButton()));
  insert = bbox->addButton(i18n("&Insert"),QDialogButtonBox::ActionRole);
  connect(insert, SIGNAL(clicked()), SLOT(insertButton()));
  remove = bbox->addButton(i18n("&Remove"),QDialogButtonBox::ActionRole);
  connect(remove, SIGNAL(clicked()), SLOT(removeButton()));
  tl->addWidget(bbox);

  QHBoxLayout *l12 = new QHBoxLayout;
  tl->addLayout(l12);
  stl = new Q3ListBox(parent);
  stl->setVScrollBarMode( Q3ScrollView::AlwaysOff );
  connect(stl, SIGNAL(highlighted(int)), SLOT(stlhighlighted(int)));
  stl->setMinimumSize(QSize(70, 140));

  sl = new Q3ListBox(parent);
  sl->setVScrollBarMode( Q3ScrollView::AlwaysOff );
  connect(sl, SIGNAL(highlighted(int)), SLOT(slhighlighted(int)));
  sl->setMinimumSize(QSize(150, 140));

  slb = new QScrollBar(parent);
  slb->setFixedWidth(slb->sizeHint().width());
  connect(slb, SIGNAL(valueChanged(int)), SLOT(scrolling(int)));

  l12->addWidget(stl, 1);
  l12->addWidget(sl, 3);
  l12->addWidget(slb, 0);

  //load data from gpppdata
  if(!isnewaccount) {
    QStringList &comlist = gpppdata.scriptType();
    QStringList &arglist = gpppdata.script();
    QStringList::Iterator itcom = comlist.begin();
    QStringList::Iterator itarg = arglist.begin();

    for ( ;
          itcom != comlist.end() && itarg != arglist.end();
          ++itcom, ++itarg )
    {
      stl->insertItem(*itcom);
      sl->insertItem(*itarg);
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
	if(stl->text(i) == "LoopStart") {
		lstart++;
        }
	if (stl->text(i) == "LoopEnd") {
		lend++;
        }
	if ( lend > lstart ) errcnt++;
    }
    return ( (errcnt == 0 ) && (lstart == lend) );
  }
  return true;
}


void ScriptWidget::save() {
  QStringList typelist, arglist;
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

    default:
      break;
  }

  //get the scrollbar adjusted, and scroll the list so we can see what
  //we're adding to
  adjustScrollBar();
  slb->setValue(slb->maximum());

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
PhoneNumberDialog::PhoneNumberDialog(QWidget *parent)
    : KDialog(parent) {
    setCaption( i18n("Add Phone Number") );
    setButtons( Ok|Cancel );
    setDefaultButton( Ok );
    setModal( true );
  KWindowSystem::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(KIconLoader::Desktop),IconSize(KIconLoader::Desktop)), qApp->windowIcon().pixmap(IconSize(KIconLoader::Small),IconSize(KIconLoader::Small)));

  QWidget *hbox = new QWidget(this);
  QHBoxLayout *hboxLayout2 = new QHBoxLayout(hbox);
  hbox->setLayout(hboxLayout2);
  setMainWidget(hbox);

  hboxLayout2->setSpacing(KDialog::spacingHint());

  new QLabel(i18n("Enter a phone number:"), hbox);

  le = newLineEdit(14, hbox);
  le->setMinimumWidth(125);

  connect(le, SIGNAL(textChanged(const QString &)),
	  this, SLOT(textChanged(const QString &)));

  le->setFocus();
  textChanged("");

  showButtonSeparator(true);
}


QString PhoneNumberDialog::phoneNumber() {
  QString s = le->text();

  return s;
}


void PhoneNumberDialog::textChanged(const QString &s) {
  enableButtonOk(s.length() > 0);
}


#include "edit.moc"
