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
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//---------------------------------------------------------------------------

#ifndef PROVIDER_DB
#define PROVIDER_DB

#include <kwizard.h>
#include <ksimpleconfig.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qlineedit.h>

class PDB_Intro : public QWidget {
  Q_OBJECT
public:
  PDB_Intro(QWidget *parent);
};


class PDB_Country : public QWidget {
  Q_OBJECT
public:
  PDB_Country(QWidget *parent);
  ~PDB_Country();
  QListBox *lb;
  QStringList *list;

public slots:
  void selectionChanged(int);
};


class PDB_Provider : public QWidget {
  Q_OBJECT
public:
  PDB_Provider(QWidget *parent);

  void setDir(const char *d);
  const char *getDir();

  QListBox *lb;

public slots:
  void selectionChanged(int);

private:
  QString dir;
};


class PDB_UserInfo : public QWidget {
  Q_OBJECT
public:
  PDB_UserInfo(QWidget *parent);
  QString username();
  QString password();
  void activate();

public slots:
  void textChanged(const QString &);

private:
  QLineEdit *_username;
  QLineEdit *_password;
};


class PDB_DialPrefix : public QWidget {
  Q_OBJECT
public:
  PDB_DialPrefix(QWidget *parent);
  QString prefix();
  void activate();

private:
  QLineEdit *_prefix;
};


class PDB_Finished : public QWidget {
  Q_OBJECT
public:
  PDB_Finished(QWidget *parent);
};


class ProviderDB : public KWizard {
  Q_OBJECT
public:
  ProviderDB(QWidget *parent);
  ~ProviderDB();
  static QWizard *wiz;

public slots:
  void pageSelected();
  void accept();

private:
  void loadProviderInfo();
  KSimpleConfig *cfg;

  PDB_Intro *page1;
  PDB_Country *page2;
  PDB_Provider *page3;
  PDB_UserInfo *page4;
  PDB_DialPrefix *page5;
  PDB_Finished *page9;
};


// Decodes a (some sort of)URL-encoded filename to a human-readable name.
// This is used for the provider database
void urlDecode(QString &);

// Encodes a (some sort of)URL-encoded filename from a human-readable name.
// This is used for the provider database
void urlEncode(QString &);


#endif
