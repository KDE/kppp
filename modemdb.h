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
//  Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//---------------------------------------------------------------------------

#ifndef __MODEMDB__H__
#define __MODEMDB__H__

#include <kdialog.h>
#include <qstringlist.h>
#include <q3dict.h>
#include <q3ptrlist.h>
class KConfig;
class Q3ListBox;

typedef Q3Dict<QString> CharDict;

class ModemDatabase {
public:
  ModemDatabase();
  ~ModemDatabase();

  const QStringList *vendors();
  QStringList *models(QString vendor);

  void save(KConfig *);

private:
  void load();
  void loadModem(const QString & key, CharDict &dict);
  Q3PtrList<CharDict> modems;

  QStringList *lvendors;

  KConfig *modemDB;
};


class ModemSelector : public KDialog {
  Q_OBJECT
public:
  ModemSelector(QWidget *parent = 0);
  ~ModemSelector();

private slots:
  void vendorSelected(int idx);
  void modelSelected(int idx);
  void selected(int idx);

private:
  QPushButton *ok, *cancel;
  Q3ListBox *vendor, *model;
  ModemDatabase *db;
};

#endif
