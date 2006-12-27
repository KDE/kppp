/*
 *           kPPP: A pppd front end for the KDE project
 *
 *
 *
 *
 *            Copyright (C) 2004 Simone Gotti
 *               <simone.gotti@email.it>
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
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

#include <qdir.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <stdlib.h>
#include <qlayout.h>
#include <q3tabdialog.h>

#include <qmessagebox.h>

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kglobal.h>
#include <kwin.h>
#include <kpagedialog.h>
#include <kiconloader.h>

#include "general.h"
#include "pppdata.h"
#include "modems.h"
#include "accounting.h"
#include "providerdb.h"
#include "edit.h"

void parseargs(char* buf, char** args);

ModemsWidget::ModemsWidget( QWidget *parent, const char *name )
  : QWidget( parent )
{
  setObjectName(name);

  int min = 0;
  QVBoxLayout *l1 = new QVBoxLayout(parent);
  l1->setSpacing(10);
  l1->setMargin(10);

  // add a hbox
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);

  modemlist_l = new Q3ListBox(parent);
  modemlist_l->setMinimumSize(160, 128);
  connect(modemlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));
  connect(modemlist_l, SIGNAL(selected(int)),
	  this, SLOT(editmodem()));
  l11->addWidget(modemlist_l, 10);

  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111, 1);
  edit_b = new QPushButton(i18n("&Edit..."), parent);
  connect(edit_b, SIGNAL(clicked()), SLOT(editmodem()));
  edit_b->setWhatsThis( i18n("Allows you to modify the selected account"));

  min = edit_b->sizeHint().width();
  min = qMax(70,min);
  edit_b->setMinimumWidth(min);

  l111->addWidget(edit_b);

  new_b = new QPushButton(i18n("&New..."), parent);
  connect(new_b, SIGNAL(clicked()), SLOT(newmodem()));
  l111->addWidget(new_b);
  new_b->setWhatsThis( i18n("Create a new dialup connection\n"
  			      "to the Internet"));

  copy_b = new QPushButton(i18n("Co&py"), parent);
  connect(copy_b, SIGNAL(clicked()), SLOT(copymodem()));
  l111->addWidget(copy_b);
  copy_b->setWhatsThis(
		  i18n("Makes a copy of the selected account. All\n"
		       "settings of the selected account are copied\n"
		       "to a new account that you can modify to fit your\n"
		       "needs"));

  delete_b = new QPushButton(i18n("De&lete"), parent);
  connect(delete_b, SIGNAL(clicked()), SLOT(deletemodem()));
  l111->addWidget(delete_b);
  delete_b->setWhatsThis(
		  i18n("<p>Deletes the selected account\n\n"
		       "<font color=\"red\"><b>Use with care!</b></font>"));

  //load up account list from gppdata to the list box
  // but keep the current one selected in gpppdata
  if(gpppdata.modemCount() > 0) {
    const QString currentmodem = gpppdata.modname();
    for(int i=0; i <= gpppdata.modemCount()-1; i++) {
      gpppdata.setModemByIndex(i);
      modemlist_l->insertItem(gpppdata.modname());
    }
    gpppdata.setModem(currentmodem);
  }

  slotListBoxSelect(modemlist_l->currentItem());

  l1->activate();
}



void ModemsWidget::slotListBoxSelect(int idx) {
  delete_b->setEnabled((bool)(idx != -1));
  edit_b->setEnabled((bool)(idx != -1));
  copy_b->setEnabled((bool)(idx != -1));
  if(idx!=-1) {
    QString modem = gpppdata.modname();
    gpppdata.setModemByIndex(modemlist_l->currentItem());
    gpppdata.setModem(modem);
 }
}

void ModemsWidget::editmodem() {
  gpppdata.setModem(modemlist_l->text(modemlist_l->currentItem()));

  int result = doTab();

  if(result == QDialog::Accepted) {
    modemlist_l->changeItem(gpppdata.modname(),modemlist_l->currentItem());
    emit resetmodems();
    gpppdata.save();
  }
}


void ModemsWidget::newmodem() {
  if(modemlist_l->count() == MAX_MODEMS) {
      KMessageBox::sorry(this, i18n("Maximum number of modems reached."));
    return;
  }

  int result;

    if (gpppdata.newmodem() == -1)
      return;
    result = doTab();


  if(result == QDialog::Accepted) {
    modemlist_l->insertItem(gpppdata.modname());
    modemlist_l->setSelected(modemlist_l->findItem(gpppdata.modname()),
			       true);
    emit resetmodems();
    gpppdata.save();
  } else
    gpppdata.deleteModem();
}


void ModemsWidget::copymodem() {
  if(modemlist_l->count() == MAX_MODEMS) {
    KMessageBox::sorry(this, i18n("Maximum number of modems reached."));
    return;
  }

  if(modemlist_l->currentItem()<0) {
    KMessageBox::sorry(this, i18n("No modem selected."));
    return;
  }

  gpppdata.copymodem(modemlist_l->currentItem());

  modemlist_l->insertItem(gpppdata.modname());
  emit resetmodems();
  gpppdata.save();
}


void ModemsWidget::deletemodem() {

  QString s = i18n("Are you sure you want to delete\nthe modem \"%1\"?",
     modemlist_l->text(modemlist_l->currentItem()));

  if(KMessageBox::warningContinueCancel(this, s, i18n("Confirm"), KStandardGuiItem::del()) != KMessageBox::Continue)
    return;

  if(gpppdata.deleteModem(modemlist_l->text(modemlist_l->currentItem())))
    modemlist_l->removeItem(modemlist_l->currentItem());

  emit resetmodems();
  gpppdata.save();

  slotListBoxSelect(modemlist_l->currentItem());

}


int ModemsWidget::doTab(){
  tabWindow = new KPageDialog( this );
  tabWindow->setModal( true );
  tabWindow->setButtons( KDialog::Ok|KDialog::Cancel );
  tabWindow->setDefaultButton( KDialog::Ok );
  tabWindow->setFaceType( KPageDialog::Tabbed );
  KWin::setIcons(tabWindow->winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));
  bool isnewmodem;

  if(gpppdata.modname().isEmpty()) {
    tabWindow->setCaption(i18n("New Modem"));
    isnewmodem = true;
  } else {
    QString tit = i18n("Edit Modem: ");
    tit += gpppdata.modname();
    tabWindow->setCaption(tit);
    isnewmodem = false;
  }
  QFrame *frame = new QFrame();
  KPageWidgetItem *pageItem = new KPageWidgetItem( frame, i18n("&Device") );
  pageItem->setHeader( i18n("Serial Device") );
  tabWindow->addPage( pageItem );
  modem1 = new ModemWidget(frame, isnewmodem );

  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("&Modem") );
  pageItem->setHeader( i18n("Modem Settings") );
  tabWindow->addPage( pageItem );

  modem2 = new ModemWidget2(frame);
  connect ( modem1->connectName(), SIGNAL(textChanged ( const QString & )), this, SLOT(modemNameChanged(const QString & )));

  modemNameChanged(modem1->connectName()->text());
  int result = 0;
  bool ok = false;
  while (!ok){

    result = tabWindow->exec();
    ok = true;

    if(result == QDialog::Accepted) {
      if(modem1->save()) {
         modem2->save();
      } else {
        KMessageBox::error(this, i18n( "You must enter a unique\n"
			   "modem name"));
        ok = false;
      }
    }
  }

 delete tabWindow;
 return result;
}

void ModemsWidget::modemNameChanged(const QString & text)
{
  tabWindow->enableButtonOk( !text.isEmpty() );
}

QString ModemsWidget::prettyPrintVolume(unsigned int n) {
  int idx = 0;
  const QString quant[] = {i18n("Byte"), i18n("KB"),
		   i18n("MB"), i18n("GB"), QString::null};

  float n1 = n;
  while(n >= 1024 && !quant[idx].isNull()) {
    idx++;
    n /= 1024;
  }

  int i = idx;
  while(i--)
    n1 = n1 / 1024.0;

  QString s = KGlobal::locale()->formatNumber( n1, idx==0 ? 0 : 1 );
  s += ' ' + quant[idx];
  return s;
}

#include "modems.moc"
