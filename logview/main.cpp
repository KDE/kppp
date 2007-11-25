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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <kapplication.h>
#include <QMenu>
#include "log.h"
#include "monthly.h"
#include "main.h"
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
#include <kiconloader.h>
#include <kguiitem.h>

static const char description[] =
	I18N_NOOP("KPPP log viewer");

static const char version[] = "v0.0.2";


TopWidget::TopWidget() : KXmlGuiWindow(0) {
  // Check command line args for "-kppp"

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  bool kpppmode = args->isSet("kppp");
  args->clear();

  setCaption(i18n("KPPP Log Viewer"));

  w = new QWidget(this);
  QBoxLayout *l = new QVBoxLayout(w);
  l->setSpacing(5);

  td = new QTabWidget(w);
  mw = new MonthlyWidget();
  td->addTab(mw, i18n("Monthly Log"));

  // remove buttons
  if(!kpppmode) {
    // create menu
    mb = new KMenuBar;
    l->addWidget(mb);
    l->addWidget(td);
    QMenu *fm = new QMenu;
    QAction *action = fm->addAction(QIcon(SmallIcon("application-exit")), KStandardGuiItem::quit().text());
    action->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(action,SIGNAL(triggered()), this, SLOT(slotExit()));
    fm->setTitle(i18n("&File"));
    mb->addMenu(fm);
  } else {
    l->addWidget(td);
    mb = 0;
    QPushButton *but = new KPushButton(KStandardGuiItem::close(),w);
    QHBoxLayout *lh = new QHBoxLayout();
    l->addItem(lh);
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

void TopWidget::slotExit(){
    exit(0);
}

int main(int argc, char **argv) {
  KAboutData aboutData("kppplogview", 0, ki18n("KPPP Log Viewer"),
    version, ki18n(description), KAboutData::License_GPL,
    ki18n("(c) 1999-2002, The KPPP Developers"));
  aboutData.addAuthor(ki18n("Bernd Wuebben"),KLocalizedString(), "wuebben@kde.org");
  aboutData.addAuthor(ki18n("Mario Weilguni"));
  aboutData.addAuthor(ki18n("Harri Porten"),KLocalizedString(), "porten@kde.org");
  KCmdLineArgs::init(argc, argv, &aboutData);


  KCmdLineOptions option;

  option.add("kppp", ki18n("Run in KPPP mode"));

  KCmdLineArgs::addCmdLineOptions( option );

  KApplication a;

  loadLogs();

  TopWidget *w = new TopWidget;
  w->show();

  return a.exec();
}

#include "main.moc"
