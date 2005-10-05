/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 * This file contributed by: Mario Weilguni, <mweilguni@sime.com>
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

#ifndef __ACCOUNTING__H__
#define __ACCOUNTING__H__

#include <qobject.h>
//Added by qt3to4:
#include <QTimerEvent>
#include <kprocess.h>
#include "ruleset.h"

class PPPStats;

/////////////////////////////////////////////////////////////////////////////
//
// Accounting base class
//
/////////////////////////////////////////////////////////////////////////////
class AccountingBase : public QObject {
  Q_OBJECT
public:
  AccountingBase(QObject *parent);
  virtual ~AccountingBase();

  virtual double total() const;
  virtual double session() const;

  virtual bool running() const { return false; };
  virtual bool loadRuleSet(const QString & name) = 0;

public slots:
  virtual void slotStart() {};
  virtual void slotStop() {};

signals:
  void changed(QString total, QString session);

protected:
  void logMessage(QString, bool = FALSE);
  bool saveCosts();
  bool loadCosts();

  QString LogFileName;
  double _total, _session;
  QString _name;

  // static members
public:
  static void resetCosts(const QString & accountname);
  static void resetVolume(const QString & accountname);
  static QString getCosts(const QString & accountname);
  static QString getAccountingFile(const QString &accountname);
};


/////////////////////////////////////////////////////////////////////////////
//
// Accounting based on ruleset files
//
/////////////////////////////////////////////////////////////////////////////
class Accounting : public AccountingBase {
  Q_OBJECT
public:
  Accounting(QObject *parent, PPPStats *st);

  virtual double total() const;
  virtual double session() const;

  virtual bool loadRuleSet(const QString & name);
  virtual bool running() const;

private:
  virtual void timerEvent(QTimerEvent *t);

public slots:
  virtual void slotStart();
  virtual void slotStop();

signals:
  void changed(QString total, QString session);

private:
  int acct_timer_id;
  int update_timer_id;
  time_t start_time;
  double _lastcosts;
  double _lastlen;
  RuleSet rules;
  PPPStats *stats;
};


/////////////////////////////////////////////////////////////////////////////
//
// Accounting based on executable files
//
/////////////////////////////////////////////////////////////////////////////
class ExecutableAccounting : public AccountingBase {
  Q_OBJECT
public:
  ExecutableAccounting(PPPStats *st, QObject *parent = 0);

  virtual bool loadRuleSet(const QString & );
  virtual bool running() const;

public slots:
  virtual void slotStart();
  virtual void slotStop();

private slots:
  void gotData(KProcess *proc, char *buffer, int buflen);

signals:
  void changed(QString total, QString session);

private:
  KProcess *proc;
  QString currency;
  QString provider;
  PPPStats *stats;
};

#endif
