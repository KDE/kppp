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

#ifndef __MAIN__H__
#define __MAIN__H__

#include <kxmlguiwindow.h>
#include <qtabwidget.h>
#include <kmenubar.h>
#include "monthly.h"

class TopWidget : public KXmlGuiWindow {
  Q_OBJECT
public:
  TopWidget();
  ~TopWidget();

private slots:
  void slotExit();

private:
  MonthlyWidget *mw;
  KMenuBar *mb;
  QWidget *w;
  QTabWidget *td;
};

#endif
