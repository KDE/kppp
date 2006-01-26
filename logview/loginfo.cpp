/*
 * kPPPlogview: a accounting log system for kPPP
 *
 *            Copyright (C) 1998 Mario Weilguni <mweilguni@kde.org>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "loginfo.h"
#include <stdlib.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <kdebug.h>

char *mystrsep (char **stringp, const char *delim);

LogInfo::LogInfo(QCString data) {
  parse(data);
}

QDateTime LogInfo::from() const {
  QDateTime tm;
  tm.setTime_t(_from);
  return tm;
}

QDateTime LogInfo::until() const {
  QDateTime tm;
  tm.setTime_t(_until);
  return tm;
}

QString LogInfo::connectionName() const {
  return _conname;
}

QString LogInfo::currency() const {
  return _currency;
}

double LogInfo::sessionCosts() const {
  return _session_cost;
}

double LogInfo::totalCosts() const {
  return _total_cost;
}

double LogInfo::bytesIn() const {
  return _bytes_in;
}

double LogInfo::bytesOut() const {
  return _bytes_out;
}

double LogInfo::bytes() const {
  if(bytesIn() == -1 || bytesOut() == -1)
    return -1;
  else
    return bytesIn() + bytesOut();
}

int LogInfo::error() const {
  return errorfield;
}

void LogInfo::parse(QCString s) {
  errorfield = 0;
  char *c = (char *)malloc(s.length() + 1), *csep;
  strcpy(c, s);

  // init data
  _from = _until = 0;
  _conname = "";
  _currency = "";
  _bytes_in = _bytes_out = -1;
  _session_cost = _total_cost = -1;

  // start of connection time
  csep = c;
  char *p = mystrsep(&csep, ":");
  int i = 0;
  while(i < 8 && p != 0) {
    QString token = QString::fromLocal8Bit(p);

    switch(i) {
    case 0:
      _from = token.toULong();
      break;

    case 1:
      _conname = token;
      break;

    case 2:
      _currency = token;
      break;

    case 3:
      _until = token.toULong();
      break;

    case 4:
      _session_cost = token.toFloat();
      break;

    case 5:
      _total_cost = token.toFloat();
      break;

    case 6:
      _bytes_in = token.toLong();
      break;
      
    case 7:
      _bytes_out = token.toLong();
      break;
    }

    i++;
    p = mystrsep(&csep, ":");
  }

  free(c);

  if(i == 8)
    errorfield = 0;
  else
    errorfield = i+1;
}


int LogInfo::duration() const {
  if( _until - _from < 0) 
    return 0;
  else
    return _until - _from;
}

#ifdef MYDEBUG
void LogInfo::dump() {
  kdDebug(5002) << "LOGINFO " << this << endl;
  kdDebug(5002) << "connection name : " << connectionName() << endl;
  kdDebug(5002) << "currency symbol : " << currency() << endl;
  kdDebug(5002) << "begin           : " << ctime(&_from) << endl;
  kdDebug(5002) << "end             : " << ctime(&_until) << endl;
  kdDebug(5002) << "duration        : " << (int)_until - (int)_from << " seconds" << endl;
  kdDebug(5002) << "session cost    : " << sessionCosts() << endl;
  kdDebug(5002) << "total cost      : " << totalCosts() << endl;
  kdDebug(5002) << "bytes in        : " << bytesIn() << endl;
  kdDebug(5002) << "bytes out       : " << bytesOut() << endl;
  kdDebug(5002) << "bytes total     : " << bytes() << endl;
  kdDebug(5002) << endl;
}
#endif

char *mystrsep (char **stringp, const char *delim) {
  char *begin, *end;

  begin = *stringp;
  if (begin == 0L)
    return 0L;

  /* Find the end of the token.  */
  end = strpbrk (begin, delim);
  if (end) {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = '\0';
      *stringp = end;
  } else
    /* No more delimiters; this is the last token.  */
    *stringp = 0L;

  return begin;
}


