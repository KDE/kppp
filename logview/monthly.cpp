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

#include "monthly.h"

#include <klocale.h>
#include <klistview.h>

static void formatBytes(int bytes, QString &result) {
  if(bytes < 1024)
    result.setNum(bytes);
  else if(bytes < 1024*1024)
    result.sprintf(i18n("%0.1f KB"), (float)bytes / 1024.0);
  else
    result.sprintf(i18n("%0.1f MB"), (float)bytes / 1024.0 / 1024.0);
}

static void formatDuration(int seconds, QString &result) {
  if(seconds < 60)
    result.sprintf(i18n("%d s"), seconds);
  else if(seconds < 3600)
    result.sprintf(i18n("%dm %ds"), seconds/60, seconds%60);
  else
    result.sprintf(i18n("%dh %dm %ds"), seconds/3600, (seconds % 3600)/60, seconds%60);
}

MonthlyWidget::MonthlyWidget(QWidget *parent) :
  QWidget(parent)
{
  tl = 0;

  lv = new KListView(this);
  lv->addColumn(i18n("Connection"));
  lv->addColumn(i18n("Day"));
  lv->addColumn(i18n("From"));
  lv->addColumn(i18n("Until"));
  lv->addColumn(i18n("Seconds"));
  lv->addColumn(i18n("Costs"));
  lv->addColumn(i18n("Bytes in"));
  lv->addColumn(i18n("Bytes out"));
  lv->setColumnAlignment(1, AlignRight);
  lv->setColumnAlignment(2, AlignRight);
  lv->setColumnAlignment(3, AlignRight);
  lv->setColumnAlignment(4, AlignRight);
  lv->setColumnAlignment(5, AlignRight);
  lv->setColumnAlignment(6, AlignRight);
  lv->setColumnAlignment(7, AlignRight);
  lv->setAllColumnsShowFocus(true);
  lv->setShowSortIndicator(true);
  lv->setItemMargin(2);
  lv->setSorting(1);
  lv->setMinimumWidth(320);
  lv->setMinimumHeight(200);

  title = new QLabel("X", this);
  QFont f = title->font();
  f.setPointSize(f.pointSize() + 2);
  f.setBold(TRUE);
  title->setFont(f);
  title->setFixedHeight(title->sizeHint().height()*2);

  bbox = new KButtonBox(this, KButtonBox::VERTICAL);
  prev = bbox->addButton(i18n("Prev month"));
  next = bbox->addButton(i18n("Next month"));
  bbox->addStretch(1);
  today = bbox->addButton(i18n("Current month"));

  connect(prev, SIGNAL(released()),
	  this, SLOT(prevMonth()));
  connect(next, SIGNAL(released()),
	  this, SLOT(nextMonth()));
  connect(today, SIGNAL(released()),
	  this, SLOT(currentMonth()));

  bbox->addStretch(8);
  bbox->layout();

  currentMonth();
  layoutWidget();
}

void MonthlyWidget::layoutWidget() {
  if(tl)
    delete tl;
  tl = new QVBoxLayout(this, 10, 10);
  tl->addWidget(title);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1, 1);
  l1->addWidget(lv, 1);
  l1->addWidget(bbox);

  tl->activate();
}

int bestlen(QWidget *w, const char *s) {
  return w->fontMetrics().boundingRect(s).width() + 8;
}

void MonthlyWidget::plotMonth() {
  QString months[] = {i18n("January"),
		      i18n("February"),
		      i18n("March"),
		      i18n("April"),
		      i18n("May"),
		      i18n("June"),
		      i18n("July"),
		      i18n("August"),
		      i18n("September"),
		      i18n("October"),
		      i18n("November"),
		      i18n("December")};

  // search the entries for this month
  QString s;
  int lastday = -1;
  QString lastConn = "-";

  // for collecting monthly statistics
  int count = 0;
  double costs = 0;
  int bin = 0, bout = 0;
  int duration = 0;
  QListViewItem* last = 0;

  for(int i = 0; i < (int)logList.count(); i++) {
    LogInfo *li = logList.at(i);

    if(li->from().date().month() == _month && li->from().date().year() == _year) {

      count++;
      costs += li->sessionCosts();
      if(bin >= 0) {
        if(li->bytesIn() < 0)
          bin = -1;
        else
          bin += li->bytesIn();
      }

      if(bout >= 0) {
        if(li->bytesOut() < 0)
          bout = -1;
        else
          bout += li->bytesOut();
      }

      duration += li->from().secsTo(li->until());

      QString bin, bout, b;
      if(li->bytesIn() >= 0)
        formatBytes(li->bytesIn(), bin);
      else
        bin = i18n("n/a");

      if(li->bytesOut() >= 0)
        formatBytes(li->bytesOut(), bout);
      else
        bout = i18n("n/a");

      if(li->bytes() > 0)
        formatBytes(li->bytes(), b);
      else
        b = i18n("n/a");

      QString day;
      QString con;
      if(li->from().date().day() != lastday) {
        day.sprintf("%2d", li->from().date().day());
        lastday = li->from().date().day();
      }

      if(li->connectionName() != lastConn) {
        con = li->connectionName();
        lastConn = li->connectionName();
      } else
        con = " ";

      QString s_duration;
      formatDuration(li->from().secsTo(li->until()),
                     s_duration);

      QString s_lifrom, s_liuntil, s_costs;
      s_lifrom.sprintf("%02d:%02d",
                       li->from().time().hour(), li->from().time().minute());
      s_liuntil.sprintf("%02d:%02d",
                        li->until().time().hour(), li->until().time().minute());
      s_costs.sprintf("%6.2f",
                      li->sessionCosts());

      last = new QListViewItem(lv, con, day, s_lifrom, s_liuntil, s_duration,
                               s_costs, bin, bout);
    }
  }

  if(count) {
    QString s;
    QString _bin, _bout, _b;

    if(bin < 0)
      _bin = i18n("n/a");
    else
      formatBytes(bin, _bin);

    if(bout < 0)
      _bout = i18n("n/a");
    else
      formatBytes(bout, _bout);

    if(bin < 0 || bout < 0)
      _b = i18n("n/a");
    else
      formatBytes(bout + bin, _b);

    QString s_duration;
    formatDuration(duration,
                   s_duration);

    QString s_costs;
    s.sprintf("%6.2f", costs);

#if 0
    last = new QListViewItem(lv, last,
                             i18n("%1 connections").arg(count),
                             " ", " ", " ",
                             s_duration, s_costs, _bin, _bout);
#endif
  }

  QString t;
  if(lv->childCount() > 0)
    t.sprintf(i18n("Connection log for %s %d"),
	      months[_month-1].data(),
	      _year);
  else
    t.sprintf(i18n("No connection log for %s %d available"),
	      months[_month-1].data(),
	      _year);

  title->setText(t.data());
}

void MonthlyWidget::nextMonth() {
  _month++;
  if(_month == 13) {
    _month = 1;
    _year++;
  }
  plotMonth();
}

void MonthlyWidget::prevMonth() {
  _month--;
  if(_month == 0) {
    _month = 12;
    _year--;
  }
  plotMonth();
}

void MonthlyWidget::currentMonth() {
  _month = QDate::currentDate().month();
  _year  = QDate::currentDate().year();
  plotMonth();
}


#include "monthly.moc"
