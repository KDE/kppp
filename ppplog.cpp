/*
 *
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 * (c) 1998 Mario Weilguni <mweilguni@kde.org>
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <qlabel.h>
#include <qdir.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

#include <kdialog.h>
#include <qregexp.h>
#include <QTextEdit>
#include <qlayout.h>

#include <kmessagebox.h>
#include <kvbox.h>

#include "pppdata.h"
#include "requester.h"
#include <klocale.h>


int PPPL_MakeLog(QStringList &list) {
  int pid = -1, newpid;
  char buffer[1024], *p;
  const char *pidp;
  int fd;

  fd = Requester::rq->openSysLog();
  if(fd < 0) {
    list.append(i18n("Cannot open any of the following logfiles:"));
    const char * const * logFile = &kppp_syslog[0];
    while(*logFile) {
      list.append(*logFile);
      logFile++;
    }
    return 1;
  }

  FILE *f = fdopen(fd, "r");
  while(fgets(buffer, sizeof(buffer), f) != 0) {
    // pppd line ?
    p = (char *)strstr(buffer, "pppd[");
    if(p == 0)
      continue;
    pidp = p += strlen("pppd[");
    while(*p && isdigit(*p))
      p++;
    if(*p != ']')
      continue;

    /* find out pid of pppd */
    sscanf(pidp, "%d", &newpid);
    if(newpid != pid) {
      pid = newpid;
      list.clear();
    }
    if(buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = '\0';
    list.append(buffer);
  }
  fclose(f);

  if(list.isEmpty())
    return 2;

  /* clear security related info */

  const char *keyword[] = {"name = \"",
		    "user=\"",
		    "password=\"",
		    0};

  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
  {
    QByteArray tmp = (*it).toLocal8Bit();
    for(int j = 0; keyword[j] != 0; j++) {
      char *p;

      if( (p = (char *)strstr(tmp.data(), keyword[j])) != 0) {
	p += strlen(keyword[j]);
	while(*p && *p != '"')
	  *p++ = 'X';
      }
    }

  }

  return 0;
}


void PPPL_AnalyseLog(QStringList &list, QStringList &result) {
  QString msg;
  const char *rmsg = "Remote message: ";

  result.clear();

  // setup the analysis database
  struct {
    const char *regexp;
    const char *answer;
  } hints[] = {
    {"Receive serial link is not 8-bit clean",
     I18N_NOOP("You have launched pppd before the remote server " \
	  "was ready to establish a PPP connection.\n"
	  "Please use the terminal-based login to verify") },

    {"Serial line is looped back",
     I18N_NOOP("You have not started the PPP software on the peer system.") },

    {"AP authentication failed",
     I18N_NOOP("Check that you supplied the correct username and password.")} ,

    {"is locked by pid",
     I18N_NOOP("You should not pass 'lock' as an argument to pppd. "
	  "Check /etc/ppp/options and ~/.ppprc") },

    {"CP: timeout sending",
     I18N_NOOP("The remote system does not seem to answer to\n"
	  "configuration request. Contact your provider.") },

    {"unrecognized option",
     I18N_NOOP("You have passed an invalid option to pppd. See 'man pppd' "
          "for a complete list of valid arguments.") },

    // terminator
    {0,0}
  };


  // scan the log for keywords and try to offer any help
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
  {
    // look for remote message
    int pos = (*it).indexOf(rmsg);
    if (pos != -1)
    {
      QString str = (*it);
      str.remove(0, pos + strlen(rmsg));
      if(!str.isEmpty()) {
        msg = i18n("Notice that the remote system has sent the following"
                         " message:\n\"%1\"\nThis may give you a hint why the"
                         " connection has failed.", str);
        result.append(msg);
      }
    }

    // check in the hint database
    for(uint k = 0; hints[k].regexp != 0; k++) {
      QRegExp rx(hints[k].regexp);
      QString l(*it);
      if(l.contains(rx)) {
	result.append(i18n(hints[k].answer));
	break;
      }
    }
  }

  if (result.isEmpty())
    result.append(i18n("Unable to provide help."));
}


void PPPL_ShowLog() {
  QStringList sl, result;

  PPPL_MakeLog(sl);

  bool foundConnect = false;
  bool foundLCP = gpppdata.getPPPDebug();
  QString lcp = QLatin1String("[LCP");
  QString conn = QLatin1String("Connect:");
  QStringList::ConstIterator it = sl.constBegin();
  for( ; it != sl.constEnd(); it++) {
    if((*it).indexOf(lcp) >= 0) {
      foundLCP = true;
      break;
    }
    if((*it).contains(conn))
      foundConnect = true;
  }
  if(foundConnect && !foundLCP) {
    int result = KMessageBox::warningYesNo(0,
				i18n("KPPP could not prepare a PPP log. It is very likely "
				     "that pppd was started without the \"debug\" option.\n"
				     "Without this option it is difficult to find out PPP "
				     "problems, so in general the debug option should be used.\n"
				     "Enable debug now, and restart pppd?"), QString(), KGuiItem(i18n("Restart pppd")), KGuiItem(i18n("Do Not Restart")));

    if(result == KMessageBox::Yes) {
      gpppdata.setPPPDebug(true);
      KMessageBox::information(0,
		       i18n("The \"debug\" option has been added. You "
			    "should now try to reconnect. If that fails "
			    "again, you will get a PPP log that may help "
					"you to track down the connection problem."));
      //      return;
    }

    //    return;
  }

  PPPL_AnalyseLog(sl, result);

  KDialog *dlg = new KDialog();
  dlg->setButtons(KDialog::Close | KDialog::Ok);
  dlg->setWindowTitle(i18n("PPP Log"));
  dlg->setButtonText(KDialog::Ok,i18n("Write to File"));

  KVBox* v = new KVBox(dlg);
  QTextEdit *edit = new QTextEdit(v);
  edit->setReadOnly(true);
  QLabel *label = new QLabel(i18n("kppp's diagnosis (just guessing):"), v);
  QTextEdit *diagnosis = new QTextEdit(v);
  diagnosis->setReadOnly(true);
  edit->setMinimumSize(600, 250);
  label->setMinimumSize(600, 15);
  diagnosis->setMinimumSize(600, 60);

  dlg->setMainWidget(v);

  for(int i = 0; i < sl.count(); i++)
    edit->append(sl.at(i));
  for(int i = 0; i < result.count(); i++)
    diagnosis->append(result.at(i));

  if(dlg->exec()) {
    QDir d = QDir::home();
    QString s = d.absolutePath() + "/PPP-logfile";
    int old_umask = umask(0077);

    FILE *f = fopen(QFile::encodeName(s), "w");
    for(int i = 0; i < sl.count(); i++)
      fprintf(f, "%s\n", sl.at(i).toLocal8Bit().data());
    fclose(f);
    umask(old_umask);

    QString msg = i18n("The PPP log has been saved\nas \"%1\".\n\nIf you want to send a bug report, or have\nproblems connecting to the Internet, please\nattach this file. It will help the maintainers\nto find the bug and to improve KPPP", s);
    KMessageBox::information(0, msg);
  }
  delete dlg;
}
