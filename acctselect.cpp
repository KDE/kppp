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


#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qdir.h>
#include <qregexp.h>
#include <qwmatrix.h>
#include <stdio.h>

#include <kglobal.h>
#include <kstddirs.h>
#include <klocale.h>
#include "acctselect.h"
#include "pppdata.h"


AccountingSelector::AccountingSelector(QWidget *parent, bool _isnewaccount, const char *name)
  : KCheckGroupBox(i18n("Enable accounting"), parent, name),
    isnewaccount(_isnewaccount)
{
  QVBoxLayout *l1 = new QVBoxLayout(peer(), 10, 10);
  connect(this, SIGNAL(toggled(bool)), this, SLOT(enableItems(bool)));

  // insert the tree widget
  tl = new QListView(peer(), "treewidget");
  
  connect(tl, SIGNAL(selectionChanged(QListViewItem*)), this,
          SLOT(slotSelectionChanged(QListViewItem*)));
  tl->setMinimumSize(220, 200);
  l1->addWidget(tl, 1);
  

  // label to display the currently selected ruleset
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addSpacing(10);
  l1->addLayout(l11);
  QLabel *lsel = new QLabel(i18n("Selected:"), peer());
  selected = new QLabel(peer());
  selected->setFrameStyle(QFrame::Sunken | QFrame::WinPanel);
  selected->setLineWidth(1);
  selected->setFixedHeight(selected->sizeHint().height() + 16);
  l11->addWidget(lsel, 0);
  l11->addSpacing(10);
  l11->addWidget(selected, 1);

  // volume accounting
  l1->addStretch(1);
  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addLayout(l12);
  QLabel *usevol_l = new QLabel(i18n("Volume accounting:"), peer());
  use_vol = new QComboBox(peer());
  use_vol->insertItem(i18n("No accounting"), 0);
  use_vol->insertItem(i18n("Bytes in"), 1);
  use_vol->insertItem(i18n("Bytes out"), 2);
  use_vol->insertItem(i18n("Bytes in and out"), 3);
  use_vol->setCurrentItem(gpppdata.VolAcctEnabled());
  l12->addWidget(usevol_l);
  l12->addWidget(use_vol);

  // load the pmfolder pixmap from KDEdir
  QString fname = locate("appdata", "pics/folder.xpm"); 
  pmfolder.load(fname);

  // scale the pixmap
  if(pmfolder.width() > 0) {
    QWMatrix wm;
    wm.scale(16.0/pmfolder.width(), 16.0/pmfolder.width());
    pmfolder = pmfolder.xForm(wm);
  }

  // load the pmfolder pixmap from KDEdir
  fname = locate("appdata", "pics/phone.xpm");
  pmfile.load(fname);

  // scale the pixmap
  if(pmfile.width() > 0) {
    QWMatrix wm;
    wm.scale(16.0/pmfile.width(), 16.0/pmfile.width());
    pmfile = pmfile.xForm(wm);
  }
  
  setChecked(gpppdata.AcctEnabled());
  setChecked(true);
  
  setupTreeWidget();

  l1->activate();
}


QString AccountingSelector::fileNameToName(QString s) {

  s.replace(QRegExp("_"), " ");
  return s;

}


QString AccountingSelector::nameToFileName(QString s) {

  s.replace(QRegExp(" "), "_");
  return s;

}

QListViewItem *AccountingSelector::findByName(QString name)
{
  QListViewItem *ch = tl->firstChild();
  while(ch) {
    if(ch->text(0) == name)
      return ch;
    ch = ch->nextSibling();
  }
  return 0;
}


void AccountingSelector::insertDir(QDir d, QListViewItem *root) {

  QListViewItem* tli = 0;
    
  // sanity check
  if(!d.exists() || !d.isReadable())
    return;


  // set up filter
  d.setNameFilter("*.rst");
  d.setFilter(QDir::Files);
  d.setSorting(QDir::Name);

  // read the list of files
  const QFileInfoList *list = d.entryInfoList();
  QFileInfoListIterator it( *list );
  QFileInfo *fi;

  // traverse the list and insert into the widget
  while((fi = it.current())) {
    ++it;

    QString samename = fi->fileName();
    
    QListViewItem *i = findByName(samename);

    // skip this file if already in tree
    if(i)
      continue;

    // check if this is the file we should mark
    QString name = fileNameToName(fi->baseName());
    if(root)
      tli = new QListViewItem(root, name);
    else
      tli = new QListViewItem(tl, name);
    
    tli->setPixmap(0, pmfile);

    // check if this is the item we are searching for
    // (only in "Edit"-mode, not in "New"-mode
    if(!isnewaccount &&
       (edit_s == QString(fi->filePath()).right(edit_s.length()))) {
      edit_item = tli;
    }
  }

  // set up a filter for the directories
  d.setFilter(QDir::Dirs);
  d.setNameFilter("*");
  const QFileInfoList *dlist = d.entryInfoList();
  QFileInfoListIterator dit(*dlist);
  
  while((fi = dit.current())) {
    // skip "." and ".." directories
    if(fi->fileName().left(1) != ".") {
      // convert to human-readable name
      QString name = fileNameToName(fi->fileName());

      // if the tree already has an item with this name,
      // skip creation and use this one, otherwise
      // create a new entry
      QListViewItem *i = findByName(name);
      if(!i) {
        QListViewItem* item;
        
        if(root)
          item = new QListViewItem(root, name);
        else
          item = new QListViewItem(tl, name);

        item->setPixmap(0, pmfolder);
        
	insertDir(QDir(fi->filePath()), item);
      } else
	insertDir(QDir(fi->filePath()), i);
    }
    ++dit;
  }
}


void AccountingSelector::setupTreeWidget() {
  // search the item
  edit_item = 0;
  if(!isnewaccount) {
    edit_s = gpppdata.accountingFile();
  }
  else
    edit_s = "";

  tl->addColumn( i18n("Available rules") );
  tl->setColumnWidth(0, 205);
  tl->setRootIsDecorated(true);
  
  // look in ~/.kde/share/apps/kppp/Rules and $KDEDIR/share/apps/kppp/Rules
  QStringList dirs = KGlobal::dirs()->getResourceDirs("appdata");
  for (QStringList::ConstIterator it = dirs.begin(); 
       it != dirs.end(); it++) {
    insertDir(QDir((*it) + "Rules"), 0);
  }

  // when mode is "Edit", then hightlight the
  // appropriate item
  if(!isnewaccount) {
    tl->setSelected(edit_item, true);
    tl->setOpen(edit_item->parent(), true);
    tl->ensureItemVisible(edit_item);
  }
  
  enableItems(isChecked());
}


void AccountingSelector::enableItems(bool enabled) {

  tl->setEnabled(enabled);

  if(!enabled || (!tl->currentItem()))
    selected->setText(i18n("(none)"));
  else {
    QListViewItem* i = tl->currentItem();
    QString s;
    while(i) {
      s = "/" + i->text(0) + s;
      i = i->parent();
    }
    selected->setText(s.mid(1));
    
    s += ".rst";
    edit_s = nameToFileName(s);
  }
}


void AccountingSelector::slotSelectionChanged(QListViewItem* i) {
  
  if(!i || i->childCount())
    return;

  if(!isChecked())
    return;
  
  enableItems(true);
}


bool AccountingSelector::save() {

  if(isChecked()) {
    gpppdata.setAccountingFile(edit_s);
    gpppdata.setAcctEnabled(true);
  } else {
    gpppdata.setAccountingFile("");
    gpppdata.setAcctEnabled(false);
  }

  gpppdata.setVolAcctEnabled(use_vol->currentItem());

  return true;
}

#include "acctselect.moc"

