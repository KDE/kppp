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

#include <qpainter.h>
#include <qcombobox.h>
#include <qfile.h>
#include <klocale.h>
#include <kglobal.h>
#include <klistview.h>
#include <kdebug.h>
#include <kmessagebox.h>

#include "monthly.h"
#include "export.h"
#include <qstringlist.h>

static void formatBytes(int bytes, QString &result) {
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
private:
    LogInfo *li;
};

void LogListItem::paintCell( QPainter *p, const QColorGroup & cg,
			     int column, int width, int alignment )
{
  QListViewItem::paintCell(p, cg, column, width, alignment);

  // double line above sum
  if(!li) {
    p->drawLine(0, 0, width, 0);
    p->drawLine(0, 2, width, 2);
  }
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
    k.sprintf("%012d", li->bytesIn());
    break;
  case 7:
    k.sprintf("%012d", li->bytesOut());
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

  cboConnections = new QComboBox(false, this);            // add a combo box to select connections
  cboConnections->setMaximumWidth(200);                   // a resonable size
  cboConnections->insertItem(i18n("All Connections"));    // default to all connections
  connect(cboConnections, SIGNAL(activated(int)),
	  this, SLOT(slotConnections(int)));

  bbox = new KButtonBox(this, Qt::Vertical);
  prev = bbox->addButton(i18n("Prev Month"));
  next = bbox->addButton(i18n("Next Month"));
  bbox->addStretch(1);
  today = bbox->addButton(i18n("Current Month"));
  bbox->addStretch(1);
  exportBttn = bbox->addButton(i18n("Export..."));

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
  tl = new QVBoxLayout(this, 10, 10);
  tl->addWidget(title);
  tl->addWidget(cboConnections, 1);	// add the combo box

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
  QStringList months;
  for (int i=1; i<13; i++)
      months.append(KGlobal::locale()->monthName(i, false/*short*/));

  // current showed month
  currMonth = months[_month-1];

  // name of the current connection
  QString con;

  // for collecting monthly statistics
  int count = 0;
  double costs = 0;
  int bin = 0, bout = 0;
  int duration = 0;
  lv->clear();

  for(int i = 0; i < (int)logList.count(); i++) {
    LogInfo *li = logList.at(i);

    if(li->from().date().month() == _month && li->from().date().year() == _year) {
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

    (void) new LogListItem(0, lv,
			   i18n("%n connection", "%n connections", count),
			   QString::null, QString::null, QString::null,
			   s_duration, s_costs, _bin, _bout);
  }

  QString t;
  if(lv->childCount() > 0) {
    exportBttn->setEnabled(true); // export possibility
    t = i18n("Connection log for %1 %2")
	      .arg(months[_month-1])
	      .arg(_year);
  } else {
    exportBttn->setEnabled(false); // nothing to export
    t = i18n("No connection log for %1 %2 available")
	      .arg(months[_month-1])
	      .arg(_year);
  }

  title->setText(t);
}

void MonthlyWidget::slotConnections(int) {
  plotMonth();
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

void MonthlyWidget::exportWizard() {
  QString date = QString(currMonth+"-%1").arg(_year);  // e.g.: June-2001

  ExportWizard *wizard = new ExportWizard(0, date);
  wizard->exec();
  if (wizard->filename.isEmpty()) { // wizard aborted...
    return;
  }
  if (QFile::exists(wizard->filename)) {  // overwrite?
    if (KMessageBox::Continue!=KMessageBox::warningContinueCancel(0, i18n("A document with this name already exists."), i18n("Overwrite file?"), i18n("Overwrite") /*, true*/)) { // no
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
  int bin = 0, bout = 0;
  int duration = 0;

  for(int i = 0; i < (int)logList.count(); i++) {
    LogInfo *li = logList.at(i);

    if(li->from().date().month() == _month && li->from().date().year() == _year) {
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
      KMessageBox::sorry(0, i18n("An error occurred while trying to write to this file"), i18n("Sorry"), true);
      delete exportIFace;
      return;
    }

  }
  delete exportIFace;
}



#include "monthly.moc"
