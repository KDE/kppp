/*
 *
 *            kPPP: A pppd front end for the KDE project
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


#ifndef PWENTRY_H
#define PWENTRY_H
 
#include <qwidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <q3groupbox.h>
//Added by qt3to4:
#include <QFocusEvent>
#include <QResizeEvent>

class PWEntry : public QWidget
{
	Q_OBJECT

public:

   explicit PWEntry( QWidget *parent=0,  const char *name=0 );   
   bool Consumed();
   void setConsumed();
   QString text();
   void setPrompt(const QString &);

public slots:

    void setEchoModeNormal();
    void setEchoModePassword();
    void hide();
    void show();

signals:

    void returnPressed();

protected:

    void resizeEvent(QResizeEvent* qre);
    void focusInEvent( QFocusEvent *);

private:

    Q3GroupBox *frame;
    QLineEdit *pw;
    QLabel *pl;
    bool isconsumed;

};
 
#endif
