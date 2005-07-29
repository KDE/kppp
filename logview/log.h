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
 * Software Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __LOG__H__
#define __LOG__H__

#include "loginfo.h"
#include <q3ptrlist.h>

typedef Q3PtrList<LogInfo>                    QLogInfoBase;
typedef Q3PtrListIterator<LogInfo>            QLogInfoIterator;

class QLogList : public QLogInfoBase {
public:
  virtual int compareItems(Item, Item);
};

extern Q3PtrList<LogInfo> logList;

int loadLogs();
int loadLog(QString);

#endif
