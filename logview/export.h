/*
 * kPPPlogview: a accounting log system for kPPP
 *
 *            Copyright (C) 1998 Mario Weilguni <mweilguni@kde.org>
 *
 * This file has been contributed by Tilo Ulbrich <TiloUlbrich@web.de>
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

#ifndef KPPPEXPORT_H
#define KPPPEXPORT_H

#include <qwidget.h>
#include <qfile.h>
#include <qlayout.h>
#include <qlabel.h>
#include <q3listbox.h>
#include <qlineedit.h>


#include <k3wizard.h>
#include <klocale.h>
#include <kfiledialog.h>

class Export;

/***** ExportWizard *****/
class ExportWizard : public K3Wizard {
  Q_OBJECT
public:
   ExportWizard(QWidget *parent, const QString &_date);
   Export *createExportFilter();

   int filterID;
   QString filename;

public slots:
   void typeHighlighted(int);
   void getFilename();

   void accept();
    void reject();

private:
   QWidget  *formatPage;
   Q3ListBox *typeList;
   QLabel   *typeInfo;

   QWidget     *filenamePage;
   QLineEdit   *fnLine;
   QPushButton *fnGet;

   QString date;
};


/***** Export (abstract)*****/
class Export {
public:
   Export();
   Export(const QString &filename);
   virtual ~Export();
   bool openFile();
   bool closeFile();
   virtual void addHeadline(const QString &a, const QString &b,
			    const QString &c, const QString &d,
			    const QString &e, const QString &f,
			    const QString &g, const QString &h) = 0;
   virtual void addDataline(const QString &a, const QString &b,
			    const QString &c, const QString &d,
			    const QString &e, const QString &f,
			    const QString &g, const QString &h) = 0;
   virtual void addEmptyLine() = 0;
   virtual void setFinishCode() = 0;

protected:
   QFile   file;
   QString filename;
   QString buffer;
};


/***** CSVExport *****/
class CSVExport : public Export {
public:
  CSVExport(const QString &filename, const QString &separator);
  virtual void addHeadline(const QString &a, const QString &b,
			   const QString &c, const QString &d,
			   const QString &e, const QString &f,
			   const QString &g, const QString &h);
  virtual void addDataline(const QString &a, const QString &b,
			   const QString &c, const QString &d,
			   const QString &e, const QString &f,
			   const QString &g, const QString &h);
  virtual void addEmptyLine();
  virtual void setFinishCode();

private:
  QString separator;
};


/***** HTMLExport *****/
class HTMLExport : public Export {
public:
  HTMLExport(const QString &filename, const QString &date);
  virtual void addHeadline(const QString &a, const QString &b,
			   const QString &c, const QString &d,
			   const QString &e, const QString &f,
			   const QString &g, const QString &h);
  virtual void addDataline(const QString &a, const QString &b,
			   const QString &c, const QString &d,
			   const QString &e, const QString &f,
			   const QString &g, const QString &h);
  virtual void addEmptyLine();
  virtual void setFinishCode();

private:
  QString trStartCode;
  QString trEndCode;
  QString tdStartCode;
  QString tdEndCode;
};

#endif































































