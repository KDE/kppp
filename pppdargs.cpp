/*
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *
 *            Copyright (C) 1997 Bernd Johannes Wuebben
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <kguiitem.h>
#include <qlayout.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <kwindowsystem.h>
#include "pppdargs.h"
#include "pppdata.h"
#include <klocale.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <q3listbox.h>
#include <qlabel.h>
#include <KStandardGuiItem>
#include <kpushbutton.h>
#include <kiconloader.h>

PPPdArguments::PPPdArguments(QWidget *parent, const char *name)
  : KDialog(parent)
{
  setObjectName(name);

  setWindowTitle(i18n("Customize pppd Arguments"));
  QWidget *widget = new QWidget(this);
  setMainWidget(widget);
  KWindowSystem::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(KIconLoader::Desktop),IconSize(KIconLoader::Desktop)), qApp->windowIcon().pixmap(IconSize(KIconLoader::Small),IconSize(KIconLoader::Small)));
  QVBoxLayout *l = new QVBoxLayout(widget);
  l->setSpacing(10);
  l->setMargin(10);
  QHBoxLayout *tl = new QHBoxLayout();
  tl->setSpacing( 10 );
  l->addLayout(tl);
  QVBoxLayout *l1 = new QVBoxLayout();
  QVBoxLayout *l2 = new QVBoxLayout();
  tl->addLayout(l1, 1);
  tl->addLayout(l2, 0);

  QHBoxLayout *l11 = new QHBoxLayout();
  l11->setSpacing( 10 );
  l1->addLayout(l11);

  argument_label = new QLabel(i18n("Arg&ument:"), widget);
  l11->addWidget(argument_label);

  argument = new QLineEdit(widget);
  argument_label->setBuddy(argument);
  connect(argument, SIGNAL(returnPressed()),
	  SLOT(addbutton()));
  l11->addWidget(argument);
  connect(argument, SIGNAL(textChanged(QString)),
	  this, SLOT(textChanged(QString)));

  arguments = new Q3ListBox(widget);
  arguments->setMinimumSize(1, fontMetrics().lineSpacing()*10);
  connect(arguments, SIGNAL(highlighted(int)),
	  this, SLOT(itemSelected(int)));
  l1->addWidget(arguments, 1);

  add = new QPushButton(i18n("&Add"), widget);
  connect(add, SIGNAL(clicked()), SLOT(addbutton()));
  l2->addWidget(add);
  l2->addStretch(1);

  remove = new QPushButton(i18n("&Remove"), widget);
  connect(remove, SIGNAL(clicked()), SLOT(removebutton()));
  l2->addWidget(remove);

  defaults = new KPushButton(KStandardGuiItem::defaults(), widget);
  connect(defaults, SIGNAL(clicked()), SLOT(defaultsbutton()));
  l2->addWidget(defaults);

  l->addSpacing(5);

  setButtons(Ok | Cancel);
  connect(this, SIGNAL(okClicked()), SLOT(closebutton()));
 
  setFixedSize(sizeHint());

  //load info from gpppdata
  init();

  add->setEnabled(false);
  remove->setEnabled(false);
  argument->setFocus();
}


void PPPdArguments::addbutton() {
  if(!argument->text().isEmpty() && arguments->count() < MAX_PPPD_ARGUMENTS) {
    arguments->insertItem(argument->text());
    argument->setText("");
  }
}


void PPPdArguments::removebutton() {
  if(arguments->currentItem() >= 0)
    arguments->removeItem(arguments->currentItem());
}


void PPPdArguments::defaultsbutton() {
  // all of this is a hack
  // save current list
  QStringList arglist(gpppdata.pppdArgument());

  // get defaults
  gpppdata.setpppdArgumentDefaults();
  init();

  // restore old list
  gpppdata.setpppdArgument(arglist);
}


void PPPdArguments::closebutton() {
  QStringList arglist;
  for(uint i=0; i < arguments->count(); i++)
    arglist.append(arguments->text(i));
  gpppdata.setpppdArgument(arglist);

  done(0);
}


void PPPdArguments::init() {
  while(arguments->count())
    arguments->removeItem(0);

  QStringList &arglist = gpppdata.pppdArgument();
  for ( QStringList::Iterator it = arglist.begin();
        it != arglist.end();
        ++it )
    arguments->insertItem(*it);
}


void PPPdArguments::textChanged(const QString &s) {
  add->setEnabled(s.length() > 0);
}


void PPPdArguments::itemSelected(int idx) {
  remove->setEnabled(idx != -1);
}

#include "pppdargs.moc"
