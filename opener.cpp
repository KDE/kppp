
/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997,98 Bernd Johannes Wuebben,
 *		                      Mario Weilguni
 *              Copyright (C) 1998-2002 Harri Porten <porten@kde.org>
 *
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

/* A note to developers:
 *
 * Apart from the first dozen lines in main() the following code represents
 * the setuid root part of kppp. So please be careful !
 * o restrain from using X, Qt or KDE library calls
 * o check for possible buffer overflows
 * o handle requests from the parent process with care. They might be forged.
 * o be paranoid and think twice about everything you change.
 */
#include <kdefakes.h>
#include <config-kppp.h>

#if defined(__osf__) || defined(__SVR4)
#define _POSIX_PII_SOCKET
extern "C" int sethostname(char *name, int name_len);
#if !defined(__osf__)
extern "C" int _Psendmsg(int, void*, int);
extern "C" int _Precvmsg(int, void*, int);
#endif
#endif

#include "kpppconfig.h"

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/param.h>


#include <netinet/in.h>

#ifdef __FreeBSD__
#  include <sys/linker.h>  // for kldload
#endif

#ifndef HAVE_NET_IF_PPP_H
#  if defined(__DragonFly__)
#    include <net/ppp_layer/ppp_defs.h>
#    include <net/if.h>
#    include <net/ppp/if_ppp.h>
#  elif defined HAVE_LINUX_IF_PPP_H
#    include <linux/if_ppp.h>
#  endif
#else
#  include <net/ppp_defs.h>
#  include <net/if.h>
#  include <net/if_ppp.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "opener.h"
#include "devices.h"

#ifdef HAVE_RESOLV_H
#  include <arpa/nameser.h>
#  include <resolv.h>
#endif

#ifndef _PATH_RESCONF
#define _PATH_RESCONF "/etc/resolv.conf"
#endif

#ifdef _XPG4_2
extern "C" {
  ssize_t recvmsg(int, struct msghdr *, int);
  ssize_t sendmsg(int, const struct msghdr *, int);
}
#endif

#define MY_ASSERT(x)  if (!(x)) { \
        fprintf(stderr, "ASSERT: \"%s\" in %s (%d)\n",#x,__FILE__,__LINE__); \
        exit(1); }

#define MY_DEBUG
#ifndef MY_DEBUG
#define Debug(s) ((void)0);
#define Debug2(s, i) ((void)0);
#else
#define Debug(s) fprintf(stderr, (s "\n"));
#define Debug2(s, i) fprintf(stderr, (s), (i));
#endif

static void sighandler_child(int);
static pid_t pppdPid = -1;
static int pppdExitStatus = -1;
static int checkForInterface();

// processing will stop at first file that could be opened successfully
const char * const kppp_syslog[] = { "/var/log/syslog.ppp",
			      "/var/log/syslog",
			      "/var/log/messages",
			      0 };

Opener::Opener(int s) : socket(s), ttyfd(-1) {
  lockfile[0] = '\0';
  signal(SIGUSR1, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
  signal(SIGINT, SIG_IGN);
  signal(SIGCHLD, sighandler_child);
  mainLoop();
}

void Opener::mainLoop() {

  int len;
  int fd = -1;
  int flags, mode;
  const char *device, * const *logFile;
  union AllRequests request;
  struct ResponseHeader response;
  struct msghdr	msg;
  struct iovec iov;

  iov.iov_base = IOV_BASE_CAST &request;
  iov.iov_len = sizeof(request);

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  // loop forever
  while(1) {
    len = recvmsg(socket, &msg, 0);
    if(len < 0) {
      switch(errno) {
      case EINTR:
	Debug("Opener: interrupted system call, continuing");
	break;
      default:
	perror("Opener: error reading from socket");
	_exit(1);
      }
    } else {
      switch(request.header.type) {

      case OpenDevice:
	Debug("Opener: received OpenDevice");
	MY_ASSERT(len == sizeof(struct OpenModemRequest));
	close(ttyfd);
	device = deviceByIndex(request.modem.deviceNum);
	response.status = 0;
	if ((ttyfd = open(device, O_RDWR|O_NDELAY|O_NOCTTY)) == -1) {
	  Debug("error opening modem device !");
	  fd = open(DEVNULL, O_RDONLY);
	  response.status = -errno;
	  sendFD(fd, &response);
	  close(fd);
	} else
	  sendFD(ttyfd, &response);
	break;

      case OpenLock:
	Debug("Opener: received OpenLock\n");
	MY_ASSERT(len == sizeof(struct OpenLockRequest));
	flags = request.lock.flags;
	MY_ASSERT(flags == O_RDONLY || flags == (O_WRONLY|O_TRUNC|O_CREAT));
	if(flags == (O_WRONLY|O_TRUNC|O_CREAT))
	  mode = 0644;
	else
	  mode = 0;

	device = deviceByIndex(request.lock.deviceNum);
	MY_ASSERT(strlen(LOCK_DIR)+strlen(device) < MaxPathLen);
	strlcpy(lockfile, LOCK_DIR"/LCK..", MaxPathLen);
	strlcat(lockfile, strrchr(device, '/') + 1, MaxPathLen );
	response.status = 0;
	// TODO:
	//   struct stat st;
	//   if(stat(lockfile.data(), &st) == -1) {
	//     if(errno == EBADF)
	//       return -1;
	//   } else {
	//     // make sure that this is a regular file
	//     if(!S_ISREG(st.st_mode))
	//       return -1;
	//   }
	if ((fd = open(lockfile, flags, mode)) == -1) {
	  Debug("error opening lockfile!");
	  lockfile[0] = '\0';
	  fd = open(DEVNULL, O_RDONLY);
	  response.status = -errno;
	} else
	  fchown(fd, 0, 0);
        sendFD(fd, &response);
	close(fd);
	break;

      case RemoveLock:
	Debug("Opener: received RemoveLock");
	MY_ASSERT(len == sizeof(struct RemoveLockRequest));
	close(ttyfd);
	ttyfd = -1;
	response.status = unlink(lockfile);
	lockfile[0] = '\0';
	sendResponse(&response);
	break;

      case OpenResolv:
	Debug("Opener: received OpenResolv");
	MY_ASSERT(len == sizeof(struct OpenResolvRequest));
	flags = request.resolv.flags;
	response.status = 0;
	if ((fd = open(_PATH_RESCONF, flags)) == -1) {
	  Debug("error opening resolv.conf!");
	  fd = open(DEVNULL, O_RDONLY);
	  response.status = -errno;
	}
        sendFD(fd, &response);
	close(fd);
	break;

      case OpenSysLog:
	Debug("Opener: received OpenSysLog");
	MY_ASSERT(len == sizeof(struct OpenLogRequest));
	response.status = 0;
	logFile = &kppp_syslog[0];
	while (*logFile) {
	  if ((fd = open(*logFile, O_RDONLY)) >= 0)
	    break;
	  logFile++;
	}
	if (!*logFile) {
	  Debug("No success opening a syslog file !");
	  fd = open(DEVNULL, O_RDONLY);
	  response.status = -errno;
        }
        sendFD(fd, &response);
	close(fd);
	break;

      case SetSecret:
	Debug("Opener: received SetSecret");
	MY_ASSERT(len == sizeof(struct SetSecretRequest));
	response.status = !createAuthFile(request.secret.method,
					  request.secret.username,
					  request.secret.password);
	sendResponse(&response);
	break;

      case RemoveSecret:
	Debug("Opener: received RemoveSecret");
	MY_ASSERT(len == sizeof(struct RemoveSecretRequest));
	response.status = !removeAuthFile(request.remove.method);
	sendResponse(&response);
	break;

      case SetHostname:
	Debug("Opener: received SetHostname");
	MY_ASSERT(len == sizeof(struct SetHostnameRequest));
	response.status = 0;
	if(sethostname(request.host.name, strlen(request.host.name)))
	  response.status = -errno;
	sendResponse(&response);
	break;

      case ExecPPPDaemon:
	Debug("Opener: received ExecPPPDaemon");
	MY_ASSERT(len == sizeof(struct ExecDaemonRequest));
	response.status = execpppd(request.daemon.arguments);
	sendResponse(&response);
	break;

      case KillPPPDaemon:
	Debug("Opener: received KillPPPDaemon");
	MY_ASSERT(len == sizeof(struct KillDaemonRequest));
	response.status = killpppd();
	sendResponse(&response);
	break;

      case PPPDExitStatus:
	Debug("Opener: received PPPDExitStatus");
	MY_ASSERT(len == sizeof(struct PPPDExitStatusRequest));
	response.status = pppdExitStatus;
	sendResponse(&response);
	break;

      case Stop:
	Debug("Opener: received STOP command");
	_exit(0);
	break;

      default:
	Debug("Opener: unknown command type. Exiting ...");
	_exit(1);
      }
    } // else
  }
}


//
// Send an open fd over a UNIX socket pair
//
int Opener::sendFD(int fd, struct ResponseHeader *response) {

  struct { struct cmsghdr cmsg; int fd; } control;
  struct msghdr	msg;
  struct iovec iov;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  // Send data
  iov.iov_base = IOV_BASE_CAST response;
  iov.iov_len = sizeof(struct ResponseHeader);

  // Send a (duplicate of) the file descriptor
  control.cmsg.cmsg_len = sizeof(struct cmsghdr) + sizeof(int);
  control.cmsg.cmsg_level = SOL_SOCKET;
  control.cmsg.cmsg_type = MY_SCM_RIGHTS;

  msg.msg_control = (char *) &control;
  msg.msg_controllen = control.cmsg.cmsg_len;

#ifdef CMSG_DATA
  *((int *)CMSG_DATA(&control.cmsg)) = fd;
#else
  *((int *) &control.cmsg.cmsg_data) = fd;
#endif

  if (sendmsg(socket, &msg, 0) < 0) {
    perror("unable to send file descriptors");
    return -1;
  }

  return 0;
}

int Opener::sendResponse(struct ResponseHeader *response) {

  struct msghdr	msg;
  struct iovec iov;

  msg.msg_name = 0L;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = 0L;
  msg.msg_controllen = 0;

  // Send data
  iov.iov_base = IOV_BASE_CAST response;
  iov.iov_len = sizeof(struct ResponseHeader);

  if (sendmsg(socket, &msg, 0) < 0) {
    perror("unable to send response");
    return -1;
  }

  return 0;
}

const char* Opener::deviceByIndex(int idx) {

  const char *device = 0L;

  for(int i = 0; devices[i]; i++)
    if(i == idx)
      device = devices[i];
  MY_ASSERT(device);
  return device;
}

bool Opener::createAuthFile(Auth method, char *username, char *password) {
  const char *authfile, *oldName, *newName;
  char line[100];
  char regexp[2*MaxStrLen+30];
  regex_t preg;

  if(!(authfile = authFile(method)))
    return false;

  if(!(newName = authFile(method, New)))
    return false;

  // look for username, "username" or 'username'
  // if you modify this RE you have to adapt regexp's size above
  snprintf(regexp, sizeof(regexp), "^[ \t]*%s[ \t]\\|^[ \t]*[\"\']%s[\"\']",
          username,username);
  MY_ASSERT(regcomp(&preg, regexp, 0) == 0);

  // copy to new file pap- or chap-secrets
  int old_umask = umask(0077);
  FILE *fout = fopen(newName, "w");
  if(fout) {
    // copy old file
    FILE *fin = fopen(authfile, "r");
    if(fin) {
      while(fgets(line, sizeof(line), fin)) {
        if(regexec(&preg, line, 0, 0L, 0) == 0)
           continue;
        fputs(line, fout);
      }
      fclose(fin);
    }

    // append user/pass pair
    fprintf(fout, "\"%s\"\t*\t\"%s\"\n", username, password);
    fclose(fout);
  }

  // restore umask
  umask(old_umask);

  // free memory allocated by regcomp
  regfree(&preg);

  if(!(oldName = authFile(method, Old)))
    return false;

  // delete old file if any
  unlink(oldName);

  rename(authfile, oldName);
  rename(newName, authfile);

  return true;
}


bool Opener::removeAuthFile(Auth method) {
  const char *authfile, *oldName;

  if(!(authfile = authFile(method)))
    return false;
  if(!(oldName = authFile(method, Old)))
    return false;

  if(access(oldName, F_OK) == 0) {
    unlink(authfile);
    return (rename(oldName, authfile) == 0);
  } else
    return false;
}


const char* Opener::authFile(Auth method, int version) {
  switch(method|version) {
  case PAP|Original:
    return PAP_AUTH_FILE;
    break;
  case PAP|New:
    return PAP_AUTH_FILE".new";
    break;
  case PAP|Old:
    return PAP_AUTH_FILE".old";
    break;
  case CHAP|Original:
    return CHAP_AUTH_FILE;
    break;
  case CHAP|New:
    return CHAP_AUTH_FILE".new";
    break;
  case CHAP|Old:
    return CHAP_AUTH_FILE".old";
    break;
  default:
    return 0L;
  }
}


bool Opener::execpppd(const char *arguments) {
  char buf[MAX_CMDLEN];
  char *args[MaxArgs];
  pid_t pgrpid;

  if(ttyfd<0)
    return false;

  pppdExitStatus = -1;

  switch(pppdPid = fork())
    {
    case -1:
      fprintf(stderr,"In parent: fork() failed\n");
      return false;
      break;

    case 0:
      // let's parse the arguments the user supplied into UNIX suitable form
      // that is a list of pointers each pointing to exactly one word
      strlcpy(buf, arguments, sizeof(buf));
      parseargs(buf, args);
      // become a session leader and let /dev/ttySx
      // be the controlling terminal.
      pgrpid = setsid();
#ifdef TIOCSCTTY
      if(ioctl(ttyfd, TIOCSCTTY, 0)<0)
        fprintf(stderr, "ioctl() failed.\n");
#elif defined (TIOCSPGRP)
       if(ioctl(ttyfd, TIOCSPGRP, &pgrpid)<0)
       fprintf(stderr, "ioctl() failed.\n");
#endif
      if(tcsetpgrp(ttyfd, pgrpid)<0)
        fprintf(stderr, "tcsetpgrp() failed.\n");

      dup2(ttyfd, 0);
      dup2(ttyfd, 1);

      switch (checkForInterface()) {
        case 1:
          fprintf(stderr, "Cannot determine if kernel supports ppp.\n");
          break;
        case -1:
          fprintf(stderr, "Kernel does not support ppp, oops.\n");
          break;
        case 0:
          fprintf(stderr, "Kernel supports ppp alright.\n");
          break;
      }

      execve(pppdPath(), args, 0L);
      _exit(0);
      break;

    default:
      Debug2("In parent: pppd pid %d\n",pppdPid);
      close(ttyfd);
      ttyfd = -1;
      return true;
      break;
    }
}


bool Opener::killpppd()const {
  if(pppdPid > 0) {
    Debug2("In killpppd(): Sending SIGTERM to %d\n", pppdPid);
    if(kill(pppdPid, SIGTERM) < 0) {
      Debug2("Error terminating %d. Sending SIGKILL\n", pppdPid);
      if(kill(pppdPid, SIGKILL) < 0) {
        Debug2("Error killing %d\n", pppdPid);
        return false;
      }
    }
  }
  return true;
}


void Opener::parseargs(char* buf, char** args) {
  int nargs = 0;
  int quotes;

  while(nargs < MaxArgs-1 && *buf != '\0') {

    quotes = 0;

    // Strip whitespace. Use nulls, so that the previous argument is
    // terminated automatically.

    while ((*buf == ' ' ) || (*buf == '\t' ) || (*buf == '\n' ) )
      *buf++ = '\0';

    // detect begin of quoted argument
    if (*buf == '"' || *buf == '\'') {
      quotes = *buf;
      *buf++ = '\0';
    }

    // save the argument
    if(*buf != '\0') {
      *args++ = buf;
      nargs++;
    }

    if (!quotes)
      while ((*buf != '\0') && (*buf != '\n') &&
	     (*buf != '\t') && (*buf != ' '))
	buf++;
    else {
      while ((*buf != '\0') && (*buf != quotes))
	buf++;
      *buf++ = '\0';
    }
  }

  *args = 0L;
}


const char* pppdPath() {
  // wasting a few bytes
  static char buffer[sizeof(PPPDSEARCHPATH)+sizeof(PPPDNAME)];
  static char *pppdPath = 0L;
  char *p;

  if(pppdPath == 0L) {
    const char *c = PPPDSEARCHPATH;
    while(*c != '\0') {
      while(*c == ':')
        c++;
      p = buffer;
      while(*c != '\0' && *c != ':')
        *p++ = *c++;
      *p = '\0';
      strcat(p, "/");
      strcat(p, PPPDNAME);
      if(access(buffer, F_OK) == 0)
        return (pppdPath = buffer);
    }
  }

  return pppdPath;
}

int checkForInterface()
{
// I don't know if Linux needs more initialization to get the ioctl to
// work, pppd seems to hint it does.  But BSD doesn't, and the following
// code should compile.
#if (defined(HAVE_NET_IF_PPP_H) || defined(HAVE_LINUX_IF_PPP_H)) && !defined(__SVR4)
    int s, ok;
    struct ifreq ifr;
    //    extern char *no_ppp_msg;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return 1;               /* can't tell */

    strlcpy(ifr.ifr_name, "ppp0", sizeof (ifr.ifr_name));
    ok = ioctl(s, SIOCGIFFLAGS, (caddr_t) &ifr) >= 0;
    close(s);

    if (ok == -1) {
// This is ifdef'd FreeBSD, because FreeBSD is the only BSD that supports
// KLDs, the old LKM interface couldn't handle loading devices
// dynamically, and thus can't load ppp support on the fly
#ifdef __FreeBSD__
        // If we failed to load ppp support and don't have it already.
        if (kldload("if_ppp") == -1) {
            return -1;
        }
        return 0;
#else
        return -1;
#endif
    }
    return 0;
#else
// We attempt to use the SunOS/SysVr4 method and stat /dev/ppp
   struct stat buf;

   memset(&buf, 0, sizeof(buf));
   return stat("/dev/ppp", &buf);
#endif
}


void sighandler_child(int) {
  pid_t pid;
  int status;

  signal(SIGCHLD, sighandler_child);
  if(pppdPid>0) {
    pid = waitpid(pppdPid, &status, WNOHANG);
    if(pid != pppdPid) {
      fprintf(stderr, "received SIGCHLD from unknown origin.\n");
    } else {
      Debug("It was pppd that died");
      pppdPid = -1;
      if((WIFEXITED(status))) {
	pppdExitStatus = (WEXITSTATUS(status));
        Debug2("pppd exited with return value %d\n", pppdExitStatus);
      } else {
	pppdExitStatus = 99;
        Debug("pppd exited abnormally.");
      }
      Debug2("Sending %i a SIGUSR1\n", getppid());
      kill(getppid(), SIGUSR1);
    }
  } else
    fprintf(stderr, "received unexpected SIGCHLD.\n");
}
