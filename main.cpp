/*
 *            kPPP: A pppd front end for the KDE project
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *            Copyright (C) 1998-2002 Harri Porten <porten@kde.org>
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

#include <config.h>

#include "main.h"

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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include "kpppwidget.h"
#include "opener.h"
#include "pppdata.h"
#include "providerdb.h"
#include "version.h"
#include "requester.h"

#include <X11/Xlib.h>

static const char description[] =
	I18N_NOOP("A dialer and front-end to pppd");

static const KCmdLineOptions options[] =
{
   { "c <account_name>", I18N_NOOP("Connect using 'account_name'"), 0 },
   { "m <modem_name>", I18N_NOOP("Connect using 'modem_name'"), 0 },
   { "k", I18N_NOOP("Terminate an existing connection"), 0 },
   { "q", I18N_NOOP("Quit after end of connection"), 0 },
   { "r <rule_file>", I18N_NOOP("Check syntax of rule_file"), 0 },
   { "T", I18N_NOOP("Enable test-mode"), 0 },
   { "dev <device_name>", I18N_NOOP("Use the specified device"), 0 },
   KCmdLineLastOption
};


KPPPWidget*	p_kppp;

// for testing purposes
bool TESTING=0;

// initial effective user id before possible suid status is dropped
uid_t euid;
// helper process' pid
pid_t helperPid = -1;

QString local_ip_address;
QString remote_ip_address;
QString pidfile;

#if 0
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
#endif

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
  if (setgid(getgid()) < 0 && errno != EPERM) {
    perror("setgid() failed");
    exit(1);
  }
  setuid(getuid());
  if (geteuid() != getuid()) {
    perror("setuid() failed");
    exit(1);
  }

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
    I18N_NOOP("(c) 1999-2002, The KPPP Developers"));
  aboutData.addAuthor("Harri Porten", I18N_NOOP("Current maintainer"), "porten@kde.org");
  aboutData.addAuthor("Bernd Wuebben", I18N_NOOP("Original author"), "wuebben@kde.org");
  aboutData.addAuthor("Mario Weilguni",0, "");

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );



  KApplication a;

  // set portable locale for decimal point
  setlocale(LC_NUMERIC ,"C");

  // open configuration file
  gpppdata.open();

  kdDebug(5002) << "helperPid: " << (int) helperPid << endl;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  bool terminate_connection = args->isSet("k");
  if (args->isSet("r"))
    return RuleSet::checkRuleFile(args->getOption("r"));

  TESTING = args->isSet("T");

  // make sure that nobody can read the password from the
  // config file
  QString configFile = KGlobal::dirs()->saveLocation("config")
    + QString(kapp->name()) + "rc";
  if(access(QFile::encodeName(configFile), F_OK) == 0)
    chmod(QFile::encodeName(configFile), S_IRUSR | S_IWUSR);

  // do we really need to generate an empty directory structure here ?
  KGlobal::dirs()->saveLocation("appdata", "Rules");

  int pid = create_pidfile();
  QString err_msg = i18n("kppp can't create or read from\n%1.").arg(pidfile);

  if(pid < 0) {
    KMessageBox::error(0L, err_msg);
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
    gpppdata.setAccountByIndex(0);

    QString s = argv[2];
    urlEncode(s);
    kdDebug(5002) << s << endl;

    remove_pidfile();
    return 0;
  }

  if (pid > 0) {
    QString msg = i18n("kppp has detected a %1 file.\n"
                       "Another instance of kppp seems to be "
                       "running under process-ID %2.\n"
                       "Please click Exit, make sure that you are "
                       "not running another kppp, delete the pid "
                       "file, and restart kppp.\n"
                       "Alternatively, if you have determined that "
                       "there is no other kppp running, please "
                       "click Continue to begin.")
                  .arg(pidfile).arg(pid);
    int button = KMessageBox::warningYesNo(0, msg, i18n("Error"),
                                      i18n("Exit"), KStdGuiItem::cont());
    if (button == KMessageBox::Yes)            /* exit */
       return 1;

    remove_pidfile();
    pid = create_pidfile();
    if(pid) {
      KMessageBox::error(0L, err_msg);
      return 1;
    }
  }

  KPPPWidget kppp;
  p_kppp = &kppp;

  (void)new DockWidget(p_kppp->con_win, "dockw", p_kppp->stats);

  a.setMainWidget(&kppp);
  a.setTopWidget(&kppp);

  // we really don't want to die accidentally, since that would leave the
  // modem connected. If you really really want to kill me you must send
  // me a SIGKILL.
  signal(SIGINT, sighandler);
  signal(SIGCHLD, sighandler);
  signal(SIGUSR1, sighandler);
  signal(SIGTERM, SIG_IGN);

  //  XSetErrorHandler( kppp_x_errhandler );
  //  XSetIOErrorHandler( kppp_xio_errhandler );

  int ret = a.exec();

  remove_pidfile();

  return ret;
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
    const int open_max = sysconf( _SC_OPEN_MAX );
    for (int fd = 3; fd < open_max; ++fd)
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
    if (sz < 0)
      return -1;
    pidstr[sz] = '\0';

    kdDebug(5002) << "found kppp.pid containing: " << pidstr << endl;

    // non-empty file ?
    if (sz > 0) {
      int oldpid;
      int match = sscanf(pidstr, "%d", &oldpid);

      // found a pid in pidfile ?
      if (match < 1 || oldpid <= 0)
        return -1;

      // check if process exists
      if (kill((pid_t)oldpid, 0) == 0 || errno != ESRCH)
        return oldpid;
    }

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

