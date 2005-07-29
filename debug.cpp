/*
 *        kPPP: A pppd front end for the KDE project
 *
 * $Id$
 *            Copyright (C) 1997  Bernd Wuebben
 *                 wuebben@math.cornel.edu
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
 * Software Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "debug.h"
#include "main.h"
#include "pppdata.h"
#include <klocale.h>

#include <assert.h>
//Added by qt3to4:
#include <QHideEvent>
#include <QLabel>
#include <Q3Frame>
#include <QResizeEvent>

extern KPPPWidget *p_kppp;

myMultiEdit::myMultiEdit(QWidget *parent, const char *name)
  : Q3MultiLineEdit(parent, name)
{
 setReadOnly(true);
}

void myMultiEdit::newLine() {
  Q3MultiLineEdit::newLine();
}


DebugWidget::DebugWidget(QWidget *parent, const char *name)
  : QDialog(parent, name, FALSE)
{
  setCaption(i18n("Login Script Debug Window"));

  text_window = new myMultiEdit(this,"debugwindow");
  text_window->setGeometry(2,5,400, 300);
  //  text_window->setReadOnly(FALSE);

  statuslabel = new QLabel("", this, "statuslabel");

  statuslabel->setFrameStyle( Q3Frame::Panel | Q3Frame::Sunken );
  statuslabel->setAlignment( Qt::AlignLeft|Qt::AlignVCenter );
  statuslabel->setGeometry(2, 307, 400, 20);
  //statusPageLabel->setFont( KGlobalSettings::generalFont() );

  dismiss = new QPushButton(this);
  dismiss->setGeometry(330,340,70,30);
  dismiss->setText(i18n("&Close"));
  dismiss->setFocus();
  connect(dismiss, SIGNAL(clicked()), SLOT(hide()));


  /*  fline = new QFrame(this,"line");
  fline->setFrameStyle(QFrame::HLine |QFrame::Sunken);
  fline->setGeometry(2,332,398,5);*/
  adjustSize();
  setMinimumSize(width(),height());

}

void DebugWidget::hideEvent(QHideEvent *)
{
  assert(p_kppp);
  p_kppp->con->debug->setOn(false);
}

void DebugWidget::clear() {
  text_window->clear(); 
}


void DebugWidget::addChar(unsigned char c) {
  QString stuff;

  if(c == '\r' || c == '\n') {
    if(c == '\n')
      text_window->newLine();    
  } else
    text_window->insert(QString(c));
}


void DebugWidget::statusLabel(const QString &s) {
  statuslabel->setText(s);
}


/*
void DebugWidget::keyPressEvent(QKeyEvent *k) {
}

*/
void DebugWidget::resizeEvent(QResizeEvent *e){
  int w = width() ;
  int h = height();
  e = e;

  text_window->setGeometry(2,5,w - 2 ,h - 63);
  statuslabel->setGeometry(2, h - 56 , w -2 , 20);
  dismiss->setGeometry(w - 72 , h - 32, 70, 30);
  //  fline->setGeometry(2,h -70 ,w - 4,5);
}


void DebugWidget::enter() {
  text_window->append("\r\n");
}


void DebugWidget::toggleVisibility() {
  if(isVisible())
    hide();
  else
    show();

  bool showlog = isVisible();
  gpppdata.set_show_log_window(showlog);
}


#include "debug.moc"

