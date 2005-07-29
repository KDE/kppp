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
//  Software Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
//
//---------------------------------------------------------------------------

#include <qlabel.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <kbuttonbox.h>
#include <qlayout.h>
#include "modemdb.h"
#include <klocale.h>
#include <qpushbutton.h>
#include <q3listbox.h>
#include <kconfig.h>
#include <kstdguiitem.h>

ModemSelector::ModemSelector(QWidget *parent) : QDialog(parent, 0, true) {
  // set up widgets and such
  setCaption(i18n("Select Modem Type"));
  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  QLabel *l1 = new QLabel(i18n("To set up your modem, first choose its vendor in the "
			       "list to the left, and then select the model from the "
			       "right list. If you don't know which modem you have, "
			       "you can try out one of the \"Generic\" modems."),
			this);
  l1->setAlignment(Qt::AlignLeft | Qt::WordBreak);
  l1->setFixedWidth(400);
  l1->setMinimumHeight(50);
  tl->addWidget(l1, 0);

  tl->addSpacing(10);

  QHBoxLayout *tl1 = new QHBoxLayout(10);
  tl->addLayout(tl1, 1);
  vendor = new Q3ListBox(this);
  model  = new Q3ListBox(this);
  vendor->setMinimumSize(200, 130);
  model->setMinimumSize(200, 130);
  tl1->addWidget(vendor, 2);
  tl1->addWidget(model, 3);

  KButtonBox *bbox = new KButtonBox(this);
  bbox->addStretch(1);
  ok = bbox->addButton(KStdGuiItem::ok());
  ok->setDefault(true);
  ok->setEnabled(false);
  cancel =   bbox->addButton(KStdGuiItem::cancel());
  bbox->layout();
  tl->addWidget(bbox);
  setFixedSize(sizeHint());

  // set up modem database
  db = new ModemDatabase();

  // set up signal/slots
  connect(ok, SIGNAL(clicked()),
	  this, SLOT(reject()));
  connect(cancel, SIGNAL(clicked()),
	  this, SLOT(reject()));
  connect(vendor, SIGNAL(highlighted(int)),
	  this, SLOT(vendorSelected(int)));
  connect(model, SIGNAL(highlighted(int)),
	  this, SLOT(modelSelected(int)));
  connect(model, SIGNAL(selected(int)),
	  this, SLOT(selected(int)));

  // fill vendor list with life
  vendor->insertStringList(*db->vendors());

  vendor->setCurrentItem(0);
}


ModemSelector::~ModemSelector() {
  delete db;
}


void ModemSelector::vendorSelected(int idx) {
  ok->setEnabled(false);

  QString name = vendor->text(idx);
  QStringList *models = db->models(name);
  model->clear();
  model->insertStringList(*models);

  // FIXME: work around Qt bug
  if(models->count() == 0)
    model->update();
  delete models;
}


void ModemSelector::modelSelected(int) {
  ok->setEnabled(true);
}

void ModemSelector::selected(int) {
  accept();
}


ModemDatabase::ModemDatabase() {
  load();
}


ModemDatabase::~ModemDatabase() {
  delete lvendors;
  delete modemDB;
}


const QStringList *ModemDatabase::vendors() {
  return lvendors;
}


QStringList *ModemDatabase::models(QString vendor)  {
  QStringList *sl = new QStringList;
  QString s = i18n("<Generic>");
  if(vendor == s)
    vendor = i18n("<Generic>");

  for(uint i = 0; i < modems.count(); i++) {
    CharDict *dict = modems.at(i);
    if(dict->find("Vendor") != 0) {
      if(vendor == *(*dict)["Vendor"] && (*(*dict)["Name"]).at(0) != '!')
	sl->append(*(*dict)["Name"]);
    }
  }
  sl->sort();

  return sl;
}


void ModemDatabase::loadModem(const QString &key, CharDict &dict) {
  //  KEntryIterator *it = modemDB->entryIterator(key);
  //  KEntryDictEntry *e;
  QMap <QString, QString> map;
  QMap <QString, QString>::Iterator it;
  //  KEntryMapConstIterator e;
  KEntry e;
  map = modemDB->entryMap(key);
  it = map.begin();

  // remove parent attribute
  dict.setAutoDelete(true);
  dict.remove("Parent");

  //  e = it->current();
  while(!it.key().isNull()) {
    if(dict.find(it.key()) == 0) {
      dict.insert(it.key(), new QString(it.data()));
    }
    it++;
  }

  // check name attribute
  if(dict["Name"] == 0 || key[0]=='!') {
    dict.replace("Name", new QString(key));
  }

  // check parent attribute
  if(dict["Parent"] != 0)
    loadModem(*dict["Parent"], dict);
  else
    // inherit common at last
    if (key != "Common")
      loadModem("Common", dict);

}


void ModemDatabase::load() {
  modemDB = new KConfig("DB/modemDB.rc", 0);
  lvendors = new QStringList;
  modems.setAutoDelete(true);

  QStringList list = modemDB->groupList();
  QStringList::Iterator it = list.begin();
  while(it != list.end()) {
    modemDB->setGroup(*it);
    CharDict *c = new CharDict;
    c->setAutoDelete(true);
    loadModem(*it, *c);

    //    if(strcmp(it->latin1(), "Common") == 0) {
    if(*it == "Common") {
      QString s = i18n("Hayes(tm) compatible modem");
      c->replace("Name", new QString (s));

      s = i18n("<Generic>");
      c->replace("Vendor", new QString(s));
    }
    modems.append(c);

    if(modemDB->hasKey("Vendor")) {
      QString vendor = modemDB->readEntry("Vendor");
      if(lvendors->findIndex(vendor) == -1)
	lvendors->append(vendor);
    }
    ++it;
  }

  lvendors->sort();

  lvendors->insert(0, i18n("<Generic>"));
}


void ModemDatabase::save(KConfig *) {
}

#include "modemdb.moc"
