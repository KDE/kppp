
/*
 *            kPPP: A front end for pppd for the KDE project
 *
 * $Id$
 * 
 *              Copyright (C) 1997 Bernd Johannes Wuebben 
 *                      wuebben@math.cornell.edu
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

#include "scriptedit.h"

ScriptEdit::ScriptEdit( QWidget *parent, const char *name )
  : QWidget(parent, name)
{
  setFixedSize(300, 30);

  st = new QComboBox(this, "st");
  st->setGeometry(0, 0, 110, 30);
  st->insertItem("Expect");
  st->insertItem("Send");
  st->insertItem("Pause (sec)");
  st->insertItem("Hangup");
  st->insertItem("Answer");
  st->insertItem("Timeout (sec)");
  connect(st, SIGNAL(activated(int)), SLOT(setType(int)));

  se = new QLineEdit(this, "se");
  se->setGeometry(120, 5, 140, 25);
  se->setMaxLength(50);
  connect(se, SIGNAL(returnPressed()), SLOT(seReturnPressed()));

  setType(0);
}


void ScriptEdit::seReturnPressed() {
  emit returnPressed();
}


const char *ScriptEdit::text() {
  return se->text();
}

void ScriptEdit::setText(const char *t) {
  se->setText(t);
}


int ScriptEdit::type() {
  return st->currentItem();
}

void ScriptEdit::setType(int i) {
  switch(i) {
  case Expect:
    se->setText("");
    se->setEnabled(TRUE);
    break;

  case Send:
    se->setText("");
    se->setEnabled(TRUE);
    break;

  case Pause:
    se->setText("");
    se->setEnabled(TRUE);
    break;

  case Hangup:
    se->setText("");
    se->setEnabled(FALSE);
    break;

  case Answer:
    se->setText("");
    se->setEnabled(FALSE);
    break;

  case Timeout:
    se->setText("");
    se->setEnabled(TRUE);
    break;

  default: break;
  }
}
