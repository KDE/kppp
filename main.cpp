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

#include <config.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <locale.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#ifdef _XPG4_2
#define __xnet_connect	connect
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <qfileinfo.h>
#include <qregexp.h>

#include <kdebug.h>
#include <kbuttonbox.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstddirs.h>
#include <kglobal.h>
#include <kaboutdata.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <knotifyclient.h>
#include <kseparator.h>
#include <qdir.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>
#include <kwin.h>

#include "providerdb.h"

#include "acctselect.h"
#include "main.h"
#include "version.h"
#include "auth.h"
#include "docking.h"
#include "runtests.h"
#include "modem.h"
#include "ppplog.h"
#include "groupbox.h"
#include "opener.h"
#include "requester.h"
#include "modemdb.h"
#include "iplined.h"
#include "pppstats.h"
#include "pppdata.h"
#include "general.h"

#include <X11/Xlib.h>
#include <kapp.h>
#include <kcmdlineargs.h>

static const char *description =
	I18N_NOOP("A dialer and front-end to pppd.");

static KCmdLineOptions option[] =
{
   { "c <account_name>", I18N_NOOP("Connect using 'account_name'"), 0 },
   { "k", I18N_NOOP("Terminate an existing connection"), 0 },
   { "q", I18N_NOOP("Quit after end of connection"), 0 },
   { "r <rule_file>", I18N_NOOP("Check syntax of rule_file"), 0 },
   { "T", I18N_NOOP("Enable test-mode"), 0 },
   { 0, 0, 0}
};


KPPPWidget*	p_kppp;
QString 	cmdl_account;

bool	have_cmdl_account;
bool    quit_on_disconnect = false;

// for testing purposes
bool TESTING=0;

// initial effective user id before possible suid status is dropped
uid_t euid;
// helper process' pid
pid_t helperPid = -1;

QString local_ip_address;
QString remote_ip_address;
QString pidfile;

class SignalEvent : public QEvent {
public:
  SignalEvent(int s) : QEvent(User), sig(s) { }
  int sigType() const { return sig; }
private:
  int sig;
};

void showNews() {
#if 0
  /*
   * Introduce the QuickHelp feature to new users of this version
   */
  #define QUICKHELP_HINT "Hint_QuickHelp"
  if(gpppdata.readNumConfig(GENERAL_GRP, QUICKHELP_HINT, 0) == 0) {
    QDialog dlg(0, 0, true);
    dlg.setCaption(i18n("Recent changes in kppp"));

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

    QString tmp = i18n("This is an example for <b>QuickHelp</b>.\n"
		       "This window will stay open until you\n"
		       "click a mouse button or a hit a key.\n");

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

/**
 * pppd's getword() function knows about escape characters.
 * If we write the username and password to the secrets file
 * we'll therefore have to escape back slashes.
 */
QString encodeWord(const QString &s) {
    QString r = s;
    r.replace(QRegExp("\\"), "\\\\");
    return r;
}

extern "C" {
  static int kppp_x_errhandler( Display *dpy, XErrorEvent *err ) {
    char errstr[256]; // safe

    /*
      if(gpppdata.pppdpid() >= 0) {
      kill(gpppdata.pppdpid(), SIGTERM);
      }

      p_kppp->stopAccounting();
      removedns();
      unlockdevice();*/

    XGetErrorText( dpy, err->error_code, errstr, 256 );
    kdFatal() << "X Error: " << errstr << endl;
    kdFatal() << "Major opcode: " << err->request_code << endl;
    exit(256);
    return 0;
  }


  static int kppp_xio_errhandler( Display * ) {
    if(gpppdata.get_xserver_exit_disconnect()) {
      fprintf(stderr, "X11 Error!\n");
      if(gpppdata.pppdRunning())
        Requester::rq->killPPPDaemon();

      p_kppp->stopAccounting();
      removedns();
      Modem::modem->unlockdevice();
      return 0;
    } else{
      kdFatal() << "Fatal IO error: client killed" << endl;
      exit(256);
      return 0;
    }
  }
} /* extern "C" */


int main( int argc, char **argv ) {
  // make sure that open/fopen and so on NEVER return 1 or 2 (stdout and stderr)
  // Expl: if stdout/stderr were closed on program start (by parent), open()
  // would return a FD of 1, 2 (or even 0 if stdin was closed too)
  if(fcntl(0, F_GETFL) == -1)
    (void)open("/dev/null", O_RDONLY);

  if(fcntl(1, F_GETFL) == -1)
    (void)open("/dev/null", O_WRONLY);

  if(fcntl(2, F_GETFL) == -1)
    (void)open("/dev/null", O_WRONLY);

  // Don't insert anything above this line unless you really know what
  // you're doing. We're most likely running setuid root here,
  // until we drop this status a few lines below.
  int sockets[2];
  if(socketpair(AF_UNIX, SOCK_DGRAM, 0, sockets) != 0) {
    fprintf(stderr, "error creating socketpair !\n");
    return 1;
  }

  switch(helperPid = fork()) {
  case 0:
    // child process
    // make process leader of new group
    setsid();
    umask(0);
    close(sockets[0]);
    signal(SIGHUP, SIG_IGN);
    (void) new Opener(sockets[1]);
    // we should never get here
    _exit(1);

  case -1:
    perror("fork() failed");
    exit(1);
  }

  // parent process
  close(sockets[1]);

  // drop setuid status
  euid = geteuid();
  setgid(getgid());
  setuid(getuid());

  //
  // end of setuid-dropping block.
  //

  // install exit handler that will kill the helper process
  atexit(myShutDown);

  // not needed anymore, just causes problems with broken setup
  //  if(getHomeDir() != 0)
  //      setenv("HOME", getHomeDir(), 1);

  (void) new Requester(sockets[0]);

  KAboutData aboutData("kppp", I18N_NOOP("KPPP"),
    KPPPVERSION, description, KAboutData::License_GPL,
    I18N_NOOP("(c) 1999-2000, The KPPP Developers"));
  aboutData.addAuthor("Bernd Wuebben",0, "wuebben@kde.org");
  aboutData.addAuthor("Mario Weilguni",0, "");
  aboutData.addAuthor("Harri Porten",0, "porten@kde.org");

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( option );



  KApplication a;

  // set portable locale for decimal point
  setlocale(LC_NUMERIC ,"C");

  // open configuration file
  gpppdata.open();

  kdDebug(5002) << "helperPid: " << (int) helperPid << endl;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  cmdl_account = args->getOption("c");
  bool terminate_connection = args->isSet("k");
  quit_on_disconnect = args->isSet("q");
  if (args->isSet("r"))
    return RuleSet::checkRuleFile(args->getOption("r"));

  TESTING = args->isSet("T");

  if(!cmdl_account.isEmpty()) {
    have_cmdl_account = true;
    kdDebug(5002) << "cmdl_account: " << cmdl_account << endl;
  }

  // make sure that nobody can read the password from the
  // config file
  QString configFile = KGlobal::dirs()->saveLocation("config")
    + QString(kapp->name()) + "rc";
  if(access(QFile::encodeName(configFile), F_OK) == 0)
    chmod(QFile::encodeName(configFile), S_IRUSR | S_IWUSR);

  // do we really need to generate an empty directory structure here ?
  KGlobal::dirs()->saveLocation("appdata", "Rules");

  QString msg;
  int pid = create_pidfile();
  if(pid < 0) {
    msg = i18n("kppp can't create or read from\n%1.").arg(pidfile);
    KMessageBox::error(0L, msg);
    return 1;
  }

  if (terminate_connection) {
    setgid(getgid());
    setuid(getuid());
    if (pid > 0)
      kill(pid, SIGINT);
    else
      remove_pidfile();
    return 0;
  }

  // Mario: testing
  if(TESTING) {
    gpppdata.open();
    gpppdata.setAccountbyIndex(0);

    QString s = argv[2];
    urlEncode(s);
    kdDebug() << s << endl;

    remove_pidfile();
    return 0;
  }

  if (pid > 0) {
    msg = i18n("kppp has detected a %1 file.\n\n"
                     "Another instance of kppp seems to be "
                     "running under\nprocess-ID %2.\n\n"
                     "Make sure that you are not running another "
                     "kppp,\ndelete the pid file, and restart kppp.")
                .arg(pidfile).arg(pid);
    QMessageBox::warning(0L, i18n("Error"), msg, i18n("Exit"));
    return 1;
  }

  KPPPWidget kppp;
  p_kppp = &kppp;

  (void)new DockWidget(p_kppp->con_win, "dockw", p_kppp->stats);

#if 0
  // keep user informed about recent changes
  if(!have_cmdl_account)
    showNews();
#endif

  a.setMainWidget(&kppp);
  a.setTopWidget(&kppp);

  // we really don't want to die accidentally, since that would leave the
  // modem connected. If you really really want to kill me you must send
  // me a SIGKILL.
  signal(SIGINT, sighandler);
  signal(SIGCHLD, sighandler);
  signal(SIGUSR1, sighandler);
  signal(SIGTERM, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  //  XSetErrorHandler( kppp_x_errhandler );
  //  XSetIOErrorHandler( kppp_xio_errhandler );

  int ret = a.exec();

  remove_pidfile();

  return ret;
}



KPPPWidget::KPPPWidget( QWidget *parent, const char *name )
  : QWidget(parent, name),
    acct(0)
{
  tabWindow = 0;

  // before doing anything else, run a few tests

  int result = runTests();
  if(result == TEST_CRITICAL)
    exit(4);

  installEventFilter(this);

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);

  fline1 = new KSeparator( KSeparator::HLine, this);
  tl->addWidget(fline1);

  QGridLayout *l1 = new QGridLayout(3, 4);
  tl->addLayout(l1);
  l1->addColSpacing(0, 10);
  l1->addColSpacing(3, 10);
  l1->setColStretch(1, 3);
  l1->setColStretch(2, 4);

  label1 = new QLabel(i18n("Connect to: "), this);
  l1->addWidget(label1, 0, 1);

  connectto_c = new QComboBox(false, this);

  connect(connectto_c, SIGNAL(activated(int)),
	  SLOT(newdefaultaccount(int)));
  l1->addWidget(connectto_c, 0, 2);

  ID_Label = new QLabel(i18n("Login ID:"), this);
  l1->addWidget(ID_Label, 1, 1);

  // the entry line for usernames
  ID_Edit = new QLineEdit(this);
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

  PW_Label = new QLabel(i18n("Password:"), this);
  l1->addWidget(PW_Label, 2, 1);

  PW_Edit= new QLineEdit(this);
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
   log = new QCheckBox(i18n("Show Log Window"), this);
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
  quit_b = new QPushButton(i18n("&Quit"), this);
  connect( quit_b, SIGNAL(clicked()), SLOT(quitbutton()));
  if(quit_b->sizeHint().width() > minw)
    minw = quit_b->sizeHint().width();

  setup_b = new QPushButton(i18n("&Setup"), this);
  connect( setup_b, SIGNAL(clicked()), SLOT(expandbutton()));
  if(setup_b->sizeHint().width() > minw)
    minw = setup_b->sizeHint().width();

  if(gpppdata.access() != KConfig::ReadWrite)
    setup_b->setEnabled(false);

  help_b = new QPushButton(i18n("?"), this);
  connect( help_b, SIGNAL(clicked()), SLOT(helpbutton()));
  if(help_b->sizeHint().width() > minw)
    minw = help_b->sizeHint().width();

  connect_b = new QPushButton(i18n("&Connect"), this);
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

  con_win = new ConWindow(0, "conw", this, stats);
  KWin::setIcons(con_win->winId(), kapp->icon(), kapp->miniIcon());
/*
constructor of con_win reads position from config file

  con_win->setGeometry(QApplication::desktop()->width()/2-160,
		    QApplication::desktop()->height()/2-55,
		    320,110);
*/
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
  con->setGeometry(QApplication::desktop()->width()/2-175,
		    QApplication::desktop()->height()/2-55,
		    350,110);

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

  debugwindow->setGeometry(QApplication::desktop()->width()/2+190,
			   QApplication::desktop()->height()/2-55,
			   debugwindow->width(),debugwindow->height());

  move(QApplication::desktop()->width()/2-width()/2,
       QApplication::desktop()->height()/2-height()/2);

  if(have_cmdl_account){
    bool result;
    result = gpppdata.setAccount(cmdl_account);
    if (!result){
      QString string;
      string = i18n("No such Account:\n%1").arg(cmdl_account);
      KMessageBox::error(this, string);
      have_cmdl_account = false;
      this->show();
    } else
      emit cmdl_start();
  } else
    show();
}

KPPPWidget::~KPPPWidget()
{
  delete stats;
}

bool KPPPWidget::eventFilter(QObject *o, QEvent *e) {
  if(e->type() == QEvent::User) {
    switch(((SignalEvent*)e)->sigType()) {
    case SIGINT:
      sigInt();
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
      if(e->type() == KeyPress) {
        if(connect_b->hasFocus() && ((QKeyEvent *)e)->key() == Key_Return) {
          beginConnect();
          return true;
        }
      }
    }

    return false;
}

void KPPPWidget::prepareSetupDialog() {
  if(tabWindow == 0) {
    tabWindow = new QTabDialog( kapp->mainWidget(), 0, TRUE );
    KWin::setIcons(tabWindow->winId(), kapp->icon(), kapp->miniIcon());
    tabWindow->setCaption( i18n("kppp Configuration") );
    tabWindow->setOkButton(i18n("OK"));
    tabWindow->setCancelButton(i18n("Cancel"));
    //    tabWindow->setFixedSize( 365, 375 );

    accounts = new AccountWidget(tabWindow,"accounts");
    connect(accounts, SIGNAL(resetaccounts()),
	    this, SLOT(resetaccounts()));
    connect(accounts, SIGNAL(resetCosts(const QString &)),
	    this, SLOT(resetCosts(const QString &)));
    connect(accounts, SIGNAL(resetVolume(const QString &)),
	    this, SLOT(resetVolume(const QString &)));
    modem1 = new ModemWidget(tabWindow);
    modem2 = new ModemWidget2(tabWindow);
    general = new GeneralWidget(tabWindow);
    graph = new GraphSetup(tabWindow);
    about  = new AboutWidget(tabWindow);

    tabWindow->addTab( accounts, i18n("Accounts") );
    tabWindow->addTab( modem1, i18n("Device") );
    tabWindow->addTab( modem2, i18n("Modem") );
    tabWindow->addTab( graph, i18n("Graph") );
    tabWindow->addTab( general, i18n("Misc.") );
    tabWindow->addTab( about, i18n("About") );
  }
}


void KPPPWidget::enterPressedInID() {
  PW_Edit->setFocus();
}


void KPPPWidget::enterPressedInPW() {
  connect_b->setFocus();
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
}

void sighandler(int sig) {
  QEvent *e = 0L;
  if(sig == SIGCHLD) {
    pid_t id = wait(0L);
    if(id >= 0 && id == helperPid) // helper process died
      e = new SignalEvent(sig);
  } else if(sig == SIGINT || sig == SIGUSR1)
    e = new SignalEvent(sig);

  // let eventFilter() deal with this when we're back in the loop
  if (e)
    QApplication::postEvent(p_kppp, e);

  signal(sig, sighandler); // reinstall signal handler
}

void KPPPWidget::sigInt() {
  kdDebug(5002) << "Received a SIGINT" << endl;

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
	    msg = i18n("The pppd daemon died unexpectedly!");
	    Requester::rq->pppdExitStatus();
	    if (Requester::rq->lastStatus != 99) {	// more recent pppds only
		msg += i18n("\n\nExit status: %1").arg(Requester::rq->lastStatus);
		msg += i18n("\n\nSee 'man pppd' for an explanation of the error "
			    "codes\nor take a look at the kppp FAQ on\n\n"
			    "  http://devel-home.kde.org/~kppp/index.html");
	    }
	}

	if(QMessageBox::critical(0, i18n("Error"), msg, i18n("OK"), i18n("Details...")))
	  PPPL_ShowLog();
      } else { /* reconnect on disconnect */
	kdDebug(5002) << "Trying to reconnect ... " << endl;

        if(gpppdata.authMethod() == AUTH_PAP ||
	   gpppdata.authMethod() == AUTH_CHAP ||
	   gpppdata.authMethod() == AUTH_PAPCHAP)
          Requester::rq->setSecret(gpppdata.authMethod(),
				   encodeWord(gpppdata.storedUsername()),
				   encodeWord(gpppdata.password));

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
    helperPid = -1;
    QString msg = i18n("Sorry. kppp's helper process just died.\n\n"
                       "Since a further execution would be pointless, "
                       "kppp will shut down right now.");
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
  if(!have_cmdl_account)
    gpppdata.setAccount(connectto_c->currentText());

  QFileInfo info(pppdPath());

  if(!info.exists()){
    KMessageBox::error(this, i18n("Cannot find the PPP daemon!\n\n"
                              "Make sure that pppd is installed and\n"
                              "you have entered the correct path."));
    return;
  }
#if 0
  if(!info.isExecutable()){

    QString string;
    string = i18n("kppp cannot execute:\n %1\n"
    		   "Please make sure that you have given kppp "
		   "setuid permission and that\n"
		   "pppd is executable.").arg(gpppdata.pppdPath());
    KMessageBox::error(this, string);
    return;

  }
#endif

  QFileInfo info2(gpppdata.modemDevice());

  if(!info2.exists()){
    QString string;
    string = i18n("kppp can not find:\n %1\nPlease make sure you have setup\n"
		   "your modem device properly\n"
		   "and/or adjust\n the location of the modem device on "
		   "the modem tab of\n"
		   "the setup dialog.\n Thank You").arg(gpppdata.modemDevice());
    KMessageBox::error(this, string);
    return;
  }

  gpppdata.setPassword(PW_Edit->text());

  // if this is a PAP or CHAP account, ensure that username is
  // supplied
  if(gpppdata.authMethod() == AUTH_PAP ||
     gpppdata.authMethod() == AUTH_CHAP ||
     gpppdata.authMethod() == AUTH_PAPCHAP ) {
    if(ID_Edit->text().isEmpty()) {
      KMessageBox::error(this,
			   i18n(
                           "You have selected the authentication\n"
			   "method PAP or CHAP. This requires that you\n"
			   "supply a username and a password!"));
      return;
    } else {
      gpppdata.password = PW_Edit->text();
      if(!Requester::rq->setSecret(gpppdata.authMethod(),
				   encodeWord(gpppdata.storedUsername()),
				   encodeWord(gpppdata.password))) {
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
    con->setCaption(i18n("Disconnecting ..."));
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

  if(quit_on_disconnect)
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
    QString s= i18n("Can not load the accounting\n"
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


pid_t execute_command (const QString & cmd) {
  QCString command = QFile::encodeName(cmd);
  if (command.isEmpty() || command.length() > COMMAND_SIZE)
    return (pid_t) -1;

  pid_t id;

  kdDebug(5002) << "Executing command: " << command << endl;

  QApplication::flushX();
  if((id = fork()) == 0) {
    // don't bother dieppp()
    signal(SIGCHLD, SIG_IGN);

    // close file descriptors
    for (int fd = 3; fd < 20; fd++)
      close(fd);

    // drop privileges if running setuid root
    setgid(getgid());
    setuid(getuid());

    system(command);
    _exit(0);
  }

  return id;
}


// Create a file containing the current pid. Returns 0 on success,
// -1 on failure or the pid of an already running kppp process.
pid_t create_pidfile() {
  int fd = -1;
  char pidstr[40]; // safe

  pidfile = KGlobal::dirs()->saveLocation("appdata") + "kppp.pid";

  if(access(QFile::encodeName(pidfile), F_OK) == 0) {

    if((access(QFile::encodeName(pidfile), R_OK) < 0) ||
       (fd = open(QFile::encodeName(pidfile), O_RDONLY)) < 0)
      return -1;

    int sz = read(fd, &pidstr, 32);
    close (fd);
    if (sz <= 0)
      return -1;
    pidstr[sz] = '\0';

    kdDebug(5002) << "found kppp.pid containing: " << pidstr << endl;

    int oldpid;
    int match = sscanf(pidstr, "%d", &oldpid);

    // found a pid in pidfile ?
    if (match < 1 || oldpid <= 0)
      return -1;

    // check if process exists
    if (kill((pid_t)oldpid, 0) == 0 || errno != ESRCH)
      return oldpid;

    kdDebug(5002) << "pidfile is stale\n" << endl;
    remove_pidfile();
  }

  if((fd = open(QFile::encodeName(pidfile), O_WRONLY | O_CREAT | O_EXCL,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1)
    return -1;

  fchown(fd, getuid(), getgid());

  sprintf(pidstr, "%d\n", getpid());
  write(fd, pidstr, strlen(pidstr));
  close(fd);

  return 0;
}


bool remove_pidfile() {
  struct stat st;

  // only remove regular files with user write permissions
  if(stat(QFile::encodeName(pidfile), &st) == 0 )
    if(S_ISREG(st.st_mode) && (access(QFile::encodeName(pidfile), W_OK) == 0)) {
      unlink(QFile::encodeName(pidfile));
      return true;
    }

  fprintf(stderr, "error removing pidfile.\n");
  return false;
}


void myShutDown() {
  pid_t pid;
  // don't bother about SIGCHLDs anymore
  signal(SIGCHLD, SIG_IGN);
  //  fprintf(stderr, "myShutDown(%i)\n", status);
  pid = helperPid;
  if(pid > 0) {
    helperPid = -1;
    //    fprintf(stderr, "killing child process %i", pid);
    kill(pid, SIGKILL);
  }
}

#include "main.moc"
