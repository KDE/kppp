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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <qlayout.h>
#include <kbuttonbox.h>
#include <kwm.h>
#include <kapp.h>
#include "pppdargs.h"
#include "pppdata.h"
#include <klocale.h>

PPPdArguments::PPPdArguments(QWidget *parent, const char *name)
  : QDialog(parent, name, TRUE)
{
  setCaption(i18n("Customize pppd Arguments"));  
  KWM::setMiniIcon(winId(), kapp->getMiniIcon());
  QVBoxLayout *l = new QVBoxLayout(this, 10, 10);
  QHBoxLayout *tl = new QHBoxLayout(10);
  l->addLayout(tl);
  QVBoxLayout *l1 = new QVBoxLayout();
  QVBoxLayout *l2 = new QVBoxLayout();
  tl->addLayout(l1, 1);
  tl->addLayout(l2, 0);

  QHBoxLayout *l11 = new QHBoxLayout(10);
  l1->addLayout(l11);

  argument_label = new QLabel(i18n("Argument:"), this);
  l11->addWidget(argument_label);

  argument = new QLineEdit(this);
  connect(argument, SIGNAL(returnPressed()), 
	  SLOT(addbutton()));
  l11->addWidget(argument);
  connect(argument, SIGNAL(textChanged(const QString &)),
	  this, SLOT(textChanged(const QString &)));

  arguments = new QListBox(this);
  arguments->setMinimumSize(1, fontMetrics().lineSpacing()*10);
  connect(arguments, SIGNAL(highlighted(int)),
	  this, SLOT(itemSelected(int)));
  l1->addWidget(arguments, 1);

  add = new QPushButton(i18n("Add"), this);
  connect(add, SIGNAL(clicked()), SLOT(addbutton()));
  l2->addWidget(add);
  l2->addStretch(1);  

  remove = new QPushButton(i18n("Remove"), this);
  connect(remove, SIGNAL(clicked()), SLOT(removebutton()));
  l2->addWidget(remove);

  defaults = new QPushButton(i18n("Defaults"), this);
  connect(defaults, SIGNAL(clicked()), SLOT(defaultsbutton()));
  l2->addWidget(defaults);
  
  l->addSpacing(5);

  KButtonBox *bbox = new KButtonBox(this);
  bbox->addStretch(1);
  closebtn = bbox->addButton(i18n("OK"));
  connect(closebtn, SIGNAL(clicked()), SLOT(closebutton()));
  QPushButton *cancel = bbox->addButton(i18n("Cancel"));
  connect(cancel, SIGNAL(clicked()),
	  this, SLOT(reject()));
  bbox->layout();
  l->addWidget(bbox);

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
  QStrList arglist(gpppdata.pppdArgument());

  // get defaults
  gpppdata.setpppdArgumentDefaults();
  init();

  // restore old list
  gpppdata.setpppdArgument(arglist);
}


void PPPdArguments::closebutton() {
  QStrList arglist;
  for(uint i=0; i < arguments->count(); i++)
    arglist.append(arguments->text(i));
  gpppdata.setpppdArgument(arglist);

  done(0);
}


void PPPdArguments::init() {
  while(arguments->count())
    arguments->removeItem(0);

  QStrList &arglist = gpppdata.pppdArgument();
  for(char *arg = arglist.first(); arg; arg = arglist.next())
    arguments->insertItem(arg);
}


void PPPdArguments::textChanged(const QString &s) {
  add->setEnabled(s.length() > 0);
}


void PPPdArguments::itemSelected(int idx) {
  remove->setEnabled(idx != -1);
}

#include "pppdargs.moc"
