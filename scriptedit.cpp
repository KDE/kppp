
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "scriptedit.h"
#include <qlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <QHBoxLayout>

ScriptEdit::ScriptEdit( QWidget *parent )
  : QWidget(parent)
{
  QHBoxLayout *tl = new QHBoxLayout(this);
  tl->setSpacing(10);
  tl->setMargin(0);

  st = new QComboBox(this);
  st->setObjectName("st");
  st->addItem("Expect");
  st->addItem("Send");
  st->addItem("Pause (sec)");
  st->addItem("Hangup");
  st->addItem("Answer");
  st->addItem("Timeout (sec)");
  st->addItem("Password");
  st->addItem("ID");
  st->addItem("Prompt");
  st->addItem("PWPrompt");
  st->addItem("LoopStart");
  st->addItem("LoopEnd");
  st->addItem("Scan");
  st->addItem("Save");
  st->addItem("SendNoEcho");
  connect(st, SIGNAL(activated(int)), SLOT(setType(int)));

  se = new QLineEdit(this);
  se->setObjectName("se"); // oh yes, signor :-)
  se->setGeometry(120, 5, 140, 25);
  se->setMaxLength(50);
  connect(se, SIGNAL(returnPressed()), SLOT(seReturnPressed()));

  tl->addWidget(st, 3);
  tl->addWidget(se, 7);

  setType(0);

  tl->activate();
}


void ScriptEdit::setEnabled(bool b) {
  se->setEnabled(b);
  st->setEnabled(b);
}

void ScriptEdit::seReturnPressed() {
  emit returnPressed();
}


QString ScriptEdit::text()const {
  return se->text();
}

void ScriptEdit::setText(const QString &t) {
  se->setText(t);
}


int ScriptEdit::type()const {
  return st->currentIndex();
}

void ScriptEdit::setType(int i) {
  switch(i) {
  case Expect:
    se->setText("");
    se->setEnabled(true);
    break;

  case Send:
    se->setText("");
    se->setEnabled(true);
    break;

  case Pause:
    se->setText("");
    se->setEnabled(true);
    break;

  case Hangup:
    se->setText("");
    se->setEnabled(false);
    break;

  case Answer:
    se->setText("");
    se->setEnabled(false);
    break;

  case Timeout:
    se->setText("");
    se->setEnabled(true);
    break;

  case Password:
    se->setText("");
    se->setEnabled(true);
    break;

  case ID:
    se->setText("");
    se->setEnabled(true);
    break;

  case Prompt:
    se->setText("");
    se->setEnabled(true);
    break;

  case PWPrompt:
    se->setText("");
    se->setEnabled(true);
    break;

  case LoopStart:
    se->setText("");
    se->setEnabled(true);
    break;

  case LoopEnd:
    se->setText("");
    se->setEnabled(true);
    break;

  case Scan:
    se->setText("");
    se->setEnabled(true);
    break;

  case Save:
    se->setText("password");
    se->setEnabled(false);
    break;

  default: break;
  }
}

#include "scriptedit.moc"
