//---------------------------------------------------------------------------
//
//             kPPP: A pppd front end for the KDE project
//
//---------------------------------------------------------------------------
//
// (c) 1997-1998 Bernd Johannes Wuebben <wuebben@kde.org>
// (c) 1997-1999 Mario Weilguni <mweilguni@kde.org>
// (c) 1998-1999 Harri Porten <porten@kde.org>
//
// derived from Jay Painters "ezppp"
//
//---------------------------------------------------------------------------
//  
//  $Id$
//
//---------------------------------------------------------------------------
// 
//  This program is free software; you can redistribute it and-or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
// 
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
// 
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the Free
//  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//---------------------------------------------------------------------------

#ifndef __ACCTSELECT__H__
#define __ACCTSELECT__H__

#include <qwidget.h>
#include <qpixmap.h>
#include <qdir.h>

class QCheckBox;
class QComboBox;
class QLabel;
class QListView;
class QListViewItem;

class AccountingSelector : public QWidget {
  Q_OBJECT
public:
  AccountingSelector(QWidget *parent = 0, bool _isnewaccount = false, const char *name = 0);
  ~AccountingSelector() {}

  bool save();

private:
  QListViewItem *findByName(QString name);
  void setupTreeWidget();
  void insertDir(QDir, QListViewItem * = 0);
  QString fileNameToName(QString);
  QString nameToFileName(QString);

private slots:
  void openURL(const QString &);
  void enableItems(bool);
  void slotSelectionChanged(QListViewItem* i);

private:
  QCheckBox *enable_accounting;
  QListView *tl;
  QComboBox *use_vol;
  QPixmap pmfolder, pmfile;
  QLabel *selected;

  QListViewItem *edit_item;
  QString edit_s;
  bool isnewaccount;
};

#endif

