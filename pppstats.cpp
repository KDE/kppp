/*
 *
 * $Id$
 *
 * History:
 *
 * Bernd Wuebben, wuebben@math.cornell.edu:
 *
 * Much of this  is taken from the pppd sources in particular
 * /pppstat/pppstat.c, and modified to suit the needs of kppp.
 *
 *
 * Here the original history of pppstat.c:
 *
 * perkins@cps.msu.edu: Added compression statistics and alternate
 *                display. 11/94
 *
 * Brad Parker (brad@cayman.com) 6/92
 *
 * from the original "slstats" by Van Jaconson
 *
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	Van Jacobson (van@helios.ee.lbl.gov), Dec 31, 1989:
 *	- Initial distribution.
 */


#include <config.h>

#include <ctype.h>
#include <errno.h>

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/ppp_defs.h>

#include "config.h"
#include "pppstats.h"

#ifndef STREAMS
 #if defined(__linux__) && defined(__powerpc__) \
    && (__GLIBC__ == 2 && __GLIBC_MINOR__ == 0)
  /* kludge alert! */
  #undef __GLIBC__
 #endif
 #include <sys/socket.h>		/* *BSD, Linux, NeXT, Ultrix etc. */
 #ifndef HAVE_NET_IF_PPP_H
  #ifdef HAVE_LINUX_IF_PPP_H
   #include <linux/if.h>
   #include <linux/if_ppp.h>
  #endif
 #else
  #include <net/if.h>
  #include <net/if_ppp.h>
 #endif

#else	/* STREAMS */
 #include <sys/socket.h>
 #include <sys/stropts.h>	/* SVR4, Solaris 2, SunOS 4, OSF/1, etc. */
 #include <net/ppp_defs.h>
 #include <net/pppio.h>
 #include <net/if.h>
 #include <sys/sockio.h>

#endif	/* STREAMS */

#include <qtimer.h>
#include <kdebug.h>

PPPStats::PPPStats()
{
  clear();
  timer = new QTimer;
  connect(timer, SIGNAL(timeout()), SLOT(timerClick()));
}


PPPStats::~PPPStats() {
  stop();
  delete timer;
}


void PPPStats::clear()
{
  ibytes = 0;
  ipackets = 0;
  ibytes_last = 0;
  obytes_last = 0;
  compressedin = 0;
  uncompressedin = 0;
  errorin = 0;
  obytes = 0;
  opackets = 0;
  compressed = 0;
  packetsunc = 0;
  packetsoutunc = 0;
  ioStatus = BytesNone;
}

void PPPStats::timerClick() {
  enum IOStatus newStatus;

  doStats();

  if((ibytes != ibytes_last) && (obytes != obytes_last))
    newStatus = BytesBoth;
  else if(ibytes != ibytes_last)
    newStatus = BytesIn;
  else if(obytes != obytes_last)
    newStatus = BytesOut;
  else
    newStatus = BytesNone;

  if(newStatus != ioStatus)
    emit statsChanged(ioStatus = newStatus);

  ibytes_last = ibytes;
  obytes_last = obytes;
}

void PPPStats::setUnit(int u) {
  unit = u;
  sprintf(unitName, "ppp%d", unit);
}


void PPPStats::start() {
  timer->start(PPP_STATS_INTERVAL);
}


void PPPStats::stop() {
  emit statsChanged(BytesNone);
  timer->stop();
}


bool PPPStats::ifIsUp() {
  bool is_up;
  struct ifreq ifr;

#if defined(__svr4__ )
  usleep(1000000); // Needed for Solaris ?!
#endif

#ifdef STREAMS
    if ((t = open("/dev/ppp", O_RDONLY)) < 0) {
	perror("pppstats: Couldn't open /dev/ppp: ");
	return false;
    }
    if (!strioctl(t, PPPIO_ATTACH, (char*)&unit, sizeof(int), 0)) {
	fprintf(stderr, "pppstats: ppp%d is not available\n", unit);
	::close(t);
	return false;
    }
    // TODO: close t somewhere again
#endif
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("Couldn't create IP socket");
	return false;
    }

    strlcpy(ifr.ifr_name, unitName, sizeof(ifr.ifr_name));

    if(ioctl(s, SIOCGIFFLAGS, (caddr_t) &ifr) < 0) {
        if (errno)
          fprintf(stderr, "Couldn't find interface %s: %s\n",
                  unitName, strerror(errno));
	::close(s);
	s = 0;
	return 0;
    }

    if ((ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING)) {
	is_up = true;
	kdDebug(5002) << "Interface is up" << endl;
    }
    else{
      is_up = false;
      ::close(s);
      s = 0;
      kdDebug(5002) << "Interface is down" << endl;
    }

    return is_up;
}


bool PPPStats::initStats() {

  struct sockaddr_in *sinp;
  struct ifreq ifr;

  clear();

  strlcpy(ifr.ifr_name, unitName, sizeof(ifr.ifr_name));

  if (ioctl(s, SIOCGIFADDR, &ifr) < 0) {
  }

  sinp = (struct sockaddr_in*)&ifr.ifr_addr;

  if(sinp->sin_addr.s_addr)
    local_ip_address = inet_ntoa(sinp->sin_addr);
  else
    local_ip_address = "";
  kdDebug(5002) << "Local IP: " << local_ip_address << endl;

  if (ioctl(s, SIOCGIFDSTADDR, &ifr) < 0)
    ;

  sinp = (struct sockaddr_in*)&ifr.ifr_dstaddr;

  if(sinp->sin_addr.s_addr)
    remote_ip_address = inet_ntoa(sinp->sin_addr);
  else
    remote_ip_address = "";
  kdDebug(5002) << "Remote IP: " << remote_ip_address << endl;

  return true;

}


bool PPPStats::doStats() {
  struct ppp_stats cur;

  if(! get_ppp_stats(&cur)){
    return false;
  }

  // "in"  "pack"  "comp"  "uncomp"  "err"
  // IN    PACK    VJCOMP  VJUNC     VJERR

  ibytes =   cur.p.ppp_ibytes; 			// bytes received
  ipackets =   cur.p.ppp_ipackets; 		// packets recieved
  compressedin =  cur.vj.vjs_compressedin; 	// inbound compressed packets
  uncompressedin =   cur.vj.vjs_uncompressedin; // inbound uncompressed packets
  errorin =  cur.vj.vjs_errorin; 		//receive errors

  //  "out"  "pack"  "comp"  "uncomp"  "ip"
  // OUT       PACK   JCOMP   VJUNC    NON-VJ

  obytes =  cur.p.ppp_obytes; 		       	// raw bytes sent
  opackets =  cur.p.ppp_opackets; 		// packets sent
  compressed =  cur.vj.vjs_compressed; 		//outbound compressed packets

  // outbound packets - outbound compressed packets
  packetsunc =  cur.vj.vjs_packets - cur.vj.vjs_compressed;

  // packets sent - oubount compressed
  packetsoutunc = cur.p.ppp_opackets - cur.vj.vjs_packets;

  return true;
}


#ifndef STREAMS
bool PPPStats::get_ppp_stats(struct ppp_stats *curp){

  struct ifpppstatsreq req;

    if(s==0)
      return false;

#ifdef __linux__
    req.stats_ptr = (caddr_t) &req.stats;
    sprintf(req.ifr__name, "ppp%d", unit);
#else
    sprintf(req.ifr_name, "ppp%d", unit);
#endif
    if (ioctl(s, SIOCGPPPSTATS, &req) < 0) {
	if (errno == ENOTTY)
	    fprintf(stderr, "pppstats: kernel support missing\n");
	else
	    perror("ioctl(SIOCGPPPSTATS)");
	return false;
    }
    *curp = req.stats;
    return true;
}

#else	/* STREAMS */
bool PPPStats::get_ppp_stats( struct ppp_stats *curp){

    if (!strioctl(t, PPPIO_GETSTAT, (char*)curp, 0, sizeof(*curp))) {
	if (errno == EINVAL)
	    fprintf(stderr, "pppstats: kernel support missing\n");
	else
	    perror("pppstats: Couldn't get statistics");
	return false;
    }
    return true;
}

bool PPPStats::strioctl(int fd, int cmd, char* ptr, int ilen, int olen){

    struct strioctl str;

    str.ic_cmd = cmd;
    str.ic_timout = 0;
    str.ic_len = ilen;
    str.ic_dp = ptr;
    if (ioctl(fd, I_STR, &str) == -1)
	return false;
    if (str.ic_len != olen)
	fprintf(stderr, "strioctl: expected %d bytes, got %d for cmd %x\n",
	       olen, str.ic_len, cmd);
    return true;
}
#endif /* STREAMS */

#include "pppstats.moc"

