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


#include <qdir.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <kwm.h>
#include <qregexp.h>
#include <kapp.h>
#include <qlineedit.h>
#include "providerdb.h"
#include "newwidget.h"
#include "pppdata.h"
#include "log.h"


#define UNENCODED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"


ProviderDB::ProviderDB(QWidget *parent) : 
  KWizard(parent, 0, true),
  cfg(0)
{
  KWM::setMiniIcon(winId(), kapp->getMiniIcon());
  setCaption(i18n("Create new account..."));

  KWizardPage *p1 = new KWizardPage;
  page1 = new PDB_Intro(this);
  p1->w = page1;
  addPage(p1);
  p1->w->setFocusPolicy(StrongFocus);

  KWizardPage *p2 = new KWizardPage;
  page2 = new PDB_Country(this);
  p2->w = page2;
  addPage(p2);

  KWizardPage *p3 = new KWizardPage;
  page3 = new PDB_Provider(this);
  p3->w = page3;
  addPage(p3);

  KWizardPage *p4 = new KWizardPage;
  page4 = new PDB_UserInfo(this);
  p4->w = page4;
  addPage(p4);

  KWizardPage *p5 = new KWizardPage;
  page5 = new PDB_DialPrefix(this);
  p5->w = page5;
  addPage(p5);

  KWizardPage *p9 = new KWizardPage;
  page9 = new PDB_Finished(this);
  p9->w = page9;
  addPage(p9);

  connect(this, SIGNAL(selected(int)),
	  this, SLOT(pageSelected(int)));
  connect(this, SIGNAL(cancelclicked()),
	  this, SLOT(reject()));

  setCancelButton();
  setOkButton();
  setHelpButton();
  getOkButton()->setEnabled(false);
  connect(getOkButton(), SIGNAL(clicked()),
	  this, SLOT(accept()));
  connect(getCancelButton(), SIGNAL(clicked()),
	  this, SLOT(reject()));

  resize(minimumSize());
}


ProviderDB::~ProviderDB() {
  if(cfg)
    delete cfg;
}


void ProviderDB::pageSelected(int idx) {
  bool prev = true;
  bool next = true;
  int nextPage = idx;

  switch(idx+1) {
  case 1:
    break;
  case 2:
    next = page2->lb->currentItem() != -1;
    break;
  case 3:
    page3->setDir(page2->lb->text(page2->lb->currentItem()));
    next = page3->lb->currentItem() != -1;    
    break;
  case 4:
    loadProviderInfo();
    next = page4->username().length() > 0 &&
      page4->password().length() > 0;
    page4->activate();
    break;
  case 5:
    page5->activate();
    break;
  case 6:
    getOkButton()->setEnabled(true);
    prev = false;
    break;
  }

  getPreviousButton()->setEnabled(prev);
  getNextButton()->setEnabled(next);
  if(idx != nextPage)
    gotoPage(nextPage);
}


void ProviderDB::loadProviderInfo() {
  if(cfg)
    delete cfg;

  QRegExp re(" ");
  QString loc = page2->lb->text(page2->lb->currentItem());
  loc = loc.replace(re, "_");
  QString provider = page3->lb->text(page3->lb->currentItem());
  urlEncode(provider);
  QString fname = kapp->kde_datadir();
  fname += "/kppp/Provider/" + loc;
  fname += "/" + provider; 
  Debug("Providerfile=%s\n", fname.data());

  cfg = new KSimpleConfig(fname.data(), true);
}


void ProviderDB::accept() {
  QRegExp re_username("%USERNAME%");
  QRegExp re_password("%PASSWORD%");

  KEntryIterator *it = cfg->entryIterator("<Default>");
  while(it->current() != 0) {
    QString key = it->currentKey();
    QString value = it->current()->aValue;
    if(value.contains(re_username))
      value.replace(re_username, page4->username());

    if(value.contains(re_password))
      value.replace(re_password, page4->password());

    gpppdata.writeConfig(gpppdata.currentGroup().data(),
			 key.data(),
			 value.data());

    if(key == "Name")
      gpppdata.setAccname(value.data());
    ++(*it);
  }

  gpppdata.writeConfig(gpppdata.currentGroup(), "DialPrefix", page5->prefix());
  done(Accepted);
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Intro::PDB_Intro(QWidget *parent) : QWidget(parent) {
  QLabel *l = newLabel(i18n("You will be asked a few questions on informations\n"
			    "which are needed to establish an Internet connection\n"
			    "with your Internet Service Provider (ISP).\n\n"
			    "Make sure you have the registration form from your\n"
			    "ISP handy. If you have any problems, try the online\n"
			    "help first. If any information is missing, contact\n"
			    "your ISP."),
		       this);
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  tl->addWidget(l);
  tl->activate();
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Country::PDB_Country(QWidget *parent) : QWidget(parent) {
  QLabel *l = newLabel(i18n("Select the location where you plan to use this\n"
			    "account from the list below. If your country or\n"
			    "location is not listed, you have to create the\n"
			    "account with the normal, dialog based setup.\n\n"
			    "If you hit \"Cancel\", the dialog based setup\n"
			    "will start."),
		       this);
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  tl->addWidget(l);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1);
  l1->addStretch(1);

  lb = new QListBox(this);
  connect(lb, SIGNAL(highlighted(int)),
	  this, SLOT(selectionChanged(int)));
  lb->setMinimumSize(220, 100);
  l1->addWidget(lb, 2);
  l1->addStretch(1);

  // fill the listbox
  // set up filter
  QDir d(kapp->kde_datadir() + "/kppp/Provider/");
  d.setFilter(QDir::Dirs);
  d.setSorting(QDir::Name);

  // read the list of files
  const QFileInfoList *list = d.entryInfoList();
  if(list) {
    QFileInfoListIterator it( *list );
    QFileInfo *fi;

    // traverse the list and insert into the widget
    QRegExp re("_");
    while((fi = it.current()) != NULL) {
      QString fname = fi->fileName();
      if(fname.length() && fname[0] != '.') {
        fname = fname.replace(re, " ");
        lb->insertItem(fname);
      }
      ++it;
    }
  }

  tl->activate();
}

void PDB_Country::selectionChanged(int idx) {
  KWizard *w = (KWizard *)parent();
  w->getNextButton()->setEnabled(idx != -1);
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Provider::PDB_Provider(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = newLabel(i18n("Select your Internet Service Provider (ISP) from\n"
			    "the list below. If the ISP is not in this list,\n"
			    "you have to click on \"Cancel\" and create this\n"
			    "account using the normal, dialog-based setup.\n\n"
			    "Click on \"Next\" when you have finished your\n"
			    "selection."), this); 
  tl->addWidget(l);

  QHBoxLayout *l1 = new QHBoxLayout;
  tl->addLayout(l1);
  l1->addStretch(1);

  lb = new QListBox(this);
  connect(lb, SIGNAL(highlighted(int)),
	  this, SLOT(selectionChanged(int)));
  lb->setMinimumSize(220, 100);
  l1->addWidget(lb, 2);
  l1->addStretch(1);  
}

void PDB_Provider::selectionChanged(int idx) {
  KWizard *w = (KWizard *)parent();
  w->getNextButton()->setEnabled(idx != -1);
}


void PDB_Provider::setDir(const char *_dir) {
  if(dir != _dir) {
    lb->clear();
    
    // fill the listbox
    // set up filter
    dir = _dir;
    
    QString dir1 = kapp->kde_datadir();
    dir1 += "/kppp/Provider/";
    QRegExp re1(" ");
    dir = dir.replace(re1, "_");
    dir1 += dir;
    
    QDir d(dir1.data());
    d.setFilter(QDir::Files);
    d.setSorting(QDir::Name);
    
    // read the list of files
    const QFileInfoList *list = d.entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi;
    
    // traverse the list and insert into the widget
    QRegExp re("_");
    while((fi = it.current()) != NULL) {
      QString fname = fi->fileName();
      if(fname.length() && fname[0] != '.') {
	urlDecode(fname);
	lb->insertItem(fname);
      }
      ++it;
    }
    
    // TODO: Qt 1.x needs this if list is empty
    lb->update();
  }
}


const char *PDB_Provider::getDir() {
  return dir.data();
}



/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_UserInfo::PDB_UserInfo(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = newLabel(i18n("To log on to your ISP, kppp needs the username\n"
			    "and the password you got from your ISP. Type\n"
			    "in this information in the fields below.\n\n"
			    "Word case is important here."),
		       this);
  tl->addWidget(l);

  QGridLayout *l1 = new QGridLayout(2, 2);
  tl->addLayout(l1);
  l = newLabel(i18n("Username:"), this);
  l1->addWidget(l, 0, 0);
  l = newLabel(i18n("Password:"), this);
  l1->addWidget(l, 1, 0);
  _username = newLineEdit(24, this);
  connect(_username, SIGNAL(textChanged(const char *)),
	  this, SLOT(textChanged(const char *)));
  l1->addWidget(_username, 0, 1);
  _password = newLineEdit(24, this);
  _password->setEchoMode(QLineEdit::Password);
  connect(_password, SIGNAL(textChanged(const char *)),
	  this, SLOT(textChanged(const char *)));
  l1->addWidget(_password, 1, 1);
  tl->activate();
};


void PDB_UserInfo::textChanged(const char *) {
  KWizard *w = (KWizard*)parent();
  w->getNextButton()->setEnabled(strlen(_password->text()) > 0 &&
				 strlen(_username->text()) > 0);
}


QString PDB_UserInfo::username() {
  QString s = _username->text();
  return s;
}


QString PDB_UserInfo::password() {
  QString s = _password->text();
  return s;
}


void PDB_UserInfo::activate() {
  _username->setFocus();
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_DialPrefix::PDB_DialPrefix(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = newLabel(i18n("If you need a special dial prefix (e.g. if you\n"
			    "are using a telephone switch) you can specify\n"
			    "this here. This prefix is dialed just before the\n"
			    "phone number.\n\n"
			    "If you have a telephone switch, you probly need\n"
			    "to write \"0\" or \"0,\" here."),
		       this);
  tl->addWidget(l);

  QGridLayout *l1 = new QGridLayout(1, 2);
  tl->addLayout(l1);
  l = newLabel(i18n("Dial prefix:"), this);
  l1->addWidget(l, 0, 0);
  _prefix = newLineEdit(24, this);
  l1->addWidget(_prefix, 0, 1);
  tl->activate();
}


QString PDB_DialPrefix::prefix() {
  QString s = _prefix->text();

  return s;
}


void PDB_DialPrefix::activate() {
  _prefix->setFocus();
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Finished::PDB_Finished(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = newLabel(i18n("Finished!\n\n"
			    "A new account has been created. Hit \"OK\" to\n"
			    "go back to the setup dialog. If you want to\n"
			    "check the settings of the newly created account,\n"
			    "you can use \"Edit\" in the setup dialog."),
		       this);
  tl->addWidget(l);
  tl->addStretch(1);
};


void urlDecode(QString &s) {
  QString s1;
  int i = 0;

  while(s[i]) {
    if(s[i] == '%') {
      s1 += 100*(s[i+1]-'0') + 10*(s[i+2]-'0') + (s[i+3]-'0');
      i += 4;
    } else {
      s1 += s[i];
      i++;
    }
  }
  
  s = s1;
}


void urlEncode(QString &s) {
  QString s1;  
  
  for(uint i = 0; i < s.length(); i++) {
    if(strchr(UNENCODED_CHARS, s[i]))
      s1 += s[i];
    else {
      unsigned char ch=(unsigned char)s[i];
      s1 += "%";
      s1 += ch / 100 + '0';
      s1 += (ch / 10)%10 + '0';
      s1 += ch % 10 + '0';
    }
  }
  s = s1;
}


#include "providerdb.moc"

