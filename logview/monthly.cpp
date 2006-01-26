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

#include <qpainter.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qheader.h>

#include <kcalendarsystem.h>
#include <klocale.h>
#include <kglobal.h>
#include <klistview.h>
#include <kdebug.h>
#include <kmessagebox.h>

#include "monthly.h"
#include "export.h"
#include <qstringlist.h>

static void formatBytes(double bytes, QString &result) {
  if(bytes < 1024)
    result.setNum(bytes);
  else if(bytes < 1024*1024)
    result = i18n("%1 KB").arg(KGlobal::locale()->formatNumber((float)bytes / 1024.0, 1));
  else
    result = i18n("%1 MB").arg(KGlobal::locale()->formatNumber((float)bytes / 1024.0 / 1024.0, 1));
}
 
static void formatBytesMonth(double bytes, QString &result) {

  int day, days;
  day = QDate::currentDate().day();
  days = QDate::currentDate().daysInMonth();

  bytes = (bytes / day) * days;

  if(bytes < 1024)
    result.setNum(bytes);
  else if(bytes < 1024*1024)
    result = i18n("%1 KB").arg(KGlobal::locale()->formatNumber((float)bytes / 1024.0, 1));
  else
    result = i18n("%1 MB").arg(KGlobal::locale()->formatNumber((float)bytes / 1024.0 / 1024.0, 1));
}

static void formatDuration(int seconds, QString &result) {
  QString sec;
  sec.sprintf("%02d", seconds%60);
  if(seconds < 60)
    result = i18n("%1s").arg(sec);
  else if(seconds < 3600)
    result = i18n("%1m %2s").arg(seconds/60).arg(sec);
  else
    result = i18n("%1h %2m %3s")
        .arg(seconds/3600)
        .arg((seconds % 3600)/60)
        .arg(sec);
}
 
static void formatDurationMonth(int seconds, QString &result) {

	int day, days;
	day = QDate::currentDate().day();
	days = QDate::currentDate().daysInMonth();

	seconds = (seconds / day) * days;

	QString sec;
	sec.sprintf("%02d", seconds%60);
	if(seconds < 60)
		result = i18n("%1s").arg(sec);
	else if(seconds < 3600)
		result = i18n("%1m %2s").arg(seconds/60).arg(sec);
	else
		result = i18n("%1h %2m %3s")
			.arg(seconds/3600)
			.arg((seconds % 3600)/60)
			.arg(sec);
}

static void costsMonth(double costs, double &result) {

	int day, days;
	day = QDate::currentDate().day();
	days = QDate::currentDate().daysInMonth();

	result = (costs / day) * days;

}

class LogListItem : public QListViewItem {
public:
    LogListItem(LogInfo *l,
		QListView * parent,
		QString s1, QString s2,
		QString s3, QString s4,
		QString s5, QString s6,
		QString s7, QString s8)
	: QListViewItem(parent, s1, s2, s3, s4, s5, s6, s7, s8),
	  li(l)
	{
	}
  virtual void paintCell( QPainter *p, const QColorGroup & cg,
			  int column, int width, int alignment );

    virtual QString key(int, bool) const;

    LogInfo *li;
};

void LogListItem::paintCell( QPainter *p, const QColorGroup & cg,
			     int column, int width, int alignment )
{
  QListViewItem::paintCell(p, cg, column, width, alignment);

  // double line above sum
  //if(!li) {
   // p->drawLine(0, 0, width, 0);
    //p->drawLine(0, 2, width, 2);
  //}
}

QString LogListItem::key(int c, bool ascending) const
{
  if (!li)	// we want the sum to be always at the bottom
    return ascending ? "z" : " ";

  QString k;
  switch (c) {
  case 0:
    k = li->connectionName();
    break;
  case 1:
  case 2:
  case 3:
    k.sprintf("%012u", (uint)li->from_t());
    break;
  case 4:
    k.sprintf("%012d", li->duration());
    break;
  case 5:
    k.sprintf("%012.2f", li->sessionCosts());
    break;
  case 6:
    k.sprintf("%012f", li->bytesIn());
    break;
  case 7:
    k.sprintf("%012f", li->bytesOut());
    break;
  }
  return k;
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
  lv->addColumn(i18n("Duration"));
  lv->addColumn(i18n("Costs"));
  lv->addColumn(i18n("Bytes In"));
  lv->addColumn(i18n("Bytes Out"));
  lv->setColumnAlignment(1, AlignRight);
  lv->setColumnAlignment(2, AlignRight);
  lv->setColumnAlignment(3, AlignRight);
  lv->setColumnAlignment(4, AlignRight);
  lv->setColumnAlignment(5, AlignRight);
  lv->setColumnAlignment(6, AlignRight);
  lv->setColumnAlignment(7, AlignRight);
  lv->setAllColumnsShowFocus(true);
  lv->setShowSortIndicator(true);
  lv->setItemMargin(1);
  lv->setSorting(1);
  lv->setMinimumWidth(180);
  lv->setMinimumHeight(280);
  lv->setSelectionMode(QListView::Extended);
  selectionItem = 0L;
  connect(lv, SIGNAL(selectionChanged()), SLOT(slotSelectionChanged()));

  lv2 = new KListView(this);
  lv2->addColumn(i18n("Connection"));
  lv2->addColumn(i18n("Duration"));
  lv2->addColumn(i18n("Costs"));
  lv2->addColumn(i18n("Bytes In"));
  lv2->addColumn(i18n("Bytes Out"));
  lv2->setColumnAlignment(1, AlignRight);
  lv2->setColumnAlignment(2, AlignRight);
  lv2->setColumnAlignment(3, AlignRight);
  lv2->setColumnAlignment(4, AlignRight);
  lv2->setAllColumnsShowFocus(true);
  lv2->setSorting(-1);
  lv2->setItemMargin(2);
  lv2->setMaximumHeight(100);
  lv2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  lv2->setSelectionMode(QListView::NoSelection);

  title = new QLabel("X", this);
  QFont f = title->font();
  f.setPointSize(f.pointSize() + 2);
  f.setBold(TRUE);
  title->setFont(f);
  title->setFixedHeight(title->sizeHint().height()*2);

  cboConnections = new QComboBox(false, this);            // add a combo box to select connections
  cboConnections->setMaximumWidth(200);                   // a resonable size
  cboConnections->insertItem(i18n("All Connections"));    // default to all connections
  connect(cboConnections, SIGNAL(activated(int)),
	  this, SLOT(slotConnections(int)));

  bbox = new KButtonBox(this, Qt::Vertical);
  prev = bbox->addButton(i18n("&Prev Month"));
  next = bbox->addButton(i18n("&Next Month"));
  bbox->addStretch(1);
  today = bbox->addButton(i18n("C&urrent Month"));
  bbox->addStretch(1);
  exportBttn = bbox->addButton(i18n("&Export..."));

  connect(prev, SIGNAL(released()),
	  this, SLOT(prevMonth()));
  connect(next, SIGNAL(released()),
	  this, SLOT(nextMonth()));
  connect(today, SIGNAL(released()),
	  this, SLOT(currentMonth()));
  connect(exportBttn, SIGNAL(clicked()),
	  this, SLOT(exportWizard()));

  bbox->addStretch(8);
  bbox->layout();

  currentMonth();
  layoutWidget();
}

void MonthlyWidget::layoutWidget() {
  if(tl)
    delete tl;

  tl = new QGridLayout(this, 1, 1, 11, 16, "MainLayout");
  tl->addWidget(title, 0, 0);
  tl->addWidget(cboConnections, 0, 1);
  QLabel *l = new QLabel(this);
  l->setText(i18n("Statistics:"));
  QFont f2 = l->font();
  f2.setPointSize(f2.pointSize() + 1);
  f2.setBold(TRUE);
  l->setFont(f2);
  l->setFixedHeight(l->sizeHint().height());
  l->setAlignment( AlignLeft );
  tl->addWidget(l, 5, 0);
  tl->addWidget(bbox, 1, 2);
  tl->addMultiCellWidget(lv, 1, 4, 0, 1);
  tl->addMultiCellWidget(lv2, 6, 6, 0, 1);
      
  tl->activate();
}

int bestlen(QWidget *w, const char *s) {
  return w->fontMetrics().boundingRect(s).width() + 8;
}

void MonthlyWidget::plotMonth() {
  // name of the current connection
  QString con;

  // for collecting monthly statistics
  int count = 0;
  double costs = 0;
  double bin = 0, bout = 0;
  int duration = 0;
  lv->clear();
  selectionItem = 0L;
  lv2->clear();

  const KCalendarSystem * calendar = KGlobal::locale()->calendar();
  QDate startDate = periodeFirst();

  for(int i = 0; i < (int)logList.count(); i++) {
    LogInfo *li = logList.at(i);

    QDate logDate = li->from().date();
    if ( periodeFirst() <= logDate && periodeLast() >= logDate ) {
      // get connection name for this line
      con = li->connectionName();

      // this connection name not in the list and combo box
      if(lstConnections.findIndex(con) == -1) {
	lstConnections.append(con);
        cboConnections->insertItem(con);
      }
      // if all connections or the selected one
      if(cboConnections->currentText() != con &&
	cboConnections->currentItem() != 0)
        continue;
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

      QString _bin, _bout, b;
      if(li->bytesIn() >= 0)
        formatBytes(li->bytesIn(), _bin);
      else
        _bin = i18n("n/a");

      if(li->bytesOut() >= 0)
        formatBytes(li->bytesOut(), _bout);
      else
        _bout = i18n("n/a");

      if(li->bytes() > 0)
        formatBytes(li->bytes(), b);
      else
        b = i18n("n/a");

      QString day;
      day.sprintf("%2d", li->from().date().day());

      QString s_duration;
      formatDuration(li->from().secsTo(li->until()),
                     s_duration);

      QString s_lifrom, s_liuntil, s_costs;
      s_lifrom = KGlobal::locale()->formatTime(li->from().time(), false);
      s_liuntil = KGlobal::locale()->formatTime(li->until().time(), false);
      s_costs = KGlobal::locale()->formatMoney(li->sessionCosts());

      (void) new LogListItem(li, lv, con, day, s_lifrom, s_liuntil, s_duration, s_costs, _bin, _bout);
    }
  }

  if(count) {
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

    QString s_costs(KGlobal::locale()->formatMoney(costs, QString::null, 2));

    selectionItem =  new LogListItem(0, lv2,
			   i18n("Selection (%n connection)", "Selection (%n connections)", 0),
			   QString::null, QString::null, QString::null,
			   QString::null, QString::null, QString::null, QString::null);
    (void) new LogListItem(0, lv2,
			   i18n("%n connection", "%n connections", count),
			   s_duration, s_costs, _bin, _bout, QString::null, QString::null, QString::null);

	const KCalendarSystem * calendar = KGlobal::locale()->calendar();

	if(calendar->month(periodeFirst()) == calendar->month(QDate::currentDate())) {

	QString m_bin, m_bout;

    if(bin < 0)
      _bin = i18n("n/a");
    else
	formatBytesMonth(bin, m_bin);

    if(bout < 0)
      _bout = i18n("n/a");
    else
	formatBytesMonth(bout, m_bout);

    QString m_duration;
    formatDurationMonth(duration, m_duration);

	costsMonth(costs, costs);
    QString m_costs(KGlobal::locale()->formatMoney(costs, QString::null, 2));

    (void) new QListViewItem(lv2, selectionItem,
			   i18n("Monthly estimates"), m_duration, m_costs, m_bin, m_bout,
			   QString::null, QString::null, QString::null);
	}
  }

  QString t;
  if(lv->childCount() > 0) {
    exportBttn->setEnabled(true); // export possibility
    t = i18n("Connection log for %1 %2")
	      .arg(calendar->monthName(startDate))
	      .arg(calendar->year(startDate));
  } else {
    exportBttn->setEnabled(false); // nothing to export
    t = i18n("No connection log for %1 %2 available")
	      .arg(calendar->monthName(startDate))
	      .arg(calendar->year(startDate));
  }

  title->setText(t);
}

void MonthlyWidget::slotConnections(int) {
  plotMonth();
}

void MonthlyWidget::nextMonth() {
  m_periodeFirst = KGlobal::locale()->calendar()->addMonths(m_periodeFirst, 1);

  plotMonth();
}

void MonthlyWidget::prevMonth() {
  m_periodeFirst = KGlobal::locale()->calendar()->addMonths(m_periodeFirst, -1);

  plotMonth();
}

void MonthlyWidget::currentMonth() {
  const KCalendarSystem * calendar = KGlobal::locale()->calendar();
  QDate dt = QDate::currentDate();
  calendar->setYMD(m_periodeFirst, calendar->year(dt), calendar->month(dt), 1);

  plotMonth();
}

void MonthlyWidget::exportWizard() {
  const KCalendarSystem * calendar = KGlobal::locale()->calendar();
  QString date = QString::fromLatin1("%1-%2") // e.g.: June-2001
    .arg(calendar->monthName(periodeFirst()))
    .arg(calendar->year(periodeFirst()));

  ExportWizard *wizard = new ExportWizard(0, date);
  wizard->exec();
  if (wizard->filename.isEmpty()) { // wizard aborted...
    return;
  }
  if (QFile::exists(wizard->filename)) {  // overwrite?
    if (KMessageBox::Continue!=KMessageBox::warningContinueCancel(0, i18n("A document with this name already exists."), i18n("Overwrite file?"), i18n("&Overwrite") /*, true*/)) { // no
      return;
    }
  }

  // open file
  Export *exportIFace = wizard->createExportFilter();
  if (exportIFace == NULL) { // error
    return;
  }

  if (!exportIFace->openFile()) {  // error opening
    KMessageBox::sorry(0, i18n("An error occurred while trying to open this file"), i18n("Sorry"), true);
    delete exportIFace;
    return; // abort...
  }

  // start writing data
  exportIFace->addHeadline(i18n("Connection"), i18n("Day"), i18n("From"), i18n("Until"),
			   i18n("Duration"), i18n("Costs"), i18n("Bytes In"), i18n("Bytes Out") );

  // name of the current connection
  QString con;

  // for collecting monthly statistics
  int count = 0;
  double costs = 0;
  double bin = 0, bout = 0;
  int duration = 0;

  for(int i = 0; i < (int)logList.count(); i++) {
    LogInfo *li = logList.at(i);

    QDate logDate = li->from().date();
    if (periodeFirst() <= logDate && periodeLast() >= logDate ) {
      // get connection name for this line
      con = li->connectionName();

      // this connection name not in the list and combo box
      if(lstConnections.findIndex(con) == -1) {
          lstConnections.append(con);
        cboConnections->insertItem(con);
      }
      // if all connections or the selected one
      if(cboConnections->currentText() != con &&
         cboConnections->currentItem() != 0)
        continue;

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

      QString _bin, _bout, b;
      if(li->bytesIn() >= 0)
        formatBytes(li->bytesIn(), _bin);
      else
        _bin = i18n("n/a");

      if(li->bytesOut() >= 0)
        formatBytes(li->bytesOut(), _bout);
      else
        _bout = i18n("n/a");

      if(li->bytes() > 0)
        formatBytes(li->bytes(), b);
      else
        b = i18n("n/a");

      QString day;
      day.sprintf("%2d", li->from().date().day());
      QString con = li->connectionName();

      QString s_duration;
      formatDuration(li->from().secsTo(li->until()),
                     s_duration);

      QString s_lifrom, s_liuntil, s_costs;
      s_lifrom = KGlobal::locale()->formatTime(li->from().time(), false);
      s_liuntil = KGlobal::locale()->formatTime(li->until().time(), false);
      s_costs = KGlobal::locale()->formatMoney(li->sessionCosts());

      // call export method
      exportIFace->addDataline(con, day, s_lifrom, s_liuntil, s_duration,
			       s_costs, _bin, _bout);

    }
  }

	if(calendar->month(periodeFirst()) == calendar->month(QDate::currentDate())) {

	QString m_bin, m_bout;
	if(bin < 0)
		m_bin = i18n("n/a");
	else
		formatBytesMonth(bin, m_bin);

	if(bout < 0)
		m_bout = i18n("n/a");
	else
		formatBytesMonth(bout, m_bout);

	QString m_duration;
	formatDurationMonth(duration, m_duration);

	costsMonth(costs, costs);
	QString m_costs(KGlobal::locale()->formatMoney(costs, QString::null, 2));

	QString datetime = KGlobal::locale()->formatDateTime( QDateTime::currentDateTime(), true);

	exportIFace->addEmptyLine();
	exportIFace->addDataline(i18n("Monthly estimates (%1)").arg(datetime), 
		QString::null, QString::null, QString::null, m_duration, m_costs, m_bin, m_bout);
	}

  if(count) {
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

    QString s_costs(KGlobal::locale()->formatMoney(costs, QString::null, 2));

    // call export methods
    exportIFace->addEmptyLine();
    exportIFace->addDataline(i18n("%n connection", "%n connections", count), QString::null, QString::null, QString::null, s_duration,
			     s_costs, _bin, _bout);
    exportIFace->setFinishCode();

    // write buffer to file and close file
    if (!exportIFace->closeFile())  {
      KMessageBox::sorry(0, i18n("An error occurred while trying to write to this file."), i18n("Sorry"), true);
      delete exportIFace;
      return;
    }

  }
  delete exportIFace;
}

QDate MonthlyWidget::periodeFirst() const
{
  return m_periodeFirst;
}

QDate MonthlyWidget::periodeLast() const
{
  const KCalendarSystem * calendar = KGlobal::locale()->calendar();

  // One month minus one day
  return calendar->addDays(calendar->addMonths(m_periodeFirst, 1), -1);
}

void MonthlyWidget::slotSelectionChanged()
{
  if (selectionItem)
  {
    int count = 0;
    double costs = 0;
    double bin = 0, bout = 0;
    int duration = 0;
    LogListItem *item;
    LogInfo *li;
    QListViewItemIterator it(lv);
    while ( it.current() )
    {
      item = dynamic_cast<LogListItem*>(it.current());
      if ( item && item->isSelected() && item->li)
      {
        li = item->li;
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
        count++;
      }
      ++it;
    }
    if(count)
    {
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

      QString s_costs(KGlobal::locale()->formatMoney(costs, QString::null, 2));
      selectionItem->setText(0, i18n("Selection (%n connection)", "Selection (%n connections)", count));
      selectionItem->setText(1, s_duration);
      selectionItem->setText(2, s_costs);
      selectionItem->setText(3, _bin);
      selectionItem->setText(4, _bout);
    }
  }
}

#include "monthly.moc"
