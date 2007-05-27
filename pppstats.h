/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 * (c) 1997-1998 Bernd Johannes Wuebben <wuebben@kde.org>
 * (c) 1997-1999 Mario Weilguni <mweilguni@kde.org>
 * (c) 1998-1999 Harri Porten <porten@kde.org>
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

#ifndef _PPPSTATS_H_
#define _PPPSTATS_H_


#include "kpppconfig.h"
#include <qobject.h>

#ifdef __FreeBSD__
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_ppp.h>
#endif

class QTimer;

class PPPStats : public QObject {
  Q_OBJECT
public:
  PPPStats();
  ~PPPStats();
  void clear();
  bool initStats();
  bool doStats();
  bool ifIsUp();
  void setUnit(int u);
  void start();
  void stop();

signals:
  void statsChanged(int);

private slots:
  void timerClick();

public:
  int ibytes, obytes;
  int totalbytes;
  int ipackets, opackets;
  int compressedin;
  int uncompressedin;
  int compressed;
  int errorin;
  int packetsunc, packetsoutunc;

  QString local_ip_address;
  QString remote_ip_address;

  enum IOStatus { BytesNone = 0, BytesIn, BytesOut, BytesBoth };

private:
  bool get_ppp_stats(struct ppp_stats *curp);
  bool strioctl(int fd, int cmd, char* ptr,int ilen, int olen);

  int ibytes_last, obytes_last;
  int s;                      // socket file descriptor
#ifdef STREAMS
  int t;
#endif
  int unit;
  char unitName[5];
  enum IOStatus ioStatus;
  QTimer *timer;
};

#endif
