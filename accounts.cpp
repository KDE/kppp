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
#include <qlayout.h>
#include <kmsgbox.h>
#include <kquickhelp.h>
#include <kwm.h>
#include <kbuttonbox.h>
#include "groupbox.h"
#include "pppdata.h"
#include "macros.h"
#include "accounts.h"
#include "accounting.h"

void parseargs(char* buf, char** args);

AccountWidget::AccountWidget( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  int min = 0;
  QGridLayout *tl = new QGridLayout(this, 3, 3, 10, 10);  
  tl->addRowSpacing(0, fontMetrics().lineSpacing() - 10); // magic
  box = new QGroupBox(this,"box");
  box->setTitle(i18n("Account Setup"));
  tl->addMultiCellWidget(box, 0, 2, 0, 2);

  // add a vbox in the middle of the grid
  QVBoxLayout *l1 = new QVBoxLayout;
  tl->addLayout(l1, 1, 1);

  // add a hbox
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);
    
  accountlist_l = new QListBox(this);
  accountlist_l->setMinimumSize(160, 128);
  connect(accountlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));
  connect(accountlist_l, SIGNAL(selected(int)),
	  this, SLOT(editaccount()));
  l11->addWidget(accountlist_l, 10);

  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111, 1);  
  edit_b = new QPushButton(i18n("Edit..."), this);
  connect(edit_b, SIGNAL(clicked()), SLOT(editaccount()));
  KQuickHelp::add(edit_b, i18n("Allows you to modify the selected account"));

  min = edit_b->sizeHint().width();
  min = QMAX(70,min);
  MIN_HEIGHT(edit_b);
  edit_b->setMinimumWidth(min);

  l111->addWidget(edit_b);

  new_b = new QPushButton(i18n("New..."), this);
  connect(new_b, SIGNAL(clicked()), SLOT(newaccount()));
  new_b->setMinimumSize(new_b->sizeHint());
  l111->addWidget(new_b);
  KQuickHelp::add(new_b, i18n("Create a new dialup connection\n"
  			      "to the internet"));

  copy_b = new QPushButton(i18n("Copy"), this);
  connect(copy_b, SIGNAL(clicked()), SLOT(copyaccount()));
  copy_b->setMinimumSize(copy_b->sizeHint());
  l111->addWidget(copy_b);
  KQuickHelp::add(copy_b, 
		  i18n("Makes a copy of the selected account. All\n"
		       "settings of the selected account are copied\n"
		       "to a new account, that you can modify to fit your\n"
		       "needs"));

  delete_b = new QPushButton(i18n("Delete"), this);
  connect(delete_b, SIGNAL(clicked()), SLOT(deleteaccount()));
  delete_b->setMinimumSize(delete_b->sizeHint());
  l111->addWidget(delete_b);
  KQuickHelp::add(delete_b, 
		  i18n("Deletes the selected account\n\n<red><b>Use with care!"));

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addStretch(1);
  l1->addLayout(l12);

  QVBoxLayout *l121 = new QVBoxLayout;
  l12->addLayout(l121);
  l121->addStretch(1);
  costlabel = new QLabel(i18n("Phone Costs:"), this);
  costlabel->setMinimumSize(costlabel->sizeHint());
  costlabel->setEnabled(FALSE);
  l121->addWidget(costlabel);

  costedit = new QLineEdit(this);
  costedit->setFocusPolicy(QWidget::NoFocus);
  costedit->setFixedHeight(costedit->sizeHint().height());
  costedit->setEnabled(FALSE);
  l121->addWidget(costedit);
  l121->addStretch(1);
  KQuickHelp::add(costlabel,
  KQuickHelp::add(costedit, 
		  i18n("This shows the accumulated phone costs\n"
		       "for the selected account.\n"
		       "\n"
		       "<b>Important</b>: If you have more than one \n"
		       "account - beware, this is <b>NOT</b> the sum \n"
		       "of the phone costs of all your accounts!")));

  vollabel = new QLabel(i18n("Volume:"), this);
  vollabel->setMinimumSize(vollabel->sizeHint());
  vollabel->setEnabled(FALSE);
  l121->addWidget(vollabel);

  voledit = new QLineEdit(this,"voledit");
  voledit->setFocusPolicy(QWidget::NoFocus);
  voledit->setFixedHeight(voledit->sizeHint().height());
  voledit->setEnabled(FALSE);
  l121->addWidget(voledit);
  KQuickHelp::add(vollabel,
  KQuickHelp::add(voledit, 
		  i18n("This shows the number of bytes transferred\n"
		       "for the selected account (not for all of your\n"
		       "accounts. You can select what to display in\n"
		       "the accounting dialog.\n"
		       "\n"
		       "<link #volaccounting>More on volume accounting</link>")));

  QVBoxLayout *l122 = new QVBoxLayout;
  l12->addStretch(1);
  l12->addLayout(l122);
  
  l122->addStretch(1);
  reset = new QPushButton(i18n("Reset..."), this);
  reset->setMinimumSize(reset->sizeHint());
  reset->setEnabled(FALSE);
  connect(reset,SIGNAL(clicked()),this,SLOT(resetClicked()));
  l122->addWidget(reset);

  log = new QPushButton(i18n("View Logs"), this);
  log->setMinimumSize(log->sizeHint());
  connect(log,SIGNAL(clicked()),this,SLOT(viewLogClicked()));
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

  tl->activate();
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
    voledit->setText(prettyPrintVolume(bytes).data());
    gpppdata.setAccount(account.data());
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
    if(fork() == 0) {
      setgid(getgid());
      setuid(getuid());
      system("kppplogview -kppp");
      exit(0);
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


void AccountWidget::editaccount(int) {
  editaccount();
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
      QMessageBox::information(this,i18n("Sorry"),
			       i18n("Maximum number of accounts reached."));
    return;
  }

  if (gpppdata.newaccount() == -1) 
    return;

  int result = doTab();

  if(result == QDialog::Accepted) {
    accountlist_l->insertItem(gpppdata.accname());
    emit resetaccounts();
    gpppdata.save();
  } else
    gpppdata.deleteAccount();  
}


void AccountWidget::copyaccount() {
  if(accountlist_l->count() == MAX_ACCOUNTS) {
    QMessageBox::information(this,i18n("Sorry"),
			     i18n("Maximum number of accounts reached."));
    return;
  }

  if(accountlist_l->currentItem()<0) {
    QMessageBox::information(this,i18n("Sorry"),
			     i18n("No account selected."));
    return;
  }

  gpppdata.copyaccount(accountlist_l->currentItem());

  accountlist_l->insertItem(gpppdata.accname());
  emit resetaccounts();
  gpppdata.save();
}


void AccountWidget::deleteaccount() {

  QString s;
  s.sprintf(i18n("Are you sure you want to delete\n" 
			       "the account \"%s\"?"),
	    accountlist_l->text(accountlist_l->currentItem()));

  if(KMsgBox::yesNo(this, 
		    i18n("Confirm"), 
		    s.data(),
		    KMsgBox::DB_SECOND) != 1)
    return;

  if(gpppdata.deleteAccount(accountlist_l->text(accountlist_l->currentItem())))
    accountlist_l->removeItem(accountlist_l->currentItem());

  emit resetaccounts();
  gpppdata.save();

  slotListBoxSelect(accountlist_l->currentItem());

}


int AccountWidget::doTab(){
  tabWindow = new QTabDialog(0,0,TRUE);
  KWM::setMiniIcon(tabWindow->winId(), kapp->getMiniIcon());
  bool isnewaccount;
 
  if(strcmp(gpppdata.accname(), "") == 0) {
    tabWindow->setCaption(i18n("New Account"));
    isnewaccount = true;
  } else {
    QString tit = i18n("Edit Account: ");
    tit += gpppdata.accname();
    tabWindow->setCaption(tit);
    isnewaccount = false;
  }

  tabWindow->resize( 360, 400 );
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
	     QMessageBox::warning(this, i18n("Error"), 
				     i18n( "You must enter an unique\naccount name"));
		ok = false;
	 }
      } else {
	      QMessageBox::warning(this, i18n("Error"), 
				   i18n("Login script has unbalanced loop Start/End") );
	      ok = false;
      }
    }
  }

 delete tabWindow;
 return result;
}


QString AccountWidget::prettyPrintVolume(unsigned int n) {
  int idx = 0;
  const char *quant[] = {i18n("Byte"), i18n("KB"), 
		   i18n("MB"), i18n("GB"), 0};

  float n1 = n;
  while(n >= 1024 && quant[idx] != 0) {
    idx++;
    n /= 1024;
  }

  int i = idx;
  while(i--)
    n1 = n1 / 1024.0;

  QString s;
  if(idx==0)
    s.sprintf("%0.0f %s", n1, quant[idx]);
  else
    s.sprintf("%0.1f %s", n1, quant[idx]);
  return s;
}


/////////////////////////////////////////////////////////////////////////////
//
// Queries the user what to reset: costs, volume or both
//
/////////////////////////////////////////////////////////////////////////////
QueryReset::QueryReset(QWidget *parent) : QDialog(parent, 0, true) {
  KWM::setMiniIcon(winId(), kapp->getMiniIcon());
  setCaption(i18n("Reset accounting"));

  QVBoxLayout *tl = new QVBoxLayout(this, 10, 10);
  KGroupBox *f = new KGroupBox(i18n("What to reset..."), this);  
  
  QVBoxLayout *l1 = new QVBoxLayout(f->peer(), 10, 10);
  costs = new QCheckBox(i18n("Reset the accumulated phone costs"), f->peer());
  costs->setMinimumSize(costs->sizeHint());
  costs->setChecked(true);
  l1->addWidget(costs);
  KQuickHelp::add(costs, i18n("Check this to set the phone costs\n"
			      "to zero. Typically you�ll want to\n"
			      "do this once a month."));

  volume = new QCheckBox(i18n("Reset volume accounting"), f->peer());
  volume->setMinimumSize(volume->sizeHint());
  volume->setChecked(true);
  l1->addWidget(volume);
  KQuickHelp::add(volume, i18n("Check this to set the volume accounting\n"
			       "to zero. Typically you�ll want to do this\n"
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
  tl->freeze();
}


void QueryReset::accepted() {
  int result = costs->isChecked() ? COSTS : 0;
  result += volume->isChecked() ? VOLUME : 0;

  done(result);
}

#include "accounts.moc"
