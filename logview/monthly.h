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


#ifndef __MONTHLY__H__
#define __MONTHLY__H__

#include "log.h"
#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvaluelist.h>
#include <qdatetime.h>

#include <kbuttonbox.h>

class KListView;
class QComboBox;
class LogListItem;

class MonthlyWidget : public QWidget {
  Q_OBJECT
public:
  MonthlyWidget(QWidget *parent = 0);

private slots:
  void prevMonth();
  void nextMonth();
  void currentMonth();
  void slotConnections(int);
  void exportWizard();
  void slotSelectionChanged();

private:
  void layoutWidget();
  void plotMonth();

  /**
   * Returns the first day in the period
   */
  QDate periodeFirst() const;
  /**
   * Returns the last day in the period
   */
  QDate periodeLast() const;

  QDate m_periodeFirst; // First day in month
  //int _month, _year;

  QComboBox *cboConnections;
  KButtonBox *bbox;
  KListView *lv;
  KListView *lv2;
  LogListItem* selectionItem;
  QLabel *title;
  QPushButton *next, *prev, *today, *exportBttn;
  QValueList<QString> lstConnections;
  QGridLayout *tl;
  QDateTime *dt;
};

#endif
