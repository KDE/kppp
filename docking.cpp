/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997 Bernd Johannes Wuebben
 *                      wuebben@math.cornell.edu
 *
 * This file was contributed by Harri Porten <porten@tu-harburg.de>
 *
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

#include <qtooltip.h>
#include <qdir.h>
#include <qpoint.h>
#include <kwm.h>
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kpopupmenu.h>

#include "docking.h"
#include "main.h"
#include "kpppconfig.h"
#include "pppstats.h"

extern KPPPWidget   *p_kppp;

// static member
DockWidget *DockWidget::dock_widget = 0;

DockWidget::DockWidget(QWidget *parent, const char *name, PPPStats *st)
  : KDockWindow(0L, name), stats(st) {

  // load pixmaps
  dock_none_pixmap = UserIcon("dock_none");
  dock_left_pixmap = UserIcon("dock_left");
  dock_right_pixmap = UserIcon("dock_right");
  dock_both_pixmap = UserIcon("dock_both");

  // popup menu for right mouse button
  popup_m = contextMenu();
  toggleID = popup_m->insertItem(i18n("Restore"),
				 this, SLOT(toggle_window_state()));
  popup_m->insertItem(i18n("Details"), parent, SLOT(showStats()));
  popup_m->insertSeparator();
  popup_m->insertItem(i18n("Disconnect"),
		      parent, SLOT(disconnect()));
  // connect to stats for little modem animation
  connect(stats, SIGNAL(statsChanged(int)), SLOT(paintIcon(int)));

  DockWidget::dock_widget = this;
}


DockWidget::~DockWidget() {
  DockWidget::dock_widget = 0;
}


void DockWidget::paintEvent (QPaintEvent *) {
  paintIcon(PPPStats::BytesNone);
}


void DockWidget::paintIcon (int status) {
  // animate modem lights

  const QPixmap *pixmap;

  if(this->isVisible()) {
    switch(status)
      {
      case PPPStats::BytesBoth:
        pixmap = &dock_both_pixmap;
        break;
      case PPPStats::BytesIn:
        pixmap = &dock_left_pixmap;
        break;
      case PPPStats::BytesOut:
        pixmap = &dock_right_pixmap;
        break;
      case PPPStats::BytesNone:
      default:
        pixmap = &dock_none_pixmap;
        break;
      }
      
    bitBlt(this, 0, 0, pixmap);
  }
}


void DockWidget::take_stats() {
  if (isVisible()) {
    stats->initStats();
    stats->start();
  }
}


void DockWidget::stop_stats() {
  stats->stop();
}


void DockWidget::mousePressEvent(QMouseEvent *e) {
  // open/close connect-window on right mouse button
  if ( e->button() == LeftButton ) {
    toggle_window_state();
  }

  // open popup menu on left mouse button
  if ( e->button() == RightButton ) {
    QString text;
    if(p_kppp->con_win->isVisible())
      text = i18n("Minimize");
    else
      text = i18n("Restore");

    popup_m->changeItem(text, toggleID);
    popup_m->popup(e->globalPos());
    popup_m->exec();
  }
}


void DockWidget::toggle_window_state() {
  // restore/hide connect-window
  if(p_kppp != 0L)  {
    if (p_kppp->con_win->isVisible())
      p_kppp->con_win->hide();
    else {
      p_kppp->con_win->show();
      KWM::activate(p_kppp->con_win->winId());
    }
  }
}

#include "docking.moc"
