/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 *            Copyright (C) 1998-2002 Harri Porten <porten@kde.org>
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

#include "kpppwidget.h"

#include <qapplication.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qevent.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <kdialogbase.h>
#include <qwhatsthis.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kbuttonbox.h>
#include <kglobalsettings.h>
#include <kcmdlineargs.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kiconloader.h> // For BarIcon
#include <klocale.h>
#include <kmessagebox.h>
#include <kseparator.h>
#include <kstandarddirs.h>
#include <kwin.h>
#include <khelpmenu.h>
#include <kpushbutton.h>
#include <kguiitem.h>

#include <stdlib.h>
#include <errno.h>

#include "runtests.h"

#include "main.h"
#include "auth.h"
#include "modem.h"
#include "ppplog.h"
#include "opener.h"
#include "requester.h"
#include "pppstats.h"
#include "pppdata.h"
#include "general.h"


extern KPPPWidget *p_kppp;

KPPPWidget::KPPPWidget( QWidget *parent, const char *name )
  : QWidget(parent, name)
  , acct(0)
  , m_bCmdlAccount (false)
{
  tabWindow = 0;

  // before doing anything else, run a few tests

  int result = runTests();
  if(result == TEST_CRITICAL)
    exit(4);

  installEventFilter(this);

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);

  QGridLayout *l1 = new QGridLayout(3, 4);
  tl->addLayout(l1);
  l1->addColSpacing(0, 10);
  l1->addColSpacing(3, 10);
  l1->setColStretch(1, 3);
  l1->setColStretch(2, 4);

  label1 = new QLabel(i18n("C&onnect to: "), this);
  l1->addWidget(label1, 0, 1);

  connectto_c = new QComboBox(false, this);
  label1->setBuddy(connectto_c);

  connect(connectto_c, SIGNAL(activated(int)),
	  SLOT(newdefaultaccount(int)));
  l1->addWidget(connectto_c, 0, 2);

  ID_Label = new QLabel(i18n("&Login ID:"), this);
  l1->addWidget(ID_Label, 1, 1);

  // the entry line for usernames
  ID_Edit = new QLineEdit(this);
  ID_Label->setBuddy(ID_Edit);
  l1->addWidget(ID_Edit, 1, 2);
  connect(ID_Edit, SIGNAL(returnPressed()),
	  this, SLOT(enterPressedInID()));
  QString tmp = i18n("<p>Type in the username that you got from your\n"
		     "ISP. This is especially important for PAP\n"
		     "and CHAP. You may omit this when you use\n"
		     "terminal-based or script-based authentication.\n"
		     "\n"
		     "<b>Important</b>: case is important here:\n"
		     "<i>myusername</i> is not the same as <i>MyUserName</i>!");

  QWhatsThis::add(ID_Label,tmp);
  QWhatsThis::add(ID_Edit,tmp);

  PW_Label = new QLabel(i18n("&Password:"), this);
  l1->addWidget(PW_Label, 2, 1);

  PW_Edit= new QLineEdit(this);
  PW_Label->setBuddy(PW_Edit);
  PW_Edit->setEchoMode(QLineEdit::Password);
  l1->addWidget(PW_Edit, 2, 2);
  connect(PW_Edit, SIGNAL(returnPressed()),
	  this, SLOT(enterPressedInPW()));

  tmp = i18n("<p>Type in the password that you got from your\n"
	     "ISP. This is especially important for PAP\n"
	     "and CHAP. You may omit this when you use\n"
	     "terminal-based or script-based authentication.\n"
	     "\n"
	     "<b>Important</b>: case is important here:\n"
	     "<i>mypassword</i> is not the same as <i>MyPassword</i>!");

  QWhatsThis::add(PW_Label,tmp);
  QWhatsThis::add(PW_Edit,tmp);

   QHBoxLayout *l3 = new QHBoxLayout;
   tl->addSpacing(5);
   tl->addLayout(l3);
   tl->addSpacing(5);
   l3->addSpacing(10);
   log = new QCheckBox(i18n("Show lo&g window"), this);
   connect(log, SIGNAL(toggled(bool)),
  	  this, SLOT(log_window_toggled(bool)));
   log->setChecked(gpppdata.get_show_log_window());
   l3->addWidget(log);

   QWhatsThis::add(log,
 		  i18n("<p>This controls whether a log window is shown.\n"
 		       "A log window shows the communication between\n"
 		       "<i>kppp</i> and your modem. This will help you\n"
 		       "in tracking down problems.\n"
 		       "\n"
 		       "Turn it off if <i>kppp</i> routinely connects without\n"
 		       "problems"));

  fline = new KSeparator( KSeparator::HLine, this);
  tl->addWidget(fline);

  QHBoxLayout *l2 = new QHBoxLayout;
  tl->addLayout(l2);

  int minw = 0;
  quit_b = new KPushButton(KGuiItem(i18n("&Quit"), "exit" ), this);
  connect( quit_b, SIGNAL(clicked()), SLOT(quitbutton()));
  if(quit_b->sizeHint().width() > minw)
      minw = quit_b->sizeHint().width();

  setup_b = new KPushButton(KGuiItem(i18n("&Configure...")), this);
  connect( setup_b, SIGNAL(clicked()), SLOT(expandbutton()));
  if(setup_b->sizeHint().width() > minw)
      minw = setup_b->sizeHint().width();

  if(gpppdata.access() != KConfig::ReadWrite)
    setup_b->setEnabled(false);

  help_b = new KPushButton(i18n("&Help"), this);
  connect( help_b, SIGNAL(clicked()), SLOT(helpbutton()));

  KHelpMenu *helpMenu = new KHelpMenu(this, KGlobal::instance()->aboutData(), true);
  help_b->setPopup((QPopupMenu*)helpMenu->menu());
  help_b->setGuiItem (KGuiItem(i18n("&Help"), "help" ) );

  if(help_b->sizeHint().width() > minw)
      minw = help_b->sizeHint().width();

  connect_b = new QPushButton(i18n("&Connect"), this);
  connect_b->setDefault(true);
  connect_b->setFocus();
  connect(connect_b, SIGNAL(clicked()), SLOT(beginConnect()));
  if(connect_b->sizeHint().width() > minw)
      minw = connect_b->sizeHint().width();

  quit_b->setFixedWidth(minw);
  setup_b->setFixedWidth(minw);
  help_b->setFixedWidth(help_b->sizeHint().width());
  connect_b->setFixedWidth(minw);

  l2->addWidget(quit_b);
  l2->addWidget(setup_b);
  l2->addWidget(help_b);
  l2->addSpacing(20);
  l2->addWidget(connect_b);

  setFixedSize(sizeHint());

  (void)new Modem;

  // we also connect cmld_start to the beginConnect so that I can run
  // the dialer through a command line argument
  connect(this,SIGNAL(cmdl_start()),this,SLOT(beginConnect()));

  stats = new PPPStats;

  KWin::setIcons(winId(), kapp->icon(), kapp->miniIcon());

  // constructor of con_win reads position from config file
  con_win = new ConWindow(0, "conw", this, stats);
  KWin::setIcons(con_win->winId(), kapp->icon(), kapp->miniIcon());

  statdlg = new PPPStatsDlg(0, "stats", this, stats);
  statdlg->hide();

  debugwindow = new DebugWidget(0,"debugwindow");
  KWin::setIcons(debugwindow->winId(), kapp->icon(), kapp->miniIcon());
  debugwindow->hide();

  // load up the accounts combo box

  resetaccounts();
  con = new ConnectWidget(0, "con", stats);
  KWin::setIcons(con->winId(), kapp->icon(), kapp->miniIcon() );
  connect(this, SIGNAL(begin_connect()),con, SLOT(preinit()));

  QRect desk = KGlobalSettings::desktopGeometry(topLevelWidget());
  con->setGeometry(desk.center().x()-175, desk.center().y()-55, 350,110);

  // connect the ConnectWidgets various signals
  connect(con, SIGNAL(closeDebugWindow()),
	  debugwindow, SLOT(hide()));
  connect(con, SIGNAL(debugMessage(const QString &)),
	  debugwindow, SLOT(statusLabel(const QString &)));
  connect(con, SIGNAL(toggleDebugWindow()),
	  debugwindow, SLOT(toggleVisibility()));
  connect(con, SIGNAL(debugPutChar(unsigned char)),
	  debugwindow, SLOT(addChar(unsigned char)));
  connect(con, SIGNAL(startAccounting()),
	  this, SLOT(startAccounting()));
  connect(con, SIGNAL(stopAccounting()),
	  this, SLOT(stopAccounting()));
  connect(KApplication::kApplication(), SIGNAL(saveYourself()),
	  this, SLOT(saveMyself()));
  connect(KApplication::kApplication(), SIGNAL(shutDown()),
	  this, SLOT(shutDown()));

  debugwindow->setGeometry(desk.center().x()+190, desk.center().y()-55,
			   debugwindow->width(),debugwindow->height());

  move(desk.center().x()-width()/2, desk.center().y()-height()/2);


  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  m_strCmdlAccount = args->getOption("c");
  m_bQuitOnDisconnect = args->isSet("q");

  if(!m_strCmdlAccount.isEmpty()) {
    m_bCmdlAccount = true;
    kdDebug(5002) << "cmdl_account: " << m_bCmdlAccount << endl;
  }

  if(m_bCmdlAccount){
    bool result = gpppdata.setAccount(m_strCmdlAccount);
    if (!result){
      QString string;
      string = i18n("No such Account:\n%1").arg(m_strCmdlAccount);
      KMessageBox::error(this, string);
      m_bCmdlAccount = false;
      this->show();
    } else {
      beginConnect();
    }
  } else
    show();


//#define KPPP_SHOW_NEWS
#ifdef KPPP_SHOW_NEWS
  // keep user informed about recent changes
  if(!m_bCmdlAccount)
    showNews();
#endif
}

KPPPWidget::~KPPPWidget()
{
  delete stats;
}

bool KPPPWidget::eventFilter(QObject *o, QEvent *e) {
  if(e->type() == QEvent::User) {
    switch(((SignalEvent*)e)->sigType()) {
    case SIGINT:
      kdDebug(5002) << "Received a SIGINT" << endl;
      interruptConnection();
      break;
    case SIGCHLD:
      sigChld();
      break;
    case SIGUSR1:
      sigPPPDDied();
      break;
    }
    return true;
  }

    if(o == connect_b) {
      if(e->type() == QEvent::KeyPress) {
        if(connect_b->hasFocus() && ((QKeyEvent *)e)->key() == Qt::Key_Return) {
          beginConnect();
          return true;
        }
      }
    }

    return false;
}

void KPPPWidget::prepareSetupDialog() {
  if(tabWindow == 0) {
    tabWindow = new KDialogBase( KDialogBase::Tabbed, i18n("KPPP Configuration"),
                                 KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok,
                                 kapp->mainWidget(), 0, true);


    KWin::setIcons(tabWindow->winId(), kapp->icon(), kapp->miniIcon());

    //    tabWindow->setFixedSize( 365, 375 );

    accounts = new AccountWidget(tabWindow->addPage( i18n("&Accounts"), i18n("Account Setup") ) );
    connect(accounts, SIGNAL(resetaccounts()),
	    this, SLOT(resetaccounts()));
    connect(accounts, SIGNAL(resetCosts(const QString &)),
	    this, SLOT(resetCosts(const QString &)));
    connect(accounts, SIGNAL(resetVolume(const QString &)),
	    this, SLOT(resetVolume(const QString &)));

    modem1 = new ModemWidget( tabWindow->addPage( i18n("&Device"), i18n("Serial Device") ) );
    modem2 = new ModemWidget2( tabWindow->addPage( i18n("&Modem"), i18n("Modem Settings") ) );
    graph = new GraphSetup( tabWindow->addPage( i18n("&Graph"), i18n("Throughput Graph" ) ) );
    general = new GeneralWidget( tabWindow->addPage( i18n("M&isc"), i18n("Miscellaneous Settings") ) );
  }
}


void KPPPWidget::enterPressedInID() {
  PW_Edit->setFocus();
}


void KPPPWidget::enterPressedInPW() {
  connect_b->setFocus();
}

// triggered by the session manager
void KPPPWidget::saveMyself() {
    gpppdata.save();
}

void KPPPWidget::shutDown() {
    interruptConnection();
    saveMyself();
}

void KPPPWidget::log_window_toggled(bool on) {
  gpppdata.set_show_log_window(on);
}


void KPPPWidget::setup() {
  prepareSetupDialog();

  if(tabWindow->exec())
    gpppdata.save();
  else
    gpppdata.cancel();
}


void KPPPWidget::resetaccounts() {
  connectto_c->clear();

  int count = gpppdata.count();

  // enable/disable controls
  connectto_c->setEnabled(count > 0);
  connect_b->setEnabled(count > 0);
  log->setEnabled(count > 0);
  ID_Edit->setEnabled(count > 0);
  PW_Edit->setEnabled(count > 0);

  //load the accounts
  for(int i=0; i < count; i++) {
    gpppdata.setAccountbyIndex(i);
     connectto_c->insertItem(gpppdata.accname());
  }

  //set the default account
  if(!gpppdata.defaultAccount().isEmpty()) {
    for(int i=0; i < count; i++)
       if(gpppdata.defaultAccount() == connectto_c->text(i)) {
 	connectto_c->setCurrentItem(i);
	gpppdata.setAccountbyIndex(i);

	ID_Edit->setText(gpppdata.storedUsername());
	PW_Edit->setText(gpppdata.storedPassword());
    }
  }
  else
    if(count > 0) {
       gpppdata.setDefaultAccount(connectto_c->text(0));
        gpppdata.save();
	ID_Edit->setText(gpppdata.storedUsername());
	PW_Edit->setText(gpppdata.storedPassword());
    }

  connect(ID_Edit, SIGNAL(textChanged(const QString &)),
 	  this, SLOT(usernameChanged(const QString &)));

  connect(PW_Edit, SIGNAL(textChanged(const QString &)),
 	  this, SLOT(passwordChanged(const QString &)));

  if (ID_Edit->text().isEmpty())
      ID_Edit->setFocus();
  else if (PW_Edit->text().isEmpty())
      PW_Edit->setFocus();
}


void KPPPWidget::interruptConnection() {
  // interrupt dial up
  if (con->isVisible())
    emit con->cancelbutton();

  // disconnect if online
  if (gpppdata.pppdRunning())
    emit disconnect();
}


void KPPPWidget::sigPPPDDied() {
  kdDebug(5002) << "Received a SIGUSR1" << endl;

    // if we are not connected pppdpid is -1 so have have to check for that
    // in the followin line to make sure that we don't raise a false alarm
    // such as would be the case when the log file viewer exits.
    if(gpppdata.pppdRunning() || gpppdata.pppdError()) {
      kdDebug(5002) << "It was pppd that died" << endl;

      // when we killpppd() on Cancel in ConnectWidget
      // we set pppid to -1 so we won't
      // enter this block

      // just to be sure
      Requester::rq->removeSecret(AUTH_PAP);
      Requester::rq->removeSecret(AUTH_CHAP);

      gpppdata.setpppdRunning(false);

      kdDebug(5002) << "Executing command on disconnect since pppd has died." << endl;
      QApplication::flushX();
      execute_command(gpppdata.command_on_disconnect());

      stopAccounting();

      con_win->stopClock();
      DockWidget::dock_widget->stop_stats();
      DockWidget::dock_widget->hide();

      if(!gpppdata.pppdError())
        gpppdata.setpppdError(E_PPPD_DIED);
      removedns();
      Modem::modem->unlockdevice();
      con->pppdDied();

      if(!gpppdata.automatic_redial()) {
	quit_b->setFocus();
	show();
	con_win->stopClock();
	stopAccounting();
	con_win->hide();
	con->hide();

        gpppdata.setpppdRunning(false);
	// // not in a signal handler !!!  KNotifyClient::beep();
	QString msg;
	if (gpppdata.pppdError() == E_IF_TIMEOUT)
	    msg = i18n("Timeout expired while waiting for the PPP interface "
                       "to come up!");
	else {
	    msg = i18n("<p>The pppd daemon died unexpectedly!</p>");
	    Requester::rq->pppdExitStatus();
	    if (Requester::rq->lastStatus != 99) {	// more recent pppds only
		msg += i18n("<p>Exit status: %1").arg(Requester::rq->lastStatus);
		msg += i18n("</p><p>See 'man pppd' for an explanation of the error "
			    "codes or take a look at the kppp FAQ on "
			    "  <a href=http://devel-home.kde.org/~kppp/index.html>"
			    "http://devel-home.kde.org/~kppp/index.html</a></p>");
	    }
	}

	if(KMessageBox::warningYesNo(0, msg, i18n("Error"), i18n("&OK"), i18n("&Details...")) == KMessageBox::No)
	  PPPL_ShowLog();
      } else { /* reconnect on disconnect */
	kdDebug(5002) << "Trying to reconnect... " << endl;

        if(gpppdata.authMethod() == AUTH_PAP ||
	   gpppdata.authMethod() == AUTH_CHAP ||
	   gpppdata.authMethod() == AUTH_PAPCHAP)
          Requester::rq->setSecret(gpppdata.authMethod(),
				   encodeWord(gpppdata.storedUsername()),
				   encodeWord(gpppdata.password()));

	con_win->hide();
	con_win->stopClock();
	stopAccounting();
	gpppdata.setpppdRunning(false);
	// not in a signal handler !!!	KNotifyClient::beep();
	emit cmdl_start();
    }
  }
  gpppdata.setpppdError(0);
}


void KPPPWidget::sigChld() {
  kdDebug(5002) << "sigchld()" << endl;
  //  pid_t id = wait(0L);
  //  if(id == helperPid && helperPid != -1) {
  //    kdDebug(5002) << "It was the setuid child that died" << endl;
 // helperPid = -1;
    QString msg = i18n("kppp's helper process just died.\n"
                       "Since further execution would be pointless, "
                       "kppp will shut down now.");
    KMessageBox::error(0L, msg);
    remove_pidfile();
    exit(1);
    //  }
}


void KPPPWidget::newdefaultaccount(int i) {
  gpppdata.setDefaultAccount(connectto_c->text(i));
  gpppdata.save();
  ID_Edit->setText(gpppdata.storedUsername());
  PW_Edit->setText(gpppdata.storedPassword());
}


void KPPPWidget::expandbutton() {
  setup();
}


void KPPPWidget::beginConnect() {
  // make sure to connect to the account that is selected in the combo box
  // (exeption: an account given by a command line argument)
  if(!m_bCmdlAccount) {
    gpppdata.setAccount(connectto_c->currentText());
    gpppdata.setPassword(PW_Edit->text());
  } else {
    gpppdata.setPassword(gpppdata.storedPassword());
  }

  QFileInfo info(pppdPath());

  if(!info.exists()){
    KMessageBox::error(this, i18n("Cannot find the PPP daemon!\n"
                              "Make sure that pppd is installed and "
                              "that you have entered the correct path."));
    return;
  }
#if 0
  if(!info.isExecutable()){

    QString string;
    string = i18n("kppp cannot execute:\n %1\n"
    		   "Please make sure that you have given kppp "
		   "setuid permission and that "
		   "pppd is executable.").arg(gpppdata.pppdPath());
    KMessageBox::error(this, string);
    return;

  }
#endif

  QFileInfo info2(gpppdata.modemDevice());

  if(!info2.exists()){
    QString string;
    string = i18n("kppp can not find:\n %1\nPlease make sure you have setup "
		   "your modem device properly "
		   "and/or adjust the location of the modem device on "
		   "the modem tab of "
		   "the setup dialog.").arg(gpppdata.modemDevice());
    KMessageBox::error(this, string);
    return;
  }

  // if this is a PAP or CHAP account, ensure that username is
  // supplied
  if(gpppdata.authMethod() == AUTH_PAP ||
     gpppdata.authMethod() == AUTH_CHAP ||
     gpppdata.authMethod() == AUTH_PAPCHAP ) {
    if(ID_Edit->text().isEmpty()) {
      KMessageBox::error(this,
			   i18n(
                           "You have selected the authentication "
			   "method PAP or CHAP. This requires that you "
			   "supply a username and a password!"));
      return;
    } else {
      if(!Requester::rq->setSecret(gpppdata.authMethod(),
				   encodeWord(gpppdata.storedUsername()),
				   encodeWord(gpppdata.password()))) {
	QString s;
	s = i18n("Cannot create PAP/CHAP authentication\n"
				     "file \"%1\"").arg(PAP_AUTH_FILE);
	KMessageBox::error(this, s);
	return;
      }
    }
  }

  if (gpppdata.phonenumber().isEmpty()) {
    QString s = i18n("You must specify a telephone number!");
    KMessageBox::error(this, s);
    return;
  }

  this->hide();

  QString tit = i18n("Connecting to: %1").arg(gpppdata.accname());
  con->setCaption(tit);

  con->show();

  bool show_debug = gpppdata.get_show_log_window();
  con->debug->setOn(show_debug);	// toggle button
  debugwindow->clear();
  if (!show_debug)
    debugwindow->hide();
  else {
    debugwindow->show();
    con->raise();
  }

  emit begin_connect();
}


void KPPPWidget::disconnect() {
  if (!gpppdata.command_before_disconnect().isEmpty()) {
    con_win->hide();
    con->show();
    con->setCaption(i18n("Disconnecting..."));
    con->setMsg(i18n("Executing command before disconnection."));

    kapp->processEvents();
    QApplication::flushX();
    pid_t id = execute_command(gpppdata.command_before_disconnect());
    int i, status;

    do {
      kapp->processEvents();
      i = waitpid(id, &status, WNOHANG);
      usleep(500000);
    } while (i == 0 && errno == 0);

    con->hide();
  }

  kapp->processEvents();

  statdlg->stop_stats();
  Requester::rq->killPPPDaemon();

  QApplication::flushX();
  execute_command(gpppdata.command_on_disconnect());

  Requester::rq->removeSecret(AUTH_PAP);
  Requester::rq->removeSecret(AUTH_CHAP);

  removedns();
  Modem::modem->unlockdevice();

  con_win->stopClock();
  p_kppp->stopAccounting();
  con_win->hide();

  DockWidget::dock_widget->stop_stats();
  DockWidget::dock_widget->hide();

  if(m_bQuitOnDisconnect)
    kapp->exit(0);
  else {
    this->quit_b->setFocus();
    this->show();
  }
}


void KPPPWidget::helpbutton() {
  kapp->invokeHelp();
}


void KPPPWidget::quitbutton() {
  if(gpppdata.pppdRunning()) {
    int ok = KMessageBox::warningYesNo(this,
			    i18n("Exiting kPPP will close your PPP Session."),
			    i18n("Quit kPPP?"));
    if(ok == KMessageBox::Yes) {
      Requester::rq->killPPPDaemon();
      QApplication::flushX();
      execute_command(gpppdata.command_on_disconnect());
      removedns();
      Modem::modem->unlockdevice();
    }
  } else {
    if (!gpppdata.accname().isEmpty() && !gpppdata.storePassword())
      gpppdata.setStoredPassword("");
  }
  gpppdata.save();
  kapp->quit();
}


void KPPPWidget::rulesetLoadError() {
  KMessageBox::error(this, ruleset_load_errmsg);
}


void KPPPWidget::startAccounting() {
  // volume accounting
  stats->totalbytes = 0;

  kdDebug() << "AcctEnabled: " << gpppdata.AcctEnabled() << endl;

  // load the ruleset
  if(!gpppdata.AcctEnabled())
    return;

  QString d = AccountingBase::getAccountingFile(gpppdata.accountingFile());
  //  if(::access(d.data(), X_OK) != 0)
    acct = new Accounting(this, stats);
    //  else
    //    acct = new ExecutableAccounting(this);

  // connect to the accounting object
  connect(acct, SIGNAL(changed(QString, QString)),
	  con_win, SLOT(slotAccounting(QString, QString)));

  if(!acct->loadRuleSet(gpppdata.accountingFile())) {
    QString s= i18n("Can not load the accounting "
    		    "ruleset \"%1\"!").arg(gpppdata.accountingFile());

    // starting the messagebox with a timer will prevent us
    // from blocking the calling function ConnectWidget::timerEvent
    ruleset_load_errmsg = s;
    QTimer::singleShot(0, this, SLOT(rulesetLoadError()));
    return;
  } else
    acct->slotStart();
}

void KPPPWidget::stopAccounting() {
  // store volume accounting
  if(stats->totalbytes != 0)
    gpppdata.setTotalBytes(stats->totalbytes);

  if(!gpppdata.AcctEnabled())
    return;

  if(acct != 0) {
    acct->slotStop();
    delete acct;
    acct = 0;
  }
}


void KPPPWidget::showStats() {
  if(statdlg) {
    statdlg->toCurrentDesktop();
    statdlg->show();
    statdlg->raise();
  }
}


void KPPPWidget::usernameChanged(const QString &) {
  // store username for later use
  gpppdata.setStoredUsername(ID_Edit->text());
}


void KPPPWidget::passwordChanged(const QString &) {
    // store the password if so requested
  if(gpppdata.storePassword())
    gpppdata.setStoredPassword(PW_Edit->text());
  else
    gpppdata.setStoredPassword("");
}


void KPPPWidget::setPW_Edit(const QString &pw) {
  PW_Edit->setText(pw);
}


void KPPPWidget::resetCosts(const QString &s) {
  AccountingBase::resetCosts(s);
}


void KPPPWidget::resetVolume(const QString &s) {
  AccountingBase::resetVolume(s);
}

/**
 * pppd's getword() function knows about escape characters.
 * If we write the username and password to the secrets file
 * we'll therefore have to escape back slashes.
 */
QString KPPPWidget::encodeWord(const QString &s) {
    QString r = s;
    r.replace(QRegExp("\\"), "\\\\");
    return r;
}

void KPPPWidget::setQuitOnDisconnect (bool b)
{
    m_bQuitOnDisconnect = b;
}

void KPPPWidget::showNews() {
#ifdef KPPP_SHOW_NEWS
  /*
   * Introduce the QuickHelp feature to new users of this version
   */
  #define QUICKHELP_HINT "Hint_QuickHelp"
  if(gpppdata.readNumConfig(GENERAL_GRP, QUICKHELP_HINT, 0) == 0) {
    QDialog dlg(0, 0, true);
    dlg.setCaption(i18n("Recent Changes in KPPP"));

    QVBoxLayout *tl = new QVBoxLayout(&dlg, 10, 10);
    QHBoxLayout *l1 = new QHBoxLayout(10);
    QVBoxLayout *l2 = new QVBoxLayout(10);
    tl->addLayout(l1);

    QLabel *icon = new QLabel(&dlg);
    icon->setPixmap(BarIcon("exclamation"));
    icon->setFixedSize(icon->sizeHint());
    l1->addWidget(icon);
    l1->addLayout(l2);

    QLabel *l = new QLabel(i18n("From version 1.4.8 on, kppp has a new feature\n"
			      "called \"Quickhelp\". It's similar to a tooltip,\n"
			      "but you can activate it whenever you want.\n"
			      "\n"
			      "To activate it, simply click on a control like\n"
			      "a button or a label with the right mouse button.\n"
			      "If the item supports Quickhelp, a popup menu\n"
			      "will appear leading to Quickhelp.\n"
			      "\n"
			      "To test it, right-click somewhere in this text."),
			 &dlg);

    QCheckBox *cb = new QCheckBox(i18n("Don't show this hint again"), &dlg);
    cb->setFixedSize(cb->sizeHint());

    KButtonBox *bbox = new KButtonBox(&dlg);
    bbox->addStretch(1);
    QPushButton *ok = bbox->addButton(i18n("OK"));
    ok->setDefault(true);
    dlg.connect(ok, SIGNAL(clicked()),
		&dlg, SLOT(accept()));
    bbox->addStretch(1);
    bbox->layout();

    l2->addWidget(l);
    l2->addWidget(cb);
    tl->addWidget(bbox);

    QString tmp = i18n("This is an example of <b>QuickHelp</b>.\n"
		       "This window will stay open until you\n"
		       "click a mouse button or a press a key.\n");

    QWhatsThis::add(cb,tmp);
    QWhatsThis::add(l, tmp);

    dlg.exec();
    if(cb->isChecked()) {
      gpppdata.writeConfig(GENERAL_GRP, QUICKHELP_HINT, 1);
      gpppdata.save();
    }
  }
#endif
}


#include "kpppwidget.moc"

