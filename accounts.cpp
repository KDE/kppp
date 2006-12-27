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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <qdir.h>
//Added by qt3to4:
#include <QLabel>
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
#include <KStandardGuiItem>
//#include <qvgroupbox.h>
#include <QBoxLayout>
#include <kiconloader.h>
#include "pppdata.h"
#include "accounts.h"
#include "accounting.h"
#include "providerdb.h"
#include "edit.h"
#include <errno.h>
void parseargs(char* buf, char** args);

AccountWidget::AccountWidget( QWidget *parent, const char *name )
  : QWidget( parent )
{
  setObjectName( name );
  int min = 0;
  QVBoxLayout *l1 = new QVBoxLayout(parent);
  l1->setSpacing(10);
  l1->setMargin(10);

  // add a hbox
  QHBoxLayout *l11 = new QHBoxLayout;
  l1->addLayout(l11);

  accountlist_l = new Q3ListBox(parent);
  accountlist_l->setMinimumSize(160, 128);
  connect(accountlist_l, SIGNAL(highlighted(int)),
	  this, SLOT(slotListBoxSelect(int)));
  connect(accountlist_l, SIGNAL(selected(int)),
	  this, SLOT(editaccount()));
  l11->addWidget(accountlist_l, 10);

  QVBoxLayout *l111 = new QVBoxLayout;
  l11->addLayout(l111, 1);
  edit_b = new QPushButton(i18n("&Edit..."), parent);
  connect(edit_b, SIGNAL(clicked()), SLOT(editaccount()));
  edit_b->setWhatsThis( i18n("Allows you to modify the selected account"));

  min = edit_b->sizeHint().width();
  min = qMax(70,min);
  edit_b->setMinimumWidth(min);

  l111->addWidget(edit_b);

  new_b = new QPushButton(i18n("&New..."), parent);
  connect(new_b, SIGNAL(clicked()), SLOT(newaccount()));
  l111->addWidget(new_b);
  new_b->setWhatsThis( i18n("Create a new dialup connection\n"
  			      "to the Internet"));

  copy_b = new QPushButton(i18n("Co&py"), parent);
  connect(copy_b, SIGNAL(clicked()), SLOT(copyaccount()));
  l111->addWidget(copy_b);
  copy_b->setWhatsThis(
		  i18n("Makes a copy of the selected account. All\n"
		       "settings of the selected account are copied\n"
		       "to a new account that you can modify to fit your\n"
		       "needs"));

  delete_b = new QPushButton(i18n("De&lete"), parent);
  connect(delete_b, SIGNAL(clicked()), SLOT(deleteaccount()));
  l111->addWidget(delete_b);
  delete_b->setWhatsThis(
		  i18n("<p>Deletes the selected account\n\n"
		       "<font color=\"red\"><b>Use with care!</b></font>"));

  QHBoxLayout *l12 = new QHBoxLayout;
  l1->addStretch(1);
  l1->addLayout(l12);

  QVBoxLayout *l121 = new QVBoxLayout;
  l12->addLayout(l121);
  l121->addStretch(1);
  costlabel = new QLabel(i18n("Phone costs:"), parent);
  costlabel->setEnabled(FALSE);
  l121->addWidget(costlabel);

  costedit = new QLineEdit(parent);
  costedit->setFocusPolicy(Qt::NoFocus);
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
  costlabel->setWhatsThis( tmp);
  costedit->setWhatsThis( tmp);

  vollabel = new QLabel(i18n("Volume:"), parent);
  vollabel->setEnabled(FALSE);
  l121->addWidget(vollabel);

  voledit = new QLineEdit(parent);
  voledit->setObjectName( "voledit" );
  voledit->setFocusPolicy(Qt::NoFocus);
  voledit->setFixedHeight(voledit->sizeHint().height());
  voledit->setEnabled(FALSE);
  l121->addWidget(voledit);
  tmp = i18n("<p>This shows the number of bytes transferred\n"
	     "for the selected account (not for all of your\n"
	     "accounts. You can select what to display in\n"
	     "the accounting dialog.\n"
	     "\n"
	     "<a href=\"#volaccounting\">More on volume accounting</a>");

  vollabel->setWhatsThis(tmp);
  voledit->setWhatsThis( tmp);

  QVBoxLayout *l122 = new QVBoxLayout;
  l12->addStretch(1);
  l12->addLayout(l122);

  l122->addStretch(1);
  reset = new QPushButton(i18n("&Reset..."), parent);
  reset->setEnabled(FALSE);
  connect(reset, SIGNAL(clicked()),
	  this, SLOT(resetClicked()));
  l122->addWidget(reset);

  log = new QPushButton(i18n("&View Logs"), parent);
  connect(log, SIGNAL(clicked()),
	  this, SLOT(viewLogClicked()));
  l122->addWidget(log);
  l122->addStretch(1);

  //load up account list from gppdata to the list box
  if(gpppdata.accountCount() > 0) {
    for(int i=0; i <= gpppdata.accountCount()-1; i++) {
      gpppdata.setAccountByIndex(i);
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
    gpppdata.setAccountByIndex(accountlist_l->currentItem());
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

    QApplication::flush();
    if(fork() == 0) {
      if (setgid(getgid()) < 0 && errno != EPERM)
        _exit(2);
      setuid(getuid());
      if( geteuid() != getuid() )
        _exit(1);
      // TODO: use execvp
      system("kppplogview -kppp");
      _exit(0);
    }
}


void AccountWidget::resetClicked(){
  if(accountlist_l->currentItem() == -1)
    return;

  QueryReset dlg(this);
  int what = dlg.exec();

  if((what & QueryReset::COSTS)) {
    emit resetCosts(accountlist_l->text(accountlist_l->currentItem()));
    costedit->setText("0");
  }

  if((what & QueryReset::VOLUME)) {
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
		i18n("Create New Account"),
		KGuiItem(i18n("&Wizard")), KGuiItem(i18n("&Manual Setup")));

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

  QString s = i18n("Are you sure you want to delete\nthe account \"%1\"?",
     accountlist_l->text(accountlist_l->currentItem()));

  if(KMessageBox::warningYesNo(this, s, i18n("Confirm"), KGuiItem(i18n("Delete"), "editdelete"), KStandardGuiItem::cancel()) != KMessageBox::Yes)
    return;

  if(gpppdata.deleteAccount(accountlist_l->text(accountlist_l->currentItem())))
    accountlist_l->removeItem(accountlist_l->currentItem());

  emit resetaccounts();
  gpppdata.save();

  slotListBoxSelect(accountlist_l->currentItem());

}


int AccountWidget::doTab(){
  tabWindow = new KPageDialog( this );
  tabWindow->setFaceType( KPageDialog::Tabbed );
  tabWindow->setButtons( KDialog::Ok|KDialog::Cancel );
  tabWindow->setDefaultButton( KDialog::Ok );
  tabWindow->setModal( true );
  KWin::setIcons(tabWindow->winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));
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

  QFrame * frame = new QFrame();
  KPageWidgetItem *pageItem = new KPageWidgetItem( frame, i18n("Dial") );
  pageItem->setHeader( i18n("Dial Setup") );
  tabWindow->addPage( pageItem );
  dial_w = new DialWidget(frame, isnewaccount);


  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("IP") );
  pageItem->setHeader( i18n("IP Setup") );
  tabWindow->addPage( pageItem );
  ip_w = new IPWidget(frame, isnewaccount);

  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("Gateway") );
  pageItem->setHeader( i18n("Gateway Setup") );
  tabWindow->addPage( pageItem );
  gateway_w = new GatewayWidget(frame, isnewaccount);

  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("DNS") );
  pageItem->setHeader( i18n("DNS Servers") );
  tabWindow->addPage( pageItem );
  dns_w = new DNSWidget(frame, isnewaccount);

  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("Login Script") );
  pageItem->setHeader( i18n("Edit Login Script") );
  tabWindow->addPage( pageItem );
  script_w = new ScriptWidget(frame, isnewaccount);


  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("Execute") );
  pageItem->setHeader( i18n("Execute Programs") );
  tabWindow->addPage( pageItem );
  ExecWidget *exec_w = new ExecWidget(frame, isnewaccount);

  frame = new QFrame();
  pageItem = new KPageWidgetItem( frame, i18n("Accounting") );
  tabWindow->addPage( pageItem );

  acct = new AccountingSelector(frame, isnewaccount);

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
             // ### add: "and valid"
	     KMessageBox::error(this, i18n( "You must enter a unique\n"
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


/////////////////////////////////////////////////////////////////////////////
//
// Queries the user what to reset: costs, volume or both
//
/////////////////////////////////////////////////////////////////////////////
QueryReset::QueryReset(QWidget *parent) : KDialog(parent) {
  KWin::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(K3Icon::Desktop),IconSize(K3Icon::Desktop)), qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));
  setWindowTitle(i18n("Reset Accounting"));

  QVBoxLayout *tl = new QVBoxLayout(this);
  tl->setSpacing(10);
  tl->setMargin(10);
  Q3GroupBox *f = new Q3GroupBox(1, Qt::Horizontal,i18n("What to Reset"), this);

  QVBoxLayout *l1 = new QVBoxLayout(parent);
  l1->setSpacing(10);
  l1->setMargin(10);
  costs = new QCheckBox(i18n("Reset the accumulated p&hone costs"), f);
  costs->setChecked(true);
  l1->addWidget(costs);
  costs->setWhatsThis( i18n("Check this to set the phone costs\n"
			      "to zero. Typically you will want to\n"
			      "do this once a month."));

  volume = new QCheckBox(i18n("Reset &volume accounting"), f);
  volume->setChecked(true);
  l1->addWidget(volume);
  volume->setWhatsThis( i18n("Check this to set the volume accounting\n"
			       "to zero. Typically you will want to do this\n"
			       "once a month."));

  l1->activate();

  // this activates the f-layout and sets minimumSize()
  f->show();

  tl->addWidget(f);

  setButtons(Ok|Cancel);

  // TODO: activate if KGroupBox is fixed
  //  setFixedSize(sizeHint());
}


void QueryReset::accepted() {
  int result = costs->isChecked() ? COSTS : 0;
  result += volume->isChecked() ? VOLUME : 0;

  done(result);
}

#include "accounts.moc"
