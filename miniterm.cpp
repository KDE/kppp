/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 *
 * Copyright (C) 1997 Bernd Johannes Wuebben
 *                    wuebben@math.cornell.edu
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
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>
#include <kwindowsystem.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include "pppdata.h"
#include "modem.h"
#include "miniterm.h"
#include <klocale.h>
#include <kmenu.h>
#include <kglobalsettings.h>
#include <kmenubar.h>
#include <kicon.h>
#include <KStandardGuiItem>
#include <kapplication.h>
#include <kguiitem.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <QMenu>
//Added by qt3to4:
#include <QVBoxLayout>
#include <Q3Frame>
#include <QKeyEvent>
#include <QCloseEvent>
#include <ktoolinvocation.h>


extern PPPData gpppdata;

MiniTerm::MiniTerm(QWidget *parent, const char *name)
  : QDialog(parent)
{
  setObjectName(name);
  setModal(true);
  setWindowTitle(i18n("Kppp Mini-Terminal"));
  KWindowSystem::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));

  m_file = new QMenu(this);
  m_file->addAction( SmallIcon("window-close"), i18n("&Close"), this, SLOT(cancelbutton()) );
  m_options = new QMenu(this);
  m_options->addAction(i18n("&Reset Modem"),this,SLOT(resetModem()));
  m_help =
    new KHelpMenu(this,
		      i18n("MiniTerm - A terminal emulation for KPPP\n\n"
			   "(c) 1997 Bernd Johannes Wuebben <wuebben@kde.org>\n"
			   "(c) 1998 Harri Porten <porten@kde.org>\n"
			   "(c) 1998 Mario Weilguni <mweilguni@kde.org>\n\n"
			   "This program is published under the GNU GPL\n"
			   "(GNU General Public License)"
			   ));

  menubar = new KMenuBar(this);
  m_file->setTitle(i18n("&File"));
  menubar->addMenu( m_file );
  m_options->setTitle(i18n("&Modem"));
  menubar->addMenu( m_options );
  m_help->menu()->setTitle(KStandardGuiItem::help().text());
  menubar->addMenu( m_help->menu());

  statusbar = new QLabel(this);
  statusbar->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);

  terminal = new MyTerm(this, "term");

  setupToolbar();

  QVBoxLayout *layout=new QVBoxLayout(this);
  layout->addWidget(menubar);
  layout->addWidget(toolbar);
  layout->addWidget(terminal);
  layout->addWidget(statusbar);

  inittimer = new QTimer(this);
  connect(inittimer,SIGNAL(timeout()),this,SLOT(init()));
  inittimer->start(500);

  resize(550,400);
}


MiniTerm::~MiniTerm() {
  delete toolbar;
  delete statusbar;
}


void MiniTerm::setupToolbar() {
  toolbar = new KToolBar( this );

  toolbar->addAction( KIcon("application-exit"), i18n("Close MiniTerm"), this, SLOT(cancelbutton()));
  toolbar->addAction( KIcon("go-previous"), i18n("Reset Modem"), this, SLOT(resetModem()));
  toolbar->addAction( KIcon("help-contents"), i18n("Help"), this, SLOT(help()));

  toolbar->setMovable(false);
}


void MiniTerm::init() {
  inittimer->stop();
  statusbar->setText(i18n("Initializing Modem"));
  qApp->processEvents();

  int lock = Modem::modem->lockdevice();
  if (lock == 1) {
    statusbar->setText(i18n("Modem device is locked."));
    return;
  }

  if (lock == -1) {
    statusbar->setText(i18n("Unable to create modem lock file."));
    return;
  }

  if(Modem::modem->opentty()) {
    if(Modem::modem->hangup()) {
    // send a carriage return and then wait a bit so that the modem will
    // let us issue commands.
    if(gpppdata.modemPreInitDelay() > 0) {
      usleep(gpppdata.modemPreInitDelay() * 5000);
      Modem::modem->writeLine("");
      usleep(gpppdata.modemPreInitDelay() * 5000);
    }
    Modem::modem->writeLine(gpppdata.modemInitStr(0).toLocal8Bit());
    usleep(gpppdata.modemInitDelay() * 10000);

      statusbar->setText(i18n("Modem Ready"));
      terminal->setFocus();

      qApp->processEvents();
      qApp->processEvents();

      Modem::modem->notify(this, SLOT(readChar(unsigned char)));
      return;
    }
  }

  // opentty() or hangup() failed
  statusbar->setText(Modem::modem->modemMessage());
  Modem::modem->unlockdevice();
}


void MiniTerm::readChar(unsigned char c) {

  switch((int)c) {
  case 8:
    terminal->backspace();
    break;
  case 10:
    terminal->mynewline();
    break;
  case 13:
    terminal->myreturn();
    break;
  case 127:
    terminal->backspace();
    break;
  default:
    terminal->insertChar(c);
  }
}


void MiniTerm::cancelbutton() {
  Modem::modem->stop();

  statusbar->setText(i18n("Hanging up..."));
  qApp->processEvents();
  KApplication::flush();

  Modem::modem->hangup();

  Modem::modem->closetty();
  Modem::modem->unlockdevice();

  reject();
}


void MiniTerm::resetModem() {
  statusbar->setText(i18n("Resetting Modem"));
  terminal->newLine();
  qApp->processEvents();
  KApplication::flush();

  Modem::modem->hangup();

  statusbar->setText(i18n("Modem Ready"));
}


void MiniTerm::closeEvent( QCloseEvent *e ) {
  cancelbutton();
  e->accept();
}


void MiniTerm::help() {
  KToolInvocation::invokeHelp();
}


MyTerm::MyTerm(QWidget *parent, const char* name)
  : Q3MultiLineEdit(parent, name)
{
   setFont(KGlobalSettings::fixedFont());
}

void MyTerm::keyPressEvent(QKeyEvent *k) {
  // ignore meta keys
  if (k->ascii() == 0)
    return;

  if(k->ascii() == 13)
    myreturn();

  Modem::modem->writeChar((unsigned char) k->ascii());
}


void MyTerm::insertChar(unsigned char c) {
	Q3MultiLineEdit::insert(QString(c));
}


void MyTerm::newLine() {
  Q3MultiLineEdit::newLine();
}


void MyTerm::del() {
  Q3MultiLineEdit::del();
}


void MyTerm::backspace() {
  Q3MultiLineEdit::backspace();
}


void MyTerm::myreturn() {
  int column;
  int line;

  getCursorPosition(&line,&column);
  for (int i = 0; i < column;i++)
    Q3MultiLineEdit::cursorLeft();
}


void MyTerm::mynewline() {
  Q3MultiLineEdit::end(FALSE);
  Q3MultiLineEdit::newLine();
}

#include "miniterm.moc"
