/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 *
 * This file was added by Harri Porten <porten@tu-harburg.de>
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <setjmp.h>
#include <qregexp.h>
#include <qtimer.h>
#include <assert.h>

#include "modem.h"
#include "pppdata.h"
#include "requester.h"
#include <klocale.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <config.h>

static sigjmp_buf jmp_buffer;

Modem *Modem::modem = 0;

Modem::Modem() :
  modemfd(-1),
  sn(0L),
  data_mode(false),
  modem_is_locked(false)
{
  assert(modem==0);
  modem = this;
}


Modem::~Modem() {
  modem = 0;
}


speed_t Modem::modemspeed() {
  // convert the string modem speed int the gpppdata object to a t_speed type
  // to set the modem.  The constants here should all be ifdef'd because
  // other systems may not have them
  int i = gpppdata.speed().toInt()/100;

  switch(i) {
  case 24:
    return B2400;
    break;
  case 96:
    return B9600;
    break;
  case 192:
    return B19200;
    break;
  case 384:
    return B38400;
    break;
#ifdef B57600
  case 576:
    return B57600;
    break;
#endif

#ifdef B115200
  case 1152:
    return B115200;
    break;
#endif

#ifdef B230400
  case 2304:
    return B230400;
    break;
#endif

#ifdef B460800
  case 4608:
    return B460800;
    break;
#endif

  default:
    return B38400;
    break;
  }
}


bool Modem::opentty() {
  //  int flags;

  if((modemfd = Requester::rq->openModem(gpppdata.modemDevice()))<0) {
    errmsg = i18n("Sorry, can't open modem.");
    return false;
  }

#if 0
  if(gpppdata.UseCDLine()) {
    if(ioctl(modemfd, TIOCMGET, &flags) == -1) {
      errmsg = i18n("Sorry, can't detect state of CD line.");
      ::close(modemfd);
      modemfd = -1;
      return false;
    }
    if ((flags&TIOCM_CD) == 0) {
      errmsg = i18n("Sorry, the modem is not ready.");
      ::close(modemfd);
      modemfd = -1;
      return false;
    }
  }
#endif

  tcdrain (modemfd);
  tcflush (modemfd, TCIOFLUSH);

  if(tcgetattr(modemfd, &tty) < 0){
    // this helps in some cases
    tcsendbreak(modemfd, 0);
    sleep(1);
    if(tcgetattr(modemfd, &tty) < 0){
      errmsg = i18n("Sorry, the modem is busy.");
      ::close(modemfd);
      modemfd = -1;
      return false;
    }
  }

  memset(&initial_tty,'\0',sizeof(initial_tty));

  initial_tty = tty;

  tty.c_cc[VMIN] = 0; // nonblocking
  tty.c_cc[VTIME] = 0;
  tty.c_oflag = 0;
  tty.c_lflag = 0;

  tty.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
  tty.c_cflag |= CS8 | CREAD;
  tty.c_cflag |= CLOCAL;                   // ignore modem status lines
  tty.c_iflag = IGNBRK | IGNPAR /* | ISTRIP */ ;
  tty.c_lflag &= ~ICANON;                  // non-canonical mode
  tty.c_lflag &= ~(ECHO|ECHOE|ECHOK|ECHOKE);


  if(gpppdata.flowcontrol() != "None") {
    if(gpppdata.flowcontrol() == "CRTSCTS") {
      tty.c_cflag |= CRTSCTS;
    }
    else {
      tty.c_iflag |= IXON | IXOFF;
      tty.c_cc[VSTOP]  = 0x13; /* DC3 = XOFF = ^S */
      tty.c_cc[VSTART] = 0x11; /* DC1 = XON  = ^Q */
    }
  }
  else {
    tty.c_cflag &= ~CRTSCTS;
    tty.c_iflag &= ~(IXON | IXOFF);
  }

  cfsetospeed(&tty, modemspeed());
  cfsetispeed(&tty, modemspeed());

  tcdrain(modemfd);

  if(tcsetattr(modemfd, TCSANOW, &tty) < 0){
    errmsg = i18n("Sorry, the modem is busy.");
    ::close(modemfd);
    modemfd=-1;
    return false;
  }

  errmsg = i18n("Modem Ready.");
  return true;
}


bool Modem::closetty() {
  if(modemfd >=0 ) {
    stop();
    /* discard data not read or transmitted */
    tcflush(modemfd, TCIOFLUSH);

    if(tcsetattr(modemfd, TCSANOW, &initial_tty) < 0){
      errmsg = i18n("Can't restore tty settings: tcsetattr()\n");
      ::close(modemfd);
      modemfd = -1;
      return false;
    }
    ::close(modemfd);
    modemfd = -1;
  }

  return true;
}


void Modem::readtty(int) {
  char buffer[200];
  unsigned char c;
  int len;

  // read data in chunks of up to 200 bytes
  if((len = ::read(modemfd, buffer, 200)) > 0) {
    // split buffer into single characters for further processing
    for(int i = 0; i < len; i++) {
      c = buffer[i] & 0x7F;
      emit charWaiting(c);
    }
  }
}


void Modem::notify(const QObject *receiver, const char *member) {
  connect(this, SIGNAL(charWaiting(unsigned char)), receiver, member);
  startNotifier();
}


void Modem::stop() {
  disconnect(SIGNAL(charWaiting(unsigned char)));
  stopNotifier();
}


void Modem::startNotifier() {
  if(modemfd >= 0) {
    if(sn == 0) {
      sn = new QSocketNotifier(modemfd, QSocketNotifier::Read, this);
      connect(sn, SIGNAL(activated(int)), SLOT(readtty(int)));
      kdDebug(5002) << "QSocketNotifier started!" << endl;
    } else {
      //    Debug("QSocketNotifier re-enabled!");
      sn->setEnabled(true);
    }
  }
}


void Modem::stopNotifier() {
  if(sn != 0) {
    sn->setEnabled(false);
    disconnect(sn);
    delete sn;
    sn = 0;
    kdDebug(5002) << "QSocketNotifier stopped!" << endl;
  }
}


void Modem::flush() {
  char c;
  while(read(modemfd, &c, 1) == 1);
}


bool Modem::writeChar(unsigned char c) {
  int s;
  do {
    s = write(modemfd, &c, 1);
    if (s < 0) {
      kdError(5002) << "write() in Modem::writeChar failed" << endl;
      return false;
    }
  } while(s == 0);

  return true;
}


bool Modem::writeLine(const char *buf) {
  int len = strlen(buf);
  char *b = new char[len+2];
  memcpy(b, buf, len);
  // different modems seem to need different line terminations
  QString term = gpppdata.enter();
  if(term == "LF")
    b[len++]='\n';
  else if(term == "CR")
    b[len++]='\r';
  else if(term == "CR/LF") {
    b[len++]='\r';
    b[len++]='\n';
  }
  int l = len;
  while(l) {
    int wr = write(modemfd, &b[len-l], l);
    if(wr < 0) {
      // TODO do something meaningful with the error code (or ignore it
      kdError(5002) << "write() in Modem::writeLine failed" << endl;
      return false;
    }
    l -= wr;
  }
  return true;
}


bool Modem::hangup() {
  // this should really get the modem to hang up and go into command mode
  // If anyone sees a fault in the following please let me know, since
  // this is probably the most imporant snippet of code in the whole of
  // kppp. If people complain about kppp being stuck, this piece of code
  // is most likely the reason.
  struct termios temptty;
  int  modemstat;

  if(modemfd >= 0) {

    // is this Escape & HangupStr stuff really necessary ? (Harri)

    if (data_mode) escape_to_command_mode();

    // Then hangup command
    writeLine(gpppdata.modemHangupStr().local8Bit());

    usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 sec

#ifndef DEBUG_WO_DIALING
    if (sigsetjmp(jmp_buffer, 1) == 0) {
      // set alarm in case tcsendbreak() hangs
      signal(SIGALRM, alarm_handler);
      alarm(2);

      tcsendbreak(modemfd, 0);

      alarm(0);
      signal(SIGALRM, SIG_IGN);
    } else {
      // we reach this point if the alarm handler got called
      closetty();
      close(modemfd);
      modemfd = -1;
      errmsg = i18n("Sorry, the modem doesn't respond.");
      return false;
    }

#ifndef __svr4__ // drops DTR but doesn't set it afterwards again. not good for init.
    tcgetattr(modemfd, &temptty);
    cfsetospeed(&temptty, B0);
    cfsetispeed(&temptty, B0);
    tcsetattr(modemfd, TCSAFLUSH, &temptty);
#else
    ioctl(modemfd, TIOCMGET, &modemstat);
    modemstat &= ~TIOCM_DTR;
    ioctl(modemfd, TIOCMSET, &modemstat);
    ioctl(modemfd, TIOCMGET, &modemstat);
    modemstat |= TIOCM_DTR;
    ioctl(modemfd, TIOCMSET, &modemstat);
#endif

    usleep(gpppdata.modemInitDelay() * 10000); // 0.01 - 3.0 secs

    cfsetospeed(&temptty, modemspeed());
    cfsetispeed(&temptty, modemspeed());
    tcsetattr(modemfd, TCSAFLUSH, &temptty);
#endif
    return true;
  } else
    return false;
}


void Modem::escape_to_command_mode() {
  // Send Properly bracketed escape code to put the modem back into command state.
  // A modem will accept AT commands only when it is in command state.
  // When a modem sends the host the CONNECT string, that signals
  // that the modem is now in the connect state (no long accepts AT commands.)
  // Need to send properly timed escape sequence to put modem in command state.
  // Escape codes and guard times are controlled by S2 and S12 values.
  //
  tcflush(modemfd, TCIOFLUSH);

  // +3 because quiet time must be greater than guard time.
  usleep((gpppdata.modemEscapeGuardTime()+3)*20000);
  QCString tmp = gpppdata.modemEscapeStr().local8Bit();
  write(modemfd, tmp.data(), tmp.length());
  tcflush(modemfd, TCIOFLUSH);
  usleep((gpppdata.modemEscapeGuardTime()+3)*20000);

  data_mode = false;
}


const QString Modem::modemMessage() {
  return errmsg;
}


QString Modem::parseModemSpeed(const QString &s) {
  // this is a small (and bad) parser for modem speeds
  int rx = -1;
  int tx = -1;
  int i;
  QString result;

  kdDebug(5002) << "Modem reported result string: " << s << endl;

  const int RXMAX = 7;
  const int TXMAX = 2;
  QRegExp rrx[RXMAX] = {
    QRegExp("[0-9]+[:/ ]RX", false),
    QRegExp("[0-9]+RX", false),
    QRegExp("[/: -][0-9]+[/: ]", false),
    QRegExp("[/: -][0-9]+$", false),
    QRegExp("CARRIER [^0-9]*[0-9]+", false),
    QRegExp("CONNECT [^0-9]*[0-9]+", false),
    QRegExp("[0-9]+") // panic mode
  };

  QRegExp trx[TXMAX] = {
    QRegExp("[0-9]+[:/ ]TX", false),
    QRegExp("[0-9]+TX", false)
  };

  for(i = 0; i < RXMAX; i++) {
    int len, idx, result;
    if((idx = rrx[i].search(s)) > -1) {
      len = rrx[i].matchedLength();

      //
      // rrx[i] has been matched, idx contains the start of the match
      // and len contains how long the match is. Extract the match.
      //
      QString sub = s.mid(idx, len);

      //
      // Now extract the digits only from the match, which will
      // then be converted to an int.
      //
      if ((idx = rrx[RXMAX-1].search( sub )) > -1) {
        len = rrx[RXMAX-1].matchedLength();
        sub = sub.mid(idx, len);
        result = sub.toInt();
        if(result > 0) {
          rx = result;
          break;
        }
      }
    }
  }

  for(i = 0; i < TXMAX; i++) {
    int len, idx, result;
    if((idx = trx[i].search(s)) > -1) {
      len = trx[i].matchedLength();

      //
      // trx[i] has been matched, idx contains the start of the match
      // and len contains how long the match is. Extract the match.
      //
      QString sub = s.mid(idx, len);

      //
      // Now extract the digits only from the match, which will then
      // be converted to an int.
      //
      if((idx = rrx[RXMAX-1].search(sub)) > -1) {
        len = rrx[RXMAX-1].matchedLength();
        sub = sub.mid(idx, len);
        result = sub.toInt();
        if(result > 0) {
          tx = result;
          break;
        }
      }
    }
  }

  if(rx == -1 && tx == -1)
    result = i18n("Unknown speed");
  else if(tx == -1)
    result.setNum(rx);
  else if(rx == -1) // should not happen
    result.setNum(tx);
  else
    result.sprintf("%d/%d", rx, tx);

  kdDebug(5002) << "The parsed result is: " << result << endl;

  return result;
}


// Lock modem device. Returns 0 on success 1 if the modem is locked and -1 if
// a lock file can't be created ( permission problem )
int Modem::lockdevice() {
  int fd;
  char newlock[80]=""; // safe

  if(!gpppdata.modemLockFile()) {
    kdDebug(5002) << "The user doesn't want a lockfile." << endl;
    return 0;
  }

  if (modem_is_locked)
    return 1;

  QString lockfile = LOCK_DIR"/LCK..";
  lockfile += gpppdata.modemDevice().mid(5); // append everything after /dev/

  if(access(QFile::encodeName(lockfile), F_OK) == 0) {
    if ((fd = Requester::rq->openLockfile(QFile::encodeName(lockfile), O_RDONLY)) >= 0) {
      // Mario: it's not necessary to read more than lets say 32 bytes. If
      // file has more than 32 bytes, skip the rest
      char oldlock[33]; // safe
      int sz = read(fd, &oldlock, 32);
      close (fd);
      if (sz <= 0)
        return 1;
      oldlock[sz] = '\0';

      kdDebug(5002) << "Device is locked by: " << &oldlock << endl;

      int oldpid;
      int match = sscanf(oldlock, "%d", &oldpid);

      // found a pid in lockfile ?
      if (match < 1 || oldpid <= 0)
        return 1;

      // check if process exists
      if (kill((pid_t)oldpid, 0) == 0 || errno != ESRCH)
        return 1;

      kdDebug(5002) << "lockfile is stale" << endl;
    }
  }

  fd = Requester::rq->openLockfile(gpppdata.modemDevice(),
                                   O_WRONLY|O_TRUNC|O_CREAT);
  if(fd >= 0) {
    sprintf(newlock,"%010d\n", getpid());
    kdDebug(5002) << "Locking Device: " << newlock << endl;

    write(fd, newlock, strlen(newlock));
    close(fd);
    modem_is_locked=true;

    return 0;
  }

  return -1;

}


// UnLock modem device
void Modem::unlockdevice() {
  if (modem_is_locked) {
    kdDebug(5002) << "UnLocking Modem Device" << endl;
    Requester::rq->removeLockfile();
    modem_is_locked=false;
  }
}

void alarm_handler(int) {
  //  fprintf(stderr, "alarm_handler(): Received SIGALRM\n");

  // jump
  siglongjmp(jmp_buffer, 1);
}

#include "modem.moc"
