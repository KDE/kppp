/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 * This file was contributed by Mario Weilguni <mweilguni@sime.com>
 * Thanks Mario !
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ruleset.h"

#include <qregexp.h>
#include <qfile.h>

#include <klocale.h>
#include <kglobal.h>
#include <kdebug.h>

RuleSet::RuleSet() {
  default_costs = -1;
  default_len = -1;
  _currency_symbol = "$";
  _currency_digits = 2;
  _minimum_costs = 0;
  flat_init_costs = 0.0;
  flat_init_duration = 0;
  have_flat_init_costs = false;

  pcf = 0.0;
}

// this function is shamelessly stolen from pppcosts 0.5 :-)
/* calculates the easter sunday in day_of_year style */
QDate RuleSet::get_easter(int year) {
  /* not optimized, I took the original names */
  signed int a,b,m,q,w,p,n,tt,mm;

  /* calculating easter is really funny */
  /* this is O'Beirne's algorithm, only valid 1900-2099 */
  n = year - 1900;
  a = n % 19;
  b = (int)((7*a+1)/19);
  m = (11*a+4-b) % 29;
  q = (int)(n/4);
  w = (n+q+31-m) % 7;
  p = 25-m-w;
  if (p>0)
    {tt=p;
    mm=4;}
  else
    {tt=p+31;
    mm=3;}

  return QDate(year, mm, tt);
}

int RuleSet::dayNameToInt(const char *s) {
  const char *const name[] = {"monday", "tuesday", "wednesday",
			"thursday", "friday", "saturday",
			"sunday", NULL};

  for(int i = 0; name[i] != NULL; i++)
    if(qstricmp(name[i], s) == 0)
      return i;

  return -1;
}

int RuleSet::load(const QString &filename) {

  flat_init_costs = 0.0;
  flat_init_duration = 0;
  have_flat_init_costs = false;

  QFile f(filename);

  // delete old rules
  rules.resize(0);
  _name = "";

  // ignore "No Accounting"
  if(filename.isEmpty())
    return 0;

  if(!f.exists())
    return -1;

  if(!f.open(QIODevice::ReadOnly))
    return -1;

  char buffer[2048]; // safe
  int lineno=0;

  while(!f.atEnd()) {
    // read continued lines
    QString line;
    bool backslashed;
    do {
      int br = f.readLine(buffer, sizeof(buffer));
      if (br < 0) break;
      if((br > 0) && (buffer[br-1] == '\n'))
	buffer[br-1] = 0;
      else
	buffer[br] = 0;
      lineno++;
      line.append(QString::fromUtf8(buffer));
      backslashed = (line.right(1) == "\\");
    } while(!f.atEnd() && backslashed);

    // strip whitespace
    line = line.replace(QRegExp("[ \t\r]"), "");
    // skip comment lines
    if((line.left(1) == "#") || line.isEmpty())
      continue;

    // parse line
    if(!parseLine(line)) {
      f.close();
      kError(5002) << "ERROR IN LINE " << lineno << endl;
      return lineno;
    }
  }

  f.close();

  if(_name.length() > 0)
    return 0;
  else {
    kError(5002) << "NO NAME DEFINED" << endl;
    return -1;
  }
}

void RuleSet::addRule(RULE r) {
  // check for a default rule
  if((r.type == 2) &&
     (r.weekday.from == 0) && (r.weekday.until == 6) &&
     (r.from == midnight()) &&
     (r.until == beforeMidnight()))
    {
      default_costs = r.costs;
      default_len = r.len;
      return;
    }

  // if from < until (i.e on (monday..friday)
  // from (21:00..05:00) use (0.2,16)
  // split it into two rules
  // ... between (21:00..23:59) use ...
  // ... between (00:00..05:00) use ...
  if(r.from > r.until) {
    RULE r1, r2;
    r1 = r;
    r2 = r;
    r1.until = beforeMidnight();
    r2.from = midnight();
    rules.resize(rules.size()+2);
    rules[rules.size()-2] = r1;
    rules[rules.size()-1] = r2;
  } else {
    rules.resize(rules.size()+1);
    rules[rules.size()-1] = r;
  }
}

bool RuleSet::parseEntry(RULE &ret, QString s, int year) {
  if(s.contains(QRegExp("^[0-9]+/[0-9]+$"))) {
    int d, m;
    sscanf(s.toAscii(), "%d/%d", &m, &d);
    ret.type = 1;
    ret.date.from = QDate(year, m, d);
    ret.date.until = QDate(year, m, d);
    return true;
  }

  if(s.contains(QRegExp("^[0-9]+\\.[0-9]+$"))) {
    int d, m;
    sscanf(s.toAscii(), "%d.%d", &d, &m);
    ret.type = 1;
    ret.date.from = QDate(year, m, d);
    ret.date.until = QDate(year, m, d);
    return true;
  }

  if(s.right(3) == "day") {
    int d = dayNameToInt(s.toAscii());
    if(d != -1) {
      ret.type = 2;
      ret.weekday.from = d;
      ret.weekday.until = d;
      return true;
    }
  }

  if(s.left(6) == "easter") {
    QDate d = get_easter(year);
    int off;
    bool ok = true;
    QString val = s.mid(6, 1000);
    if(val.isEmpty())
      off = 0;
    else
      off = val.toInt(&ok);

    if(ok) {
      d = d.addDays(off);
      ret.type = 1;
      ret.date.from = d;
      ret.date.until = d;
      return true;
    }
  }

  ret.type = 0;
  return false;
}



bool RuleSet::parseEntries(QString s, int year,
			   QTime t1, QTime t2,
			   double costs, double len, double after)
{
  // special rule: on() is the same as on(monday..sunday)
  if(s.isEmpty())
    s = "monday..sunday";

  while(s.length()) {
    int pos = s.indexOf(',');
    QString token;
    if(pos == -1) {
      token = s;
      s = "";
    } else {
      token = s.left(pos);
      s = s.right(s.length()-pos-1);
    }

    // we've a token, now check if it defines a
    // range
    RULE r;
    if(token.contains("..")) {
      QString left, right;
      left = token.left(token.indexOf(".."));
      right = token.right(token.length()-2-left.length());
      RULE lr, rr;
      if(parseEntry(lr, left, year) && parseEntry(rr, right, year)) {
	if(lr.type == rr.type) {
	  r.type = lr.type;
	  switch(lr.type) {
	  case 1:
	    r.date.from = lr.date.from;
	    r.date.until = rr.date.from;
	    break;
	  case 2:
	    r.weekday.from = lr.weekday.from;
	    r.weekday.until = rr.weekday.from;
	  }
	} else
	  return false;
      }
    } else
      if(!parseEntry(r, token, year))
	return false;

    r.costs = costs;
    r.len = len;
    r.after = after;
    r.from = t1;
    r.until = t2;
    addRule(r);
  }

  return true;
}

bool RuleSet::parseTime(QTime &t1, QTime &t2, QString s) {
  if(s.isEmpty()) {
    t1 = midnight();
    t2 = beforeMidnight();
    return true;
  } else {
    int t1m, t1h, t2m, t2h;
    if(sscanf(s.toAscii(), "%d:%d..%d:%d", &t1h, &t1m, &t2h, &t2m) == 4) {
      t1.setHMS(t1h, t1m, 0);
      t2.setHMS(t2h, t2m, 0);
      return true;
    } else
      return false;
  }
}

bool RuleSet::parseRate(double &costs, double &len, double &after, QString s) {
  after = 0;
  int fields = sscanf(s.toAscii(), "%lf,%lf,%lf", &costs, &len, &after);
  return (fields == 2) || (fields == 3);
}

bool RuleSet::parseLine(const QString &s) {

  // ### use QRegExp::cap() instead of mid() and find()

  // for our french friends -- Bernd
  if(s.contains(QRegExp("flat_init_costs=\\(.*"))) {
    // parse the time fields
    QString token = s.mid(s.indexOf("flat_init_costs=(") + 17,
			  s.indexOf(")")-s.indexOf("flat_init_costs=(") - 17);
    //    printf("TOKEN=%s\n",token.ascii());

    double after;
    if(!parseRate(flat_init_costs, flat_init_duration, after, token))
      return false;

    //printf("COST %f DURATION %f\n",flat_init_costs,flat_init_duration);

    if(! (flat_init_costs >= 0.0) )
      return false;
    if(! (flat_init_duration >= 0.0))
      return false;

    have_flat_init_costs = true;
    return true;
  }


  if(s.contains(QRegExp("on\\(.*\\)between\\(.*\\)use\\(.*\\)"))) {
    // parse the time fields
    QString token = s.mid(s.indexOf("between(") + 8,
			  s.indexOf(")use")-s.indexOf("between(") - 8);
    QTime t1, t2;
    if(!parseTime(t1, t2, token))
      return false;

    // parse the rate fields
    token = s.mid(s.indexOf("use(") + 4,
			  s.lastIndexOf(")")-s.indexOf("use(") - 4);
    double costs;
    double len;
    double after;
    if(!parseRate(costs, len, after, token))
      return false;

    // parse the days
    token = s.mid(s.indexOf("on(") + 3,
		  s.indexOf(")betw")-s.indexOf("on(") - 3);
    if(!parseEntries(token, QDate::currentDate().year(),
		     t1, t2, costs, len, after))
      return false;

    return true;
  }

  // check for the name
  if(s.contains(QRegExp("name=.*"))) {
    _name = s.right(s.length()-5);
    return !_name.isEmpty();
  }


  // check default entry
  if(s.contains(QRegExp("default=\\(.*\\)"))) {
    QString token = s.mid(9, s.length() - 10);
    double after;
    if(parseRate(default_costs, default_len, after, token))
      return true;
  }

  // check for "minimum costs"
  if(s.contains(QRegExp("minimum_costs=.*"))) {
    QString token = s.right(s.length() - strlen("minimum_costs="));
    bool ok;
    _minimum_costs = token.toDouble(&ok);
    return ok;
  }

  // check currency settings
  if(s.startsWith("currency_symbol=")) {
     _currency_symbol = s.mid(16);
     return true;
  }

  if(s.contains(QRegExp("currency_digits=.*"))) {
    QString token = s.mid(16, s.length() - 16);
    bool ok;
    _currency_digits = token.toInt(&ok);
    return ok && (_currency_digits >= 0);
  }

  // "currency_position" is deprecated so we'll simply ignore it
  if(s.contains(QRegExp("currency_position=.*")))
    return true;

  // check per connection fee
  if(s.contains(QRegExp("per_connection="))) {
    QString token = s.mid(15, s.length()-15);
    bool ok;
    pcf = token.toDouble(&ok);
    return ok;
  }

  return false;
}

void RuleSet::setStartTime(const QDateTime &dt){

  starttime = dt;

}

void RuleSet::getActiveRule(const QDateTime &dt, double connect_time, double &costs, double &len) {
  // use default costs first
  costs = default_costs;
  len = default_len;

  //printf("In getActiveRule\n");
  if(have_flat_init_costs){

    costs = flat_init_costs;
    len = flat_init_duration;
    have_flat_init_costs = false;
    //printf("getActiveRule FLATINITCOSTS\n");
    return;
  }

  // check every rule
  for(int i = 0; i < (int)rules.size(); i++) {
    RULE r = rules[i];

    switch(r.type) {
    case 1: // a date
      {
	// since rules do not have a year's entry, use the one
	// from dt
	QDate from = r.date.from;
	QDate until = r.date.until;
	from.setYMD(dt.date().year(), from.month(), from.day());
	until.setYMD(dt.date().year(), until.month(), until.day());
	if((from <= dt.date()) && (dt.date() <= until)) {
	  // check time
	  if((r.from <= dt.time()) && (dt.time() <= r.until) && (connect_time >= r.after)) {
	    costs = r.costs;
	    len = r.len;
	  }
	}
      }
    break;

    case 2: // one or more weekdays
      // check if the range overlaps sunday.
      // (i.e. "on(saturday..monday)")
      if(r.weekday.from <= r.weekday.until) {
	if((r.weekday.from <= dt.date().dayOfWeek() - 1) &&
	   (r.weekday.until >= dt.date().dayOfWeek() - 1))
	  {
	    // check time
	    if((r.from <= dt.time()) && (dt.time() <= r.until) && (connect_time >= r.after)) {
	      costs = r.costs;
	      len = r.len;
	    }
	  }
      } else { // yes, they overlap sunday
	if((r.weekday.from >= dt.date().dayOfWeek() - 1) &&
	   (dt.date().dayOfWeek() - 1 <= r.weekday.until))
	  {
	    // check time
	    if((r.from <= dt.time()) && (dt.time() <= r.until) && (connect_time >= r.after)) {
	      costs = r.costs;
	      len = r.len;
	    }
	  }
      }
    }
  }
}


#if 0
static double round(double d, int digits) {
  d *= pow(10, digits);
  d = rint(d);
  d /= pow(10, digits);
  return d;
}
#endif

QString RuleSet::currencySymbol() const {
  return _currency_symbol;
}

QString RuleSet::currencyString(double f) const {
  return KGlobal::locale()->formatMoney(f, _currency_symbol, _currency_digits);
}


double RuleSet::perConnectionCosts() const {
  return pcf;
}


QString RuleSet::name() const {
  return _name;
}


double RuleSet::minimumCosts() const {
  return _minimum_costs;
}

QTime RuleSet::midnight() const {
  return QTime(0, 0, 0, 0);
}

QTime RuleSet::beforeMidnight() const {
  return QTime(23,59,59,999);
}

int RuleSet::checkRuleFile(const QString &rulefile) {
  if(rulefile.isEmpty()) {
    fputs(i18n("kppp: no rulefile specified\n").toLocal8Bit(), stderr);
    return 1;
  }

  QFile fl(rulefile);
  if(!fl.exists()) {
    fprintf(stderr, i18n("kppp: rulefile \"%s\" not found\n").toLocal8Bit(), rulefile.toLocal8Bit().data());
    return 1;
  }

  if(rulefile.right(4) != ".rst") {
    fputs(i18n("kppp: rulefiles must have the extension \".rst\"\n").toLocal8Bit(), stderr);
    return 1;
  }

  RuleSet r;
  int err = r.load(rulefile);
  fl.close();

  if(err == -1) {
    fputs(i18n("kppp: error parsing the ruleset\n").toLocal8Bit(), stderr);
    return 1;
  }

  if(err > 0) {
    fprintf(stderr, i18n("kppp: parse error in line %d\n").toLocal8Bit(), err);
    return 1;
  }

  // check for the existence of a default rule
  if((r.default_costs < 0) || (r.default_len < 0)) {
    fputs(i18n("kppp: rulefile does not contain a default rule\n").toLocal8Bit(), stderr);
    return 1;
  }

  if(r.name().length() == 0) {
    fputs(i18n("kppp: rulefile does not contain a \"name=...\" line\n").toLocal8Bit(), stderr);
    return 1;
  }

  fputs(i18n("kppp: rulefile is ok\n").toLocal8Bit(), stderr);
  return 0;
}

