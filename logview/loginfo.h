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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __LOGINFO__H__
#define __LOGINFO__H__
#define MYDEBUG

#include <qdatetime.h>
#include <qstring.h>
#include <time.h>

class LogInfo {
public:
  LogInfo(QCString data);

  int error() const;

  QDateTime from() const;
  time_t from_t() const { return _from; }	
  QDateTime until() const;
  int duration() const;
  QString connectionName() const;
  QString currency() const;
  double sessionCosts() const;
  double totalCosts() const;
  int bytesIn() const;
  int bytesOut() const;
  int bytes() const;

#ifdef MYDEBUG
  void dump();
#endif

private:
  void parse(QCString );

  int errorfield;

  time_t _from, _until;
  QString _conname, _currency;
  double _session_cost, _total_cost;
  int _bytes_in, _bytes_out;
};


#endif
