/*
 *           kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qdir.h>
#include <stdlib.h>
#include <qlayout.h>
#include <qtabdialog.h>
#include <qwhatsthis.h>
#include <qmessagebox.h>

#include <kapplication.h>
#include <kbuttonbox.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kglobal.h>
#include "groupbox.h"
#include "pppdata.h"
#include "accounts.h"
#include "accounting.h"
#include "providerdb.h"
#include "edit.h"
#include <kwin.h>

void parseargs(char* buf, char** args);

AccountWidget::AccountWidget( QWidget *parent, const char *name )
  : KGroupBox( i18n("Account Setup"), parent, name )
{
  int min = 0;
  QVBoxLayout *l1 = new QVBoxLayout(peer(), 10, 10);

  // add a hbox
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);

  accountlist_l = new QListBox(peer());
  accountlist_l->setMinimumSize(160, 128);
  connect(accountlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));
  connect(accountlist_l, SIGNAL(selected(int)),
	  this, SLOT(editaccount()));
  l11->addWidget(accountlist_l, 10);

  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111, 1);
  edit_b = new QPushButton(i18n("Edit..."), peer());
  connect(edit_b, SIGNAL(clicked()), SLOT(editaccount()));
  QWhatsThis::add(edit_b, i18n("Allows you to modify the selected account"));

  min = edit_b->sizeHint().width();
  min = QMAX(70,min);
  edit_b->setMinimumWidth(min);

  l111->addWidget(edit_b);

  new_b = new QPushButton(i18n("New..."), peer());
  connect(new_b, SIGNAL(clicked()), SLOT(newaccount()));
  l111->addWidget(new_b);
  QWhatsThis::add(new_b, i18n("Create a new dialup connection\n"
  			      "to the internet"));

  copy_b = new QPushButton(i18n("Copy"), peer());
  connect(copy_b, SIGNAL(clicked()), SLOT(copyaccount()));
  l111->addWidget(copy_b);
  QWhatsThis::add(copy_b,
		  i18n("Makes a copy of the selected account. All\n"
		       "settings of the selected account are copied\n"
		       "to a new account, that you can modify to fit your\n"
		       "needs"));

  delete_b = new QPushButton(i18n("Delete"), peer());
  connect(delete_b, SIGNAL(clicked()), SLOT(deleteaccount()));
  l111->addWidget(delete_b);
  QWhatsThis::add(delete_b,
		  i18n("<p>Deletes the selected account\n\n"
		       "<font color=\"red\"><b>Use with care!</b></font>"));

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addStretch(1);
  l1->addLayout(l12);

  QVBoxLayout *l121 = new QVBoxLayout;
  l12->addLayout(l121);
  l121->addStretch(1);
  costlabel = new QLabel(i18n("Phone Costs:"), peer());
  costlabel->setEnabled(FALSE);
  l121->addWidget(costlabel);

  costedit = new QLineEdit(peer());
  costedit->setFocusPolicy(QWidget::NoFocus);
  costedit->setFixedHeight(costedit->sizeHint().height());
  costedit->setEnabled(FALSE);
  l121->addWidget(costedit);
  l121->addStretch(1);
  QString tmp = i18n("<p>This shows the accumulated phone costs\n"
		     "for the selected account.\n"
		     "\n"
		     "<b>Important</b>: If you have more than one \n"
		     "account - beware, this is <b>NOT</b> the sum \n"
		     "of the phone costs of all your accounts!");
  QWhatsThis::add(costlabel, tmp);
  QWhatsThis::add(costedit, tmp);

  vollabel = new QLabel(i18n("Volume:"), peer());
  vollabel->setEnabled(FALSE);
  l121->addWidget(vollabel);

  voledit = new QLineEdit(peer(),"voledit");
  voledit->setFocusPolicy(QWidget::NoFocus);
  voledit->setFixedHeight(voledit->sizeHint().height());
  voledit->setEnabled(FALSE);
  l121->addWidget(voledit);
  tmp = i18n("<p>This shows the number of bytes transferred\n"
	     "for the selected account (not for all of your\n"
	     "accounts. You can select what to display in\n"
	     "the accounting dialog.\n"
	     "\n"
	     "<a href=\"#volaccounting\">More on volume accounting</a>");

  QWhatsThis::add(vollabel,tmp);
  QWhatsThis::add(voledit, tmp);

  QVBoxLayout *l122 = new QVBoxLayout;
  l12->addStretch(1);
  l12->addLayout(l122);

  l122->addStretch(1);
  reset = new QPushButton(i18n("Reset..."), peer());
  reset->setEnabled(FALSE);
  connect(reset, SIGNAL(clicked()),
	  this, SLOT(resetClicked()));
  l122->addWidget(reset);

  log = new QPushButton(i18n("View Logs"), peer());
  connect(log, SIGNAL(clicked()),
	  this, SLOT(viewLogClicked()));
  l122->addWidget(log);
  l122->addStretch(1);

  //load up account list from gppdata to the list box
  if(gpppdata.count() > 0) {
    for(int i=0; i <= gpppdata.count()-1; i++) {
      gpppdata.setAccountbyIndex(i);
      accountlist_l->insertItem(gpppdata.accname());
    }
  }

  slotListBoxSelect(accountlist_l->currentItem());

  l1->activate();
}



void AccountWidget::slotListBoxSelect(int idx) {
  delete_b->setEnabled((bool)(idx != -1));
  edit_b->setEnabled((bool)(idx != -1));
  copy_b->setEnabled((bool)(idx != -1));
  if(idx!=-1) {
    QString account = gpppdata.accname();
    gpppdata.setAccountbyIndex(accountlist_l->currentItem());
    reset->setEnabled(TRUE);
    costlabel->setEnabled(TRUE);
    costedit->setEnabled(TRUE);
    costedit->setText(AccountingBase::getCosts(accountlist_l->text(accountlist_l->currentItem())));

    vollabel->setEnabled(TRUE);
    voledit->setEnabled(TRUE);
    int bytes = gpppdata.totalBytes();
    voledit->setText(prettyPrintVolume(bytes));
    gpppdata.setAccount(account);
 } else{
    reset->setEnabled(FALSE);
    costlabel->setEnabled(FALSE);
    costedit->setText("");
    costedit->setEnabled(FALSE);
    vollabel->setEnabled(FALSE);
    voledit->setText("");
    voledit->setEnabled(FALSE);
  }
}


void AccountWidget::viewLogClicked(){

    QApplication::flushX();
    if(fork() == 0) {
      setgid(getgid());
      setuid(getuid());
      system("kppplogview -kppp");
      _exit(0);
    }
}


void AccountWidget::resetClicked(){
  if(accountlist_l->currentItem() == -1)
    return;

  QueryReset dlg(this);
  int what = dlg.exec();

  if(what && QueryReset::COSTS) {
    emit resetCosts(accountlist_l->text(accountlist_l->currentItem()));
    costedit->setText("0");
  }

  if(what && QueryReset::VOLUME) {
    emit resetVolume(accountlist_l->text(accountlist_l->currentItem()));
    voledit->setText(prettyPrintVolume(0));
  }
}


void AccountWidget::editaccount() {
  gpppdata.setAccount(accountlist_l->text(accountlist_l->currentItem()));

  int result = doTab();

  if(result == QDialog::Accepted) {
    accountlist_l->changeItem(gpppdata.accname(),accountlist_l->currentItem());
    emit resetaccounts();
    gpppdata.save();
  }
}


void AccountWidget::newaccount() {
  if(accountlist_l->count() == MAX_ACCOUNTS) {
      KMessageBox::sorry(this, i18n("Maximum number of accounts reached."));
    return;
  }

  int result;
  int query = KMessageBox::questionYesNoCancel(this,
   i18n("Do you want to use the wizard to create the new account or the "
	"standard, dialog-based setup?\n"
	"The wizard is easier and sufficient in most cases. If you need "
	"very special settings, you might want to try the standard, "
		"dialog-based setup."), 
		i18n("Create a new account..."),
		i18n("Wizard"), i18n("Dialog setup"), i18n("Cancel"));

  switch(query) {
  case KMessageBox::Yes:
    {
      if (gpppdata.newaccount() == -1)
	return;
      ProviderDB pdb(this);
      result = pdb.exec();
      break;
    }
  case KMessageBox::No:
    if (gpppdata.newaccount() == -1)
      return;
    result = doTab();
    break;
  default:
    return;
  }

  if(result == QDialog::Accepted) {
    accountlist_l->insertItem(gpppdata.accname());
    accountlist_l->setSelected(accountlist_l->findItem(gpppdata.accname()),
			       true);
    emit resetaccounts();
    gpppdata.save();
  } else
    gpppdata.deleteAccount();
}


void AccountWidget::copyaccount() {
  if(accountlist_l->count() == MAX_ACCOUNTS) {
    KMessageBox::sorry(this, i18n("Maximum number of accounts reached."));
    return;
  }

  if(accountlist_l->currentItem()<0) {
    KMessageBox::sorry(this, i18n("No account selected."));
    return;
  }

  gpppdata.copyaccount(accountlist_l->currentItem());

  accountlist_l->insertItem(gpppdata.accname());
  emit resetaccounts();
  gpppdata.save();
}


void AccountWidget::deleteaccount() {

  QString s = i18n("Are you sure you want to delete\nthe account \"%1\"?")
    .arg(accountlist_l->text(accountlist_l->currentItem()));

  if(KMessageBox::warningYesNo(this, s, i18n("Confirm")) != KMessageBox::Yes)
    return;

  if(gpppdata.deleteAccount(accountlist_l->text(accountlist_l->currentItem())))
    accountlist_l->removeItem(accountlist_l->currentItem());

  emit resetaccounts();
  gpppdata.save();

  slotListBoxSelect(accountlist_l->currentItem());

}


int AccountWidget::doTab(){
  tabWindow = new QTabDialog(0,0,TRUE);
  KWin::setIcons(tabWindow->winId(), kapp->icon(), kapp->miniIcon());
  bool isnewaccount;

  if(gpppdata.accname().isEmpty()) {
    tabWindow->setCaption(i18n("New Account"));
    isnewaccount = true;
  } else {
    QString tit = i18n("Edit Account: ");
    tit += gpppdata.accname();
    tabWindow->setCaption(tit);
    isnewaccount = false;
  }

  //  tabWindow->resize( 360, 400 );
  tabWindow->setOKButton(i18n("OK"));
  tabWindow->setCancelButton(i18n("Cancel"));

  dial_w = new DialWidget(tabWindow, isnewaccount);
  ExecWidget *exec_w = new ExecWidget(tabWindow, isnewaccount);
  ip_w = new IPWidget(tabWindow, isnewaccount);
  dns_w = new DNSWidget(tabWindow, isnewaccount);
  gateway_w = new GatewayWidget(tabWindow, isnewaccount);
  script_w = new ScriptWidget(tabWindow, isnewaccount);
  acct = new AccountingSelector(tabWindow, isnewaccount);

  tabWindow->addTab(dial_w, i18n("Dial"));
  tabWindow->addTab(ip_w, i18n("IP"));
  tabWindow->addTab(gateway_w, i18n("Gateway"));
  tabWindow->addTab(dns_w, i18n("DNS"));
  tabWindow->addTab(script_w, i18n("Login Script"));
  tabWindow->addTab(exec_w, i18n("Execute"));
  tabWindow->addTab(acct, i18n("Accounting"));

  int result = 0;
  bool ok = false;
  while (!ok){

    result = tabWindow->exec();
    ok = true;

    if(result == QDialog::Accepted) {
      if (script_w->check()) {
          if(dial_w->save()) {
		ip_w->save();
		dns_w->save();
		gateway_w->save();
		script_w->save();
		exec_w->save();
		acct->save();
         } else {
	     KMessageBox::error(this, i18n( "You must enter an unique\n"
					    "account name"));
		ok = false;
	 }
      } else {
	      KMessageBox::error(this, i18n("Login script has unbalanced "
					    "loop Start/End"));
	      ok = false;
      }
    }
  }

 delete tabWindow;
 return result;
}


QString AccountWidget::prettyPrintVolume(unsigned int n) {
  int idx = 0;
  const QString quant[] = {i18n("Byte"), i18n("KB"),
		   i18n("MB"), i18n("GB"), QString::null};

  float n1 = n;
  while(n >= 1024 && quant[idx] != QString::null) {
    idx++;
    n /= 1024;
  }

  int i = idx;
  while(i--)
    n1 = n1 / 1024.0;

  QString s = KGlobal::locale()->formatNumber( n1, idx==0 ? 0 : 1 );
  s += " " + quant[idx];
  return s;
}


/////////////////////////////////////////////////////////////////////////////
//
// Queries the user what to reset: costs, volume or both
//
/////////////////////////////////////////////////////////////////////////////
QueryReset::QueryReset(QWidget *parent) : QDialog(parent, 0, true) {
  KWin::setIcons(winId(), kapp->icon(), kapp->miniIcon());
  setCaption(i18n("Reset Accounting"));

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  KGroupBox *f = new KGroupBox(i18n("What to reset..."), this);

  QVBoxLayout *l1 = new QVBoxLayout(f->peer(), 10, 10);
  costs = new QCheckBox(i18n("Reset the accumulated phone costs"), f->peer());
  costs->setChecked(true);
  l1->addWidget(costs);
  QWhatsThis::add(costs, i18n("Check this to set the phone costs\n"
			      "to zero. Typically you'll want to\n"
			      "do this once a month."));

  volume = new QCheckBox(i18n("Reset volume accounting"), f->peer());
  volume->setChecked(true);
  l1->addWidget(volume);
  QWhatsThis::add(volume, i18n("Check this to set the volume accounting\n"
			       "to zero. Typically you'll want to do this\n"
			       "once a month."));

  l1->activate();

  // this activates the f-layout and sets minimumSize()
  f->show();

  tl->addWidget(f);

  KButtonBox *bbox = new KButtonBox(this);
  bbox->addStretch(1);
  QPushButton *ok = bbox->addButton(i18n("OK"));
  ok->setDefault(true);
  QPushButton *cancel = bbox->addButton(i18n("Cancel"));

  connect(ok, SIGNAL(clicked()),
	  this, SLOT(accepted()));
  connect(cancel, SIGNAL(clicked()),
	  this, SLOT(reject()));

  bbox->layout();
  tl->addWidget(bbox);

  // TODO: activate if KGroupBox is fixed
  //  setFixedSize(sizeHint());
}


void QueryReset::accepted() {
  int result = costs->isChecked() ? COSTS : 0;
  result += volume->isChecked() ? VOLUME : 0;

  done(result);
}

#include "accounts.moc"
