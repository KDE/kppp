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


#include <qdir.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qregexp.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include "providerdb.h"
#include "newwidget.h"
#include "pppdata.h"
#include <qlistbox.h>
#include <qlineedit.h>
#include <ksimpleconfig.h>


#define UNENCODED_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"

QWizard* ProviderDB::wiz = 0L;

ProviderDB::ProviderDB(QWidget *parent) :
  KWizard(parent, "", true),
  cfg(0)
{
  setCaption(i18n("Create New Account"));

  wiz = this;

  page1 = new PDB_Intro(this);
  addPage(page1, "");
  setHelpEnabled(page1, false);
  // TODO  p1->w->setFocusPolicy(StrongFocus);

  page2 = new PDB_Country(this);
  addPage(page2, "");
  setHelpEnabled(page2, false);

  page3 = new PDB_Provider(this);
  addPage(page3, "");
  setHelpEnabled(page3, false);

  page4 = new PDB_UserInfo(this);
  addPage(page4, "");
  setHelpEnabled(page4, false);

  page5 = new PDB_DialPrefix(this);
  addPage(page5, "");
  setHelpEnabled(page5, false);

  page9 = new PDB_Finished(this);
  addPage(page9, "");
  setHelpEnabled(page9, false);
  setFinish(page9, true);
  setFinishEnabled(page9, true);

  connect((const QObject *)nextButton(), SIGNAL(clicked()),
  	  this, SLOT(pageSelected()));
  connect((const QObject *)backButton(), SIGNAL(clicked()),
  	  this, SLOT(pageSelected()));

  //  resize(minimumSize());
  adjustSize();
}


ProviderDB::~ProviderDB() {
  delete cfg;
}


void ProviderDB::pageSelected() {
  bool prev = true;
  bool next = true;

  QWidget *page = currentPage();
  if(page == page2) {
    next = page2->lb->currentItem() != -1;
  } else if(page == page3) {
    page3->setDir(*page2->list->at(page2->lb->currentItem()));
    next = page3->lb->currentItem() != -1;
  } else if(page == page4) {
    loadProviderInfo();
    next = !page4->username().isEmpty() &&
      !page4->password().isEmpty();
  }

  setBackEnabled(page, prev);
  setNextEnabled(page, next);
}


void ProviderDB::loadProviderInfo() {
  delete cfg;

  QString loc = *page2->list->at(page2->lb->currentItem());
  QString provider = page3->lb->text(page3->lb->currentItem());
  urlEncode(provider);
  QString prov = "Provider/" + loc;
  prov += "/" + provider;
  QString fname = locate("appdata", prov);
  kdDebug(5002) << "Providerfile=" << fname << endl;

  cfg = new KSimpleConfig(fname, true);
}


void ProviderDB::accept() {
  QRegExp re_username("%USERNAME%");
  QRegExp re_password("%PASSWORD%");

  QMap <QString, QString> map(cfg->entryMap("<default>"));
  QMap <QString, QString>::Iterator it(map.begin());
  while(it != map.end()) {
    QString key = it.key();
    QString value = *it;
    if(value.contains(re_username))
      value.replace(re_username, page4->username());

    if(value.contains(re_password))
      value.replace(re_password, page4->password());

    gpppdata.writeConfig(gpppdata.currentAccountGroup(), key, value);

    if(key == "Name")
      gpppdata.setAccname(value);
    ++it;
  }

  gpppdata.writeConfig(gpppdata.currentAccountGroup(), "DialPrefix", page5->prefix());
  done(Accepted);
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Intro::PDB_Intro(QWidget *parent) : QWidget(parent) {
  QLabel *l = new QLabel(i18n("You will be asked a few questions on information\n"
			    "which is needed to establish an Internet connection\n"
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
  QLabel *l = new QLabel(i18n("Select the location where you plan to use this\n"
			    "account from the list below. If your country or\n"
			    "location is not listed, you have to create the\n"
			    "account with the normal, dialog based setup.\n\n"
			    "If you click \"Cancel\", the dialog based setup\n"
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

  list = new QStringList;

  // fill the listbox
  // set up filter
  QDir d(KGlobal::dirs()->findDirs("appdata", "Provider").first());
  d.setFilter(QDir::Dirs);
  d.setSorting(QDir::Name);

  // read the list of files
  const QFileInfoList *flist = d.entryInfoList();
  if(flist) {
    QFileInfoListIterator it( *flist );
    QFileInfo *fi;
    // traverse the flist and insert into a map for sorting
    QMap<QString, QString> countries;
    for(; (fi = it.current()) != 0; ++it) {
      if(fi->fileName() != "." && fi->fileName() != "..") {
        QString dirFile(fi->absFilePath()+"/.directory");
        QString entryName;
        if(QFile::exists(dirFile)){
          KSimpleConfig config(dirFile);
          config.setDesktopGroup();
          entryName = config.readEntry("Name");
        }
        if (entryName.isNull()) entryName = fi->fileName();
	countries.insert(entryName, fi->fileName());
      }
    }
    // insert sorted entries into list box and string list
    QMap<QString, QString>::const_iterator mit = countries.begin();
    QMap<QString, QString>::const_iterator mend = countries.end();
    while(mit != mend) {
        lb->insertItem(mit.key());
        list->append(*mit);
	++mit;
    }
  }

  tl->activate();
}

PDB_Country::~PDB_Country()
{
  delete list;
}

void PDB_Country::selectionChanged(int idx) {
  //  QWizard *wizard = (QWizard *)parent(); Why doesn't this work ?
  ProviderDB::wiz->setNextEnabled(this, (idx != -1));
}


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_Provider::PDB_Provider(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = new QLabel(i18n("Select your Internet Service Provider (ISP) from\n"
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
  ProviderDB::wiz->setNextEnabled(this, idx != -1);
}


void PDB_Provider::setDir(const QString &_dir) {
  if(dir != _dir) {
    lb->clear();

    // fill the listbox
    // set up filter
    dir = _dir;

    QString dir1 = KGlobal::dirs()->findDirs("appdata", "Provider").first();
    QRegExp re1(" ");
    dir = dir.replace(re1, "_");
    dir1 += dir;

    QDir d(dir1);
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


QString PDB_Provider::getDir() {
  return dir;
}



/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
PDB_UserInfo::PDB_UserInfo(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l = new QLabel(i18n("To log on to your ISP, kppp needs the username\n"
			    "and the password you got from your ISP. Type\n"
			    "in this information in the fields below.\n\n"
			    "Word case is important here."),
		       this);
  tl->addWidget(l);

  QGridLayout *l1 = new QGridLayout(2, 2);
  tl->addLayout(l1);
  l = new QLabel(i18n("Username:"), this);
  l1->addWidget(l, 0, 0);
  l = new QLabel(i18n("Password:"), this);
  l1->addWidget(l, 1, 0);
  _username = newLineEdit(24, this);
  connect(_username, SIGNAL(textChanged(const QString &)),
	  this, SLOT(textChanged(const QString &)));
  l1->addWidget(_username, 0, 1);
  _password = newLineEdit(24, this);
  _password->setEchoMode(QLineEdit::Password);
  connect(_password, SIGNAL(textChanged(const QString &)),
	  this, SLOT(textChanged(const QString &)));
  l1->addWidget(_password, 1, 1);
  tl->activate();
}


void PDB_UserInfo::textChanged(const QString &) {
  ProviderDB::wiz->setNextEnabled(this, !_password->text().isEmpty() &&
                      !_username->text().isEmpty());
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
  QLabel *l = new QLabel(i18n("If you need a special dial prefix (e.g. if you\n"
			    "are using a telephone switch) you can specify\n"
			    "it here. This prefix is dialed just before the\n"
			    "phone number.\n\n"
			    "If you have a telephone switch, you probably need\n"
			    "to write \"0\" or \"0,\" here."),
		       this);
  tl->addWidget(l);

  QGridLayout *l1 = new QGridLayout(1, 2);
  tl->addLayout(l1);
  l = new QLabel(i18n("Dial prefix:"), this);
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
  QLabel *l = new QLabel(i18n("Finished!\n\n"
			    "A new account has been created. Click \"Finish\" to\n"
			    "go back to the setup dialog. If you want to\n"
			    "check the settings of the newly created account,\n"
			    "you can use \"Edit\" in the setup dialog."),
		       this);
  tl->addWidget(l);
  tl->addStretch(1);
}


void urlDecode(QString &s) {
  QString s1;

  for(uint i = 0; i < s.length(); i++) {
    if(s[i] == '%') {
      s1 += 100*s[i+1].digitValue() + 10*s[i+2].digitValue()
        + s[i+3].digitValue();
      i += 3;
    } else {
      s1 += s[i];
    }
  }

  s = s1;
}


void urlEncode(QString &s) {
  QString s1, tmp;

  for(uint i = 0; i < s.length(); i++) {
    if(QString(UNENCODED_CHARS).find(s[i]) >= 0)
      s1 += s[i];
    else {
      tmp.sprintf("%%%03i", s[i].unicode());
      s1 += tmp;
    }
  }
  s = s1;
}


#include "providerdb.moc"

