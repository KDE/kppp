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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "log.h"

#include <stdlib.h>
#include <stdio.h>

#include <QProgressDialog>
#include <QApplication>
#include <qdir.h>

#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>

Q3PtrList<LogInfo> logList;
QProgressDialog *dlg;

int loadLogs() {
  int logsize = 0;

  QString logdirname = KStandardDirs::locateLocal("data", "kppp/Log/");
  QDir logdir(logdirname, "*.log");

  kDebug(5002) << "logdirname: " << logdirname << endl;
  
  // get log file size
  QFileInfoList list = logdir.entryInfoList();
  QFileInfoList::const_iterator it = list.begin();
  QFileInfoList::const_iterator itEnd = list.end();
  QFileInfo fi;

  while (it != itEnd) {
    fi = *it;
    logsize += fi.size();
    ++it;
  }

  dlg = new QProgressDialog(i18n("Loading log files"),
			     QString::null,
			     0,logsize,0, 0);
  dlg->setValue(0);

  // load logs
  list = logdir.entryInfoList();
  QFileInfoListIterator it1 = list.begin();
  QFileInfoListIterator it1End = list.end();

  int retval = 0;
  while (it1 != it1End) {
    fi = *it1;
    retval += loadLog(fi.absoluteFilePath());
    ++it1;
  }

  delete dlg; 
  return retval;
}

int loadLog(const QString &fname) {
  FILE *f;
  int warning=0;

  f = fopen(QFile::encodeName(fname), "r");
  if(f == NULL)
    return 1;

  char buffer[2048+1];
  int lineno = 0;
  while(fgets(buffer, sizeof(buffer), f) != NULL) {
    ++lineno;
    buffer[sizeof(buffer) - 1] = 0;

    int slen = strlen(buffer);

    // skip blank lines
    if(slen < 10)
      continue;
    
    dlg->setValue(dlg->value() + slen);
    qApp->processEvents();

    LogInfo *li = new LogInfo(buffer);    
    if(li->error()) {

      // check if the connection has been terminated abnormally
      if(li->error() != 3) {
	warning++;    
	kError() << "ERROR IN FILE " << fname << " LINE " << lineno << "\"" << buffer << "\" (" << li->error() << ")" << endl;
	delete li;
      } else
	logList.append(li);
    } else
      logList.append(li);    
  }

  fclose(f);

  if(warning)
    return 2;
  else
    return 0;
}

int QLogList::compareItems(Item a, Item b) {
  LogInfo *la = (LogInfo *)a;
  LogInfo *lb = (LogInfo *)b;

  if(la->from() < lb->from())
    return -1;
  else if(la->from() > lb->from())
    return 1;
  else
    return 0;
}

