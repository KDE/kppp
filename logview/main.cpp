/*
 * kPPPlogview: a accounting log system for kPPP
 *
 *            Copyright (C) 1998 Mario Weilguni <mweilguni@kde.org>
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

#include <stdio.h>
#include <kapp.h>
#include <qkeycode.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include "log.h"
#include "monthly.h"
#include "main.h"
#include <klocale.h>

#define F_EXIT     101

TopWidget::TopWidget() : KTMainWindow("") {
  // scan command line args for "-kppp"
  bool kpppmode = FALSE;
  for(int i = 1; i < kapp->argc(); i++) 
    if(strcmp(kapp->argv()[i], "-kppp") == 0)
      kpppmode = TRUE;

  setCaption(i18n("kPPP log viewer"));

  w = new QWidget(this);

  QBoxLayout *l = new QVBoxLayout(w, 5);

  td = new QTabWidget(w, "");
  mw = new MonthlyWidget(td);
  td->addTab(mw, i18n("Monthly log"));
  l->addWidget(td);

  // remove buttons
  if(!kpppmode) {
    // create menu 
    mb = new KMenuBar(this);
    QPopupMenu *fm = new QPopupMenu;
    fm->insertItem(i18n("E&xit"), F_EXIT);
    mb->insertItem(i18n("&File"), fm);
    setMenu(mb);
    
    mb->setAccel(CTRL + Key_X, F_EXIT);
    connect(mb, SIGNAL(activated(int)),
	    this, SLOT(menuCallback(int)));
  } else {
    mb = 0;
    QPushButton *but = new QPushButton(w);
    but->setText(i18n("Close"));
    QHBoxLayout *lh = new QHBoxLayout(l);
    lh->addStretch(10);
    lh->addWidget(but);

    connect(but, SIGNAL(clicked()),
	    kapp, SLOT(quit()));
  }

  setMinimumSize(mw->sizeHint().width() + 15, 
                 mw->sizeHint().height() + 120);
  setView(w);
}

TopWidget::~TopWidget() {
}

void TopWidget::menuCallback(int id) {
  switch(id) {
  case F_EXIT:
    exit(0);
    break;
  }
}

int main(int argc, char **argv) {
  KApplication a(argc, argv, "kppplogview");

  loadLogs();

  TopWidget *w = new TopWidget;
  w->show();
  a.setMainWidget(w);

  return a.exec();
}

#include "main.moc"
