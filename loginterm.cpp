/*
 *              kPPP: A pppd Front End for the KDE project
 *
 * $Id$
 *
 *              Copyright (C) 1997-98 Bernd Johannes Wuebben
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <kstdguiitem.h>
#include <kguiitem.h>

#include "loginterm.h"
#include "main.h"
#include "modem.h"

#include <stdio.h>
#include <klocale.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QTextCursor>
extern KPPPWidget *p_kppp;

LoginMultiLineEdit::LoginMultiLineEdit(QWidget *parent)
  : QTextEdit(parent)
{
}


LoginMultiLineEdit::~LoginMultiLineEdit() {
  Modem::modem->stop();
}


void LoginMultiLineEdit::insertChar(unsigned char c) {
  insertPlainText(QString(c));
  p_kppp->debugwindow->addChar(c);
}


void LoginMultiLineEdit::myreturn() {
    textCursor().movePosition( QTextCursor::Start );
}


void LoginMultiLineEdit::mynewline() {
    textCursor().movePosition( QTextCursor::End );
    insertPlainText(QString("\n"));

#if 0
  Q3MultiLineEdit::end(FALSE);
  Q3MultiLineEdit::newLine();
#endif
    p_kppp->debugwindow->addChar('\n');
}


void LoginMultiLineEdit::keyPressEvent(QKeyEvent *k) {
  unsigned char c = (unsigned char) k->ascii();

  if ((int)c == 0) return;

  if((int)c == 13)
    Modem::modem->writeLine("");
  else
    Modem::modem->writeChar(c);
}


void LoginMultiLineEdit::readChar(unsigned char c) {

  if(((int)c != 13) && ((int)c != 10) && ((int)c != 8))
    insertChar(c);

  if((int)c == 8 || ( int )c == 127)
      textCursor().deleteChar ();
  else if((int)c == 10)
    mynewline();
  else if((int)c == 13)
    myreturn();
}


LoginTerm::LoginTerm (QWidget *parent, const char *name)
  : QDialog(parent)
{
  setObjectName(name);

  setWindowTitle(i18n("Login Terminal Window"));
  setMinimumSize(300, 200);
  setMaximumSize(600, 400);
  resize(400, 300);

  QVBoxLayout *tl = new QVBoxLayout(this);
  tl->setSpacing(2);
  QGridLayout *vgr = new QGridLayout();
  QGridLayout *hgr = new QGridLayout();
  hgr->setMargin( 30 );

  tl->addLayout(vgr);
  vgr->addLayout(hgr, 1, 0);
  vgr->setRowStretch(0, 1);
  vgr->addItem(new QSpacerItem(0, 40), 1, 0);

  text_window = new LoginMultiLineEdit(this);
  text_window->setObjectName("term");
  text_window->setFocus();
  vgr->addWidget(text_window, 0, 0);

  cancel_b = new KPushButton(KStdGuiItem::cancel(), this);
  cancel_b->setFixedHeight(25);
  connect(cancel_b, SIGNAL(clicked()), SLOT(cancelbutton()));

  continue_b = new KPushButton(KStdGuiItem::cont(), this);
  continue_b->setFixedHeight(25);
  connect(continue_b, SIGNAL(clicked()), SLOT(continuebutton()));

  int mwidth;
  if (cancel_b->sizeHint().width() > continue_b->sizeHint().width())
    mwidth = cancel_b->sizeHint().width();
  else
    mwidth = continue_b->sizeHint().width();

  cancel_b->setFixedWidth(mwidth + 20);
  continue_b->setFixedWidth(mwidth + 20);

  hgr->addWidget(cancel_b, 0, 0, Qt::AlignCenter);
  hgr->addWidget(continue_b, 0, 1, Qt::AlignCenter);

  cont = false;

  Modem::modem->notify(text_window, SLOT(readChar(unsigned char)));
}


void LoginTerm::cancelbutton () {
  hide();
}


void LoginTerm::continuebutton() {
  cont = true;
  hide();
}


bool LoginTerm::pressedContinue() {
  return cont;
}


#include "loginterm.moc"









