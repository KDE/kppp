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
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <kapplication.h>
#include "log.h"
#include "monthly.h"
#include "main.h"
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>
#include <kiconloader.h>

#define F_EXIT     101


static const char description[] =
	I18N_NOOP("KPPP log viewer");

static const char version[] = "v0.0.2";

static KCmdLineOptions option[] =
{
   { "kppp", I18N_NOOP("Run in KPPP mode"), 0 },
   KCmdLineLastOption
};


TopWidget::TopWidget() : KMainWindow(0, "") {
  // Check command line args for "-kppp"

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  bool kpppmode = args->isSet("kppp");
  args->clear();

  setCaption(i18n("KPPP Log Viewer"));

  w = new QWidget(this);

  QBoxLayout *l = new QVBoxLayout(w, 5);

  td = new QTabWidget(w, "");
  mw = new MonthlyWidget(td);
  td->addTab(mw, i18n("Monthly Log"));
  l->addWidget(td);

  // remove buttons
  if(!kpppmode) {
    // create menu
    mb = new KMenuBar(this);
    QPopupMenu *fm = new QPopupMenu;
    fm->insertItem(SmallIcon("exit"),KStdGuiItem::quit().text(), F_EXIT);
    mb->insertItem(i18n("&File"), fm);

    mb->setAccel(CTRL + Key_Q, F_EXIT);
    connect(mb, SIGNAL(activated(int)),
	    this, SLOT(menuCallback(int)));
  } else {
    mb = 0;
    QPushButton *but = new KPushButton(KStdGuiItem::close(),w);
    QHBoxLayout *lh = new QHBoxLayout(l);
    lh->addStretch(10);
    lh->addWidget(but);

    connect(but, SIGNAL(clicked()),
	    kapp, SLOT(quit()));
  }

  setMinimumSize(mw->sizeHint().width() + 15,
                 mw->sizeHint().height() + 120);
  setCentralWidget(w);
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
  KAboutData aboutData("kppplogview", I18N_NOOP("KPPP Log Viewer"),
    version, description, KAboutData::License_GPL,
    I18N_NOOP("(c) 1999-2002, The KPPP Developers"));
  aboutData.addAuthor("Bernd Wuebben",0, "wuebben@kde.org");
  aboutData.addAuthor("Mario Weilguni",0, "");
  aboutData.addAuthor("Harri Porten",0, "porten@kde.org");
  KCmdLineArgs::init(argc, argv, &aboutData);

  KCmdLineArgs::addCmdLineOptions( option );

  KApplication a;

  loadLogs();

  TopWidget *w = new TopWidget;
  w->show();
  a.setMainWidget(w);

  return a.exec();
}

#include "main.moc"
