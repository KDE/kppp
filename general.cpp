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
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <termios.h>
#include <string.h>
#include <qwhatsthis.h>

#include <knuminput.h>
#include <qslider.h>
#include "general.h"
#include "version.h"
#include "miniterm.h"
#include "modeminfo.h"
#include "modemcmds.h"
#include "devices.h"
#include "pppdata.h"
#include <klocale.h>
#include <qlayout.h>
#include <qgrid.h>

/////////////////////////////////////////////////////////////////////////////
//
// Widget containing misc. configuration options
//
/////////////////////////////////////////////////////////////////////////////
GeneralWidget::GeneralWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QVBoxLayout *tl = new QVBoxLayout(parent, 0, KDialog::spacingHint());

  QHBoxLayout *hbox = new QHBoxLayout(tl);
  QLabel *label;
  label = new QLabel(i18n("pppd version:"), parent);
  hbox->addWidget(label);
  QString version = gpppdata.pppdVersion();
  if(version == "0.0.0")
    version = "unknown";
  label = new QLabel(version, parent);
  label->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  hbox->addWidget(label);

  KIntNumInput *pppdTimeout = new KIntNumInput(gpppdata.pppdTimeout(), parent);
  pppdTimeout->setLabel(i18n("pppd &timeout:"));
  pppdTimeout->setRange(1, TIMEOUT_SIZE, 5, true);
  pppdTimeout->setSuffix(i18n(" sec"));
  connect(pppdTimeout, SIGNAL(valueChanged(int)),
          SLOT(pppdtimeoutchanged(int)));
  tl->addWidget(pppdTimeout);
  QString tmp = i18n("<i>kppp</i> will wait this number of seconds\n"
		     "to see if a PPP connection is established.\n"
		     "If no connection is made in this time frame,\n"
		     "<i>kppp</i> will give up and kill pppd.");
  QWhatsThis::add(pppdTimeout,tmp);
  label->setBuddy(pppdTimeout);

  tl->addSpacing(10);

  QCheckBox *chkBox;
  chkBox = new QCheckBox(i18n("Doc&k into panel on connect"), parent);
  QWhatsThis::add(chkBox,
		  i18n("<p>After a connection is established, the\n"
		       "window is minimized and a small icon\n"
		       "in the KDE panel represents this window.\n"
		       "\n"
		       "Clicking on this icon will restore the\n"
		       "window to its original location and\n"
		       "size."));

  chkBox->setChecked(gpppdata.get_dock_into_panel());
  connect(chkBox,SIGNAL(toggled(bool)),
	  this, SLOT(docking_toggled(bool)));
  tl->addWidget(chkBox);

  chkBox = new QCheckBox(i18n("A&utomatic redial on disconnect"), parent);
  chkBox->setChecked(gpppdata.automatic_redial());
  connect(chkBox,SIGNAL(toggled(bool)),
	  this, SLOT(redial_toggled(bool)));
  tl->addWidget(chkBox);
  QWhatsThis::add(chkBox,
		  i18n("<p>When a connection is established and\n"
		       "it somehow gets disconnected, <i>kppp</i>\n"
		       "will try to reconnect to the same account.\n"
		       "\n"
		       "See <a href=\"#redial\">here</a> for more on this topic."));

  chkBox = new QCheckBox(i18n("&Show clock on caption"), parent);
  chkBox->setChecked(gpppdata.get_show_clock_on_caption());
  connect(chkBox, SIGNAL(toggled(bool)),
	  this, SLOT(caption_toggled(bool)));
  tl->addWidget(chkBox);
  QWhatsThis::add(chkBox,
		  i18n("When this option is checked, the window\n"
		       "title shows the time since a connection\n"
		       "was established. Very useful, so you \n"
		       "should turn this on"));

  chkBox = new QCheckBox(i18n("Disco&nnect on X server shutdown"), parent);
  chkBox->setChecked(gpppdata.get_xserver_exit_disconnect());
  connect(chkBox, SIGNAL(toggled(bool)),
	  this, SLOT(xserver_toggled(bool)));
  tl->addWidget(chkBox);
  QWhatsThis::add(chkBox,
		  i18n("<p>Checking this option will close any\n"
		       "open connection when the X-server is\n"
		       "shut down. You should enable this option\n"
		       "unless you know what you are doing.\n"
		       "\n"
		       "See <a href=\"#disxserver\">here</a> for more on this."));

  chkBox = new QCheckBox(i18n("&Quit on disconnect"), parent);
  chkBox->setChecked(gpppdata.quit_on_disconnect());
  connect(chkBox, SIGNAL(toggled(bool)),
	  this, SLOT(quit_toggled(bool)));
  tl->addWidget(chkBox);
  QWhatsThis::add(chkBox,
		  i18n("When this option is turned on, <i>kppp</i>\n"
		       "will be closed when you disconnect"));

  chkBox = new QCheckBox(i18n("Minimi&ze window on connect"), parent);
  chkBox->setChecked(gpppdata.get_iconify_on_connect());
  connect(chkBox,SIGNAL(toggled(bool)),
	  this,SLOT(iconify_toggled(bool)));
  tl->addWidget(chkBox);
  QWhatsThis::add(chkBox,
		  i18n("Iconifies <i>kppp</i>'s window when a\n"
		       "connection is established"));

  tl->addStretch();
}


void GeneralWidget::docking_toggled(bool on){
  gpppdata.set_dock_into_panel(on);
}


void GeneralWidget::iconify_toggled(bool on){
  gpppdata.set_iconify_on_connect(on);
}


void GeneralWidget::caption_toggled(bool on){
  gpppdata.set_show_clock_on_caption(on);
}


void GeneralWidget::redial_toggled(bool on){
  gpppdata.set_automatic_redial(on);
}


void GeneralWidget::xserver_toggled(bool on){
  gpppdata.set_xserver_exit_disconnect(on);
}


void GeneralWidget::quit_toggled(bool on){
  gpppdata.set_quit_on_disconnect(on);
}


void GeneralWidget::pppdtimeoutchanged(int n) {
  gpppdata.setpppdTimeout(n);

}


ModemWidget::ModemWidget( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  int k;

  QGridLayout *tl = new QGridLayout(parent, 8, 2, 0, KDialog::spacingHint());

  label1 = new QLabel(i18n("Modem de&vice:"), parent);
  tl->addWidget(label1, 0, 0);

  modemdevice = new QComboBox(true, parent);
  label1->setBuddy(modemdevice);
  bool deviceExist = false;
  for(k = 0; devices[k]; k++)
  {
      if ( devices[k] == gpppdata.modemDevice())
          deviceExist = true;
    modemdevice->insertItem(devices[k]);
  }
  if ( !deviceExist )
      modemdevice->insertItem(gpppdata.modemDevice());

  tl->addWidget(modemdevice, 0, 1);
  connect(modemdevice, SIGNAL(activated(int)),
	  SLOT(setmodemdc(int)));
  QString tmp = i18n("This specifies the serial port your modem is attached \n"
		     "to. On Linux/x86, typically this is either /dev/ttyS0 \n"
		     "(COM1 under DOS) or /dev/ttyS1 (COM2 under DOS).\n"
		     "\n"
		     "If you have an internal ISDN card with AT command\n"
		     "emulation (most cards under Linux support this), you\n"
		     "should select one of the /dev/ttyIx devices.");

  QWhatsThis::add(label1,tmp);
  QWhatsThis::add(modemdevice,tmp);


  label2 = new QLabel(i18n("&Flow control:"), parent);
  tl->addWidget(label2, 1, 0);

  flowcontrol = new QComboBox(false, parent);
  label2->setBuddy(flowcontrol);
  flowcontrol->insertItem(i18n("Hardware [CRTSCTS]"));
  flowcontrol->insertItem(i18n("Software [XON/XOFF]"));
  flowcontrol->insertItem(i18n("None"));
  tl->addWidget(flowcontrol, 1, 1);
  connect(flowcontrol, SIGNAL(activated(int)),
	  SLOT(setflowcontrol(int)));

  tmp = i18n("<p>Specifies how the serial port and modem\n"
	     "communicate. You should not change this unless\n"
	     "you know what you are doing.\n"
	     "\n"
	     "<b>Default</b>: CRTSCTS");

  QWhatsThis::add(label2,tmp);
  QWhatsThis::add(flowcontrol,tmp);

  labelenter = new QLabel(i18n("&Line termination:"), parent);
  tl->addWidget(labelenter, 2, 0);

  enter = new QComboBox(false, parent);
  labelenter->setBuddy(enter);
  enter->insertItem("CR");
  enter->insertItem("LF");
  enter->insertItem("CR/LF");
  tl->addWidget(enter, 2, 1);
  connect(enter, SIGNAL(activated(int)), SLOT(setenter(int)));
  tmp = i18n("<p>Specifies how AT commands are sent to your\n"
	     "modem. Most modems will work fine with the\n"
	     "default <i>CR/LF</i>. If your modem does not react\n"
	     "to the init string, you should try different\n"
	     "settings here\n"
	     "\n"
	     "<b>Default</b>: CR/LF");

  QWhatsThis::add(labelenter,tmp);
  QWhatsThis::add(enter, tmp);

  baud_label = new QLabel(i18n("Co&nnection speed:"), parent);
  tl->addWidget(baud_label, 3, 0);
  baud_c = new QComboBox(parent);
  baud_label->setBuddy(baud_c);

  static const char *baudrates[] = {

#ifdef B460800
    "460800",
#endif

#ifdef B230400
    "230400",
#endif

#ifdef B115200
    "115200",
#endif

#ifdef B57600
    "57600",
#endif

    "38400",
    "19200",
    "9600",
    "2400",
    0};

  for(k = 0; baudrates[k]; k++)
    baud_c->insertItem(baudrates[k]);

  baud_c->setCurrentItem(3);
  connect(baud_c, SIGNAL(activated(int)),
	  this, SLOT(speed_selection(int)));
  tl->addWidget(baud_c, 3, 1);

  tmp = i18n("Specifies the speed your modem and the serial\n"
	     "port talk to each other. You should begin with\n"
	     "at least 115200 bits/sec (or more if you know\n"
	     "that your serial port supports higher speeds).\n"
	     "If you have connection problems, try to reduce\n"
	     "this value.");

  QWhatsThis::add(baud_label,tmp);
  QWhatsThis::add(baud_c,tmp);

  for(int i=0; i <= enter->count()-1; i++) {
    if(gpppdata.enter() == enter->text(i))
      enter->setCurrentItem(i);
  }

  tl->addRowSpacing(4, 10);

  //Modem Lock File
  modemlockfile = new QCheckBox(i18n("&Use lock file"), parent);

  modemlockfile->setChecked(gpppdata.modemLockFile());
  connect(modemlockfile, SIGNAL(toggled(bool)),
          SLOT(modemlockfilechanged(bool)));
  tl->addMultiCellWidget(modemlockfile, 5, 5, 0, 1);
  //  l12->addStretch(1);
  QWhatsThis::add(modemlockfile,
		  i18n("<p>To prevent other programs from accessing the\n"
		       "modem while a connection is established, a\n"
		       "file can be created to indicate that the modem\n"
		       "is in use. On Linux an example file would be\n"
                       "<tt>/var/lock/LCK..ttyS1</tt>\n"
                       "Here you can select whether this locking will\n"
		       "be done.\n"
		       "\n"
                       "<b>Default</b>: On"));

  // Modem Timeout Line Edit Box

  modemtimeout = new KIntNumInput(gpppdata.modemTimeout(), parent);
  modemtimeout->setLabel(i18n("Modem &timeout:"));
  modemtimeout->setRange(1, 120, 1);
  modemtimeout->setSuffix(i18n(" sec"));
  connect(modemtimeout, SIGNAL(valueChanged(int)),
	  SLOT(modemtimeoutchanged(int)));
  tl->addMultiCellWidget(modemtimeout, 6, 6, 0, 1);

  QWhatsThis::add(modemtimeout,
                  i18n("This specifies how long <i>kppp</i> waits for a\n"
                       "<i>CONNECT</i> response from your modem. The\n"
                       "recommended value is 30 seconds."));

  //set stuff from gpppdata
  for(int i=0; i <= enter->count()-1; i++) {
    if(gpppdata.enter() == enter->text(i))
      enter->setCurrentItem(i);
  }

  for(int i=0; i <= modemdevice->count()-1; i++) {
    if(gpppdata.modemDevice() == modemdevice->text(i))
      modemdevice->setCurrentItem(i);
  }

  for(int i=0; i <= flowcontrol->count()-1; i++) {
    if(gpppdata.flowcontrol() == flowcontrol->text(i))
      flowcontrol->setCurrentItem(i);
  }

  //set the modem speed
  for(int i=0; i < baud_c->count(); i++)
    if(baud_c->text(i) == gpppdata.speed())
      baud_c->setCurrentItem(i);

  tl->setRowStretch(7, 1);
}


void ModemWidget::speed_selection(int) {
  gpppdata.setSpeed(baud_c->text(baud_c->currentItem()));
}


void ModemWidget::setenter(int ) {
  gpppdata.setEnter(enter->text(enter->currentItem()));
}


void ModemWidget::setmodemdc(int i) {
  gpppdata.setModemDevice(modemdevice->text(i));
}


void ModemWidget::setflowcontrol(int i) {
  gpppdata.setFlowcontrol(flowcontrol->text(i));
}


void ModemWidget::modemlockfilechanged(bool set) {
  gpppdata.setModemLockFile(set);
}


void ModemWidget::modemtimeoutchanged(int n) {
  gpppdata.setModemTimeout(n);
}


ModemWidget2::ModemWidget2( QWidget *parent, const char *name)
  : QWidget(parent, name)
{
  QVBoxLayout *l1 = new QVBoxLayout(parent, 0, KDialog::spacingHint());


  waitfordt = new QCheckBox(i18n("&Wait for dial tone before dialing"), parent);
  waitfordt->setChecked(gpppdata.waitForDialTone());
  connect(waitfordt, SIGNAL(toggled(bool)), SLOT(waitfordtchanged(bool)));
  l1->addWidget(waitfordt);
  QWhatsThis::add(waitfordt,
		  i18n("<p>Normally the modem waits for a dial tone\n"
		       "from your phone line, indicating that it can\n"
		       "start to dial a number. If your modem does not\n"
		       "recognize this sound, or your local phone system\n"
		       "does not emit such a tone, uncheck this option\n"
		       "\n"
		       "<b>Default:</b>: On"));

  busywait = new KIntNumInput(gpppdata.busyWait(), parent);
  busywait->setLabel(i18n("B&usy wait:"));
  busywait->setRange(0, 300, 5, true);
  busywait->setSuffix(i18n(" sec"));
  connect(busywait, SIGNAL(valueChanged(int)), SLOT(busywaitchanged(int)));
  l1->addWidget(busywait);

  QWhatsThis::add(busywait,
                  i18n("Specifies the number of seconds to wait before\n"
                       "redial if all dialed numbers are busy. This is\n"
                       "necessary because some modems get stuck if the\n"
                       "same number is busy too often.\n"
                       "\n"
                       "The default is 0 seconds, you should not change\n"
                       "this unless you need to."));

  l1->addSpacing(10);

  QHBoxLayout *hbl = new QHBoxLayout;
  hbl->setSpacing(KDialog::spacingHint());

  QLabel *volumeLabel = new QLabel(i18n("Modem &volume:"), parent);
  hbl->addWidget(volumeLabel);
  volume = new QSlider(0, 2, 1, gpppdata.volume(), QSlider::Horizontal, parent);
  volumeLabel->setBuddy(volume);
  volume->setTickmarks(QSlider::Below);
  hbl->addWidget(volume);

  l1->addLayout(hbl);

  connect(volume, SIGNAL(valueChanged(int)),
	  this, SLOT(volumeChanged(int)));
  QString tmp = i18n("Most modems have a speaker which makes\n"
	     "a lot of noise when dialing. Here you can\n"
	     "either turn this completely off or select a\n"
	     "lower volume.\n"
	     "\n"
	     "If this does not work for your modem,\n"
	     "you must modify the modem volume command.");

  QWhatsThis::add(volumeLabel,tmp);
  QWhatsThis::add(volume, tmp);

  l1->addSpacing(20);

#if 0
  chkbox1 = new QCheckBox(i18n("Modem asserts CD line"), parent);
  chkbox1->setChecked(gpppdata.UseCDLine());
  connect(chkbox1,SIGNAL(toggled(bool)),
	  this,SLOT(use_cdline_toggled(bool)));
  l12->addWidget(chkbox1);
  l12->addStretch(1);
  l1->addStretch(1);
  QWhatsThis::add(chkbox1,
		  i18n("This controls how <i>kppp</i> detects that the modem\n"
		       "is not responding. Unless you are having\n"
		       "problems with this, do not modify this setting.\n"
		       "\n"
		       "<b>Default</b>: Off"));
#endif

  modemcmds = new QPushButton(i18n("Mod&em Commands..."), parent);
  QWhatsThis::add(modemcmds,
		  i18n("Allows you to change the AT command for\n"
		       "your modem."));

  modeminfo_button = new QPushButton(i18n("&Query Modem..."), parent);
  QWhatsThis::add(modeminfo_button,
		  i18n("Most modems support the ATI command set to\n"
		       "find out vendor and revision of your modem.\n"
		       "\n"
		       "Press this button to query your modem for\n"
		       "this information. It can be useful to help\n"
		       "you setup the modem"));

  terminal_button = new QPushButton(i18n("&Terminal..."), parent);
  QWhatsThis::add(terminal_button,
		  i18n("Opens the built-in terminal program. You\n"
		       "can use this if you want to play around\n"
		       "with your modem's AT command set"));

  QHBoxLayout *hbox = new QHBoxLayout();
  l1->addLayout(hbox);
  hbox->addStretch(1);
  QVBoxLayout *vbox = new QVBoxLayout();
  hbox->addLayout(vbox);

  vbox->addWidget(modemcmds);
  vbox->addWidget(modeminfo_button);
  vbox->addWidget(terminal_button);

  hbox->addStretch(1);
  l1->addStretch(1);

  connect(modemcmds, SIGNAL(clicked()),
	  SLOT(modemcmdsbutton()));
  connect(modeminfo_button, SIGNAL(clicked()),
	  SLOT(query_modem()));
  connect(terminal_button, SIGNAL(clicked()),
	  SLOT(terminal()));
}


void ModemWidget2::modemcmdsbutton() {
  ModemCommands mc(this);
  mc.exec();
}


void ModemWidget2::query_modem() {
  ModemTransfer mt(this);
  mt.exec();
}


void ModemWidget2::terminal() {
  MiniTerm terminal(NULL,NULL);
  terminal.exec();
}


#if 0
void ModemWidget2::use_cdline_toggled(bool on) {
    gpppdata.setUseCDLine(on);
}
#endif

void ModemWidget2::waitfordtchanged(bool b) {
  gpppdata.setWaitForDialTone((int)b);
}

void ModemWidget2::busywaitchanged(int n) {
  gpppdata.setbusyWait(n);
}


void ModemWidget2::volumeChanged(int v) {
  gpppdata.setVolume(v);
}


/////////////////////////////////////////////////////////////////////////////
//
// Setup widget for the graph
//
/////////////////////////////////////////////////////////////////////////////
GraphSetup::GraphSetup(QWidget *parent, const char *name) :
  QWidget(parent, name)
{
  QVBoxLayout *tl = new QVBoxLayout(parent);

  bool enable;
  QColor bg, text, in, out;
  gpppdata.graphingOptions(enable, bg, text, in, out);

  enable_check = new QCheckBox(i18n("&Enable throughput graph"), parent);
  tl->addWidget(enable_check);

  grpColor = new QGroupBox(2, Qt::Horizontal,
        i18n("Graph Colors"), parent);
  tl->addWidget(grpColor);

  QLabel *label;

  label = new QLabel(i18n("Bac&kground:"), grpColor);
  bg_color = new KColorButton(bg, grpColor);
  bg_color->setFixedSize(80, 24);
  label->setBuddy(bg_color);

  label = new QLabel(i18n("&Text:"), grpColor);
  text_color = new KColorButton(text, grpColor);
  text_color->setFixedSize(80, 24);
  label->setBuddy(text_color);

  label = new QLabel(i18n("I&nput bytes:"), grpColor);
  in_color = new KColorButton(in, grpColor);
  in_color->setFixedSize(80, 24);
  label->setBuddy(in_color);

  label = new QLabel(i18n("O&utput bytes:"), grpColor);
  out_color = new KColorButton(out, grpColor);
  out_color->setFixedSize(80, 24);
  label->setBuddy(out_color);

  tl->addStretch();

  connect(enable_check, SIGNAL(toggled(bool)), this, SLOT(enableToggled(bool)));
  connect(bg_color, SIGNAL(changed(const QColor &)),
	  SLOT(colorChanged(const QColor&)));
  connect(text_color, SIGNAL(changed(const QColor &)),
	  SLOT(colorChanged(const QColor&)));
  connect(in_color, SIGNAL(changed(const QColor &)),
	  SLOT(colorChanged(const QColor&)));
  connect(out_color, SIGNAL(changed(const QColor &)),
	  SLOT(colorChanged(const QColor&)));

  tl->activate();

  enable_check->setChecked(enable);
  enableToggled(enable);
}

void GraphSetup::enableToggled(bool b) {
  grpColor->setEnabled(b);
  save();
}


void GraphSetup::colorChanged(const QColor &) {
  save();
}

void GraphSetup::save() {
  gpppdata.setGraphingOptions(enable_check->isChecked(),
			      bg_color->color(),
			      text_color->color(),
			      in_color->color(),
			      out_color->color());
}

#include "general.moc"
