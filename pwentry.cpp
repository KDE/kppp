/*
 *
 *            Kppp: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
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



#include <stdio.h>
#include <qapplication.h>
//Added by qt3to4:
#include <QFocusEvent>
#include <QResizeEvent>
#include <kglobalsettings.h>
#include "pwentry.h"

PWEntry::PWEntry( QWidget *parent, const char *name )
	: QWidget(NULL, name) {

   if(parent){

     QPoint point = mapToGlobal (QPoint (0,0));
     QRect pos = geometry();
     
     setGeometry(point.x() + pos.width()/2  - 300/2,
		 point.y() + pos.height()/2 - 90/2, 
		 300,
		 90);
   } else {
     QRect desk = KGlobalSettings::desktopGeometry(parent);
     setGeometry( desk.center().x() - 150, desk.center().y() - 50, 300, 90 );
   }

   frame = new Q3GroupBox(name, this );
   
   setFocusPolicy( Qt::StrongFocus );
   
   pw = new QLineEdit( this, "le" );
   pw->setEchoMode( QLineEdit::Password ); 
   connect( pw, SIGNAL(returnPressed()), this, SLOT(hide()) );

   isconsumed = TRUE;
}

QString PWEntry::text() { return (pw->text()); }

void PWEntry::focusInEvent( QFocusEvent *){

    pw->setFocus();

}

void PWEntry::setEchoModeNormal() { 

  pw->setEchoMode(QLineEdit::Normal); 

}

void PWEntry::setEchoModePassword() { 

  pw->setEchoMode(QLineEdit::Password); 

}

void PWEntry::setPrompt(const QString &p) {

    frame->setTitle(p);

}

void PWEntry::resizeEvent(QResizeEvent* ){

    pw->setGeometry( 15,35, width() - 30, 25 );
    frame->setGeometry(5,5, width() - 10, height() - 10 );

}


void PWEntry::show() {

    pw->setText("");
    isconsumed = FALSE;
    QWidget::show();
}

bool PWEntry::Consumed() {
    return(isconsumed);
}

void PWEntry::setConsumed() {
    isconsumed = TRUE;
}

void PWEntry::hide() {
  QWidget::hide();
  return;
}

#include "pwentry.moc"
