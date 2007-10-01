/*
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


#include <qlayout.h>
#include <qpainter.h>
#include <kguiitem.h>
//Added by qt3to4:
#include <QPixmap>
#include <QPaintEvent>
#include <QGridLayout>
#include <QCloseEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <kwindowsystem.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <q3frame.h>
#include <q3groupbox.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>

#include "pppdata.h"
#include "pppstatdlg.h"
#include "iplined.h"
#include "pppstats.h"

extern PPPData gpppdata;

PPPStatsDlg::PPPStatsDlg(QWidget *parent, const char *name, QWidget *,
			 PPPStats *st)
  : QWidget(parent),
    stats(st)
{
  int i;
  max = 1024;

  setObjectName(name);

  setWindowTitle(i18n("kppp Statistics"));
  KWindowSystem::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(KIconLoader::Desktop),IconSize(KIconLoader::Desktop)), qApp->windowIcon().pixmap(IconSize(KIconLoader::Small),IconSize(KIconLoader::Small)));

  QVBoxLayout *tl = new QVBoxLayout(this);
  tl->setSpacing(10);
  QGridLayout *l1 = new QGridLayout();
  tl->addLayout(l1, 1);
  box = new Q3GroupBox(i18n("Statistics"), this);
  l1->addMultiCellWidget(box, 0, 3, 0, 3);
  l1->addItem(new QSpacerItem(0, fontMetrics().lineSpacing() - 10), 0, 0);
  l1->setRowStretch(1, 1);
  l1->setColumnStretch(1, 1);

  // inner part of the grid
  QVBoxLayout *l11 = new QVBoxLayout;
  l1->addLayout(l11, 1, 1);

  // modem pixmap and IP labels
  QHBoxLayout *l111 = new QHBoxLayout;
  l11->addLayout(l111);

  big_modem_both_pixmap = UserIcon("modemboth");
  big_modem_left_pixmap = UserIcon("modemleft");
  big_modem_right_pixmap = UserIcon("modemright");
  big_modem_none_pixmap = UserIcon("modemnone");

  pixmap_l = new QLabel(this);
  pixmap_l->setMinimumSize(big_modem_both_pixmap.size());
  l111->addWidget(pixmap_l, 1);
  pixmap_l->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  QGridLayout *l1112 = new QGridLayout();
  l111->addLayout(l1112);

  ip_address_label1 = new QLabel(this);
  ip_address_label1->setText(i18n("Local Addr:"));

  ip_address_label2 = new IPLineEdit(this);
  ip_address_label2->setFocusPolicy(Qt::NoFocus);

  ip_address_label3 = new QLabel(this);
  ip_address_label3->setText(i18n("Remote Addr:"));

  ip_address_label4 = new IPLineEdit(this);
  ip_address_label4->setFocusPolicy(Qt::NoFocus);

  l1112->addWidget(ip_address_label1, 0, 0);
  l1112->addWidget(ip_address_label2, 0, 1);
  l1112->addWidget(ip_address_label3, 1, 0);
  l1112->addWidget(ip_address_label4, 1, 1);

  // consumes space on bottom
  l1112->setRowStretch(2, 1);

  QGridLayout *l112 = new QGridLayout();
  l11->addLayout(l112);
  for(i =0 ; i < 5; i++) {
    labela1[i] = new QLabel(this);

    labela2[i] = new QLabel(this);
    labela2[i]->setFrameStyle(Q3Frame::WinPanel | Q3Frame::Sunken);

    labelb1[i] = new QLabel(this);

    labelb2[i] = new QLabel(this);
    labelb2[i]->setFrameStyle(Q3Frame::WinPanel | Q3Frame::Sunken);
 }

  labela1[0]->setText(i18n("bytes in"));
  labelb1[0]->setText(i18n("bytes out"));

  labela1[1]->setText(i18n("packets in"));
  labelb1[1]->setText(i18n("packets out"));

  labela1[2]->setText(i18n("vjcomp in"));
  labelb1[2]->setText(i18n("vjcomp out"));

  labela1[3]->setText(i18n("vjunc in"));
  labelb1[3]->setText(i18n("vjunc out"));

  labela1[4]->setText(i18n("vjerr"));
  labelb1[4]->setText(i18n("non-vj"));

  for(i = 0; i < 5; i++) {
    labela2[i]->setText("888888888");	// TODO: resize automatically
    labelb2[i]->setText("888888888");
    labela2[i]->setAlignment(Qt::AlignRight);
    labelb2[i]->setAlignment(Qt::AlignRight);
    labela2[i]->setFixedSize(labela2[i]->sizeHint());
    labelb2[i]->setFixedSize(labelb2[i]->sizeHint());
    labela2[i]->setText("");
    labelb2[i]->setText("");

    // add to layout
    l112->addWidget(labela1[i], i, 0);
    l112->addWidget(labela2[i], i, 1);
    l112->addWidget(labelb1[i], i, 2);
    l112->addWidget(labelb2[i], i, 3);
  }

  l112->setColumnStretch(1, 1);
  l112->setColumnStretch(3, 1);

  tl->addSpacing(5);
  QHBoxLayout *l12 = new QHBoxLayout;
  tl->addLayout(l12);
  l12->addStretch(1);

  if(gpppdata.graphingEnabled()) {
    bool dummy;

    gpppdata.graphingOptions(dummy, bg, text, in, out);

    graph = new Q3Frame(this);
    graph->setFrameStyle(Q3Frame::Box | Q3Frame::Sunken);
    l1->addMultiCellWidget(graph, 2, 2, 1, 2);
    graph->setMinimumWidth(300);
    graph->setFixedHeight(76+4);
    graph->setBackgroundColor(bg);
  }

  cancelbutton = new KPushButton(KStandardGuiItem::close(),this);
  cancelbutton->setFocus();
  connect(cancelbutton, SIGNAL(clicked()), this,SLOT(cancel()));
  cancelbutton->setFixedHeight(cancelbutton->sizeHint().height());
  cancelbutton->setMinimumWidth(qMax(cancelbutton->sizeHint().width(), 70));
  l12->addWidget(cancelbutton);

  if(gpppdata.graphingEnabled()) {
    graphTimer = new QTimer(this);
    connect(graphTimer, SIGNAL(timeout()), SLOT(updateGraph()));
  }

  setFixedSize(sizeHint());

  connect(stats, SIGNAL(statsChanged(int)), SLOT(paintIcon(int)));

  // read window position from config file
  int p_x, p_y;
  gpppdata.winPosStatWin(p_x, p_y);
  move(p_x, p_y);
}


PPPStatsDlg::~PPPStatsDlg() {
}


// save window position when window was closed
bool PPPStatsDlg::event(QEvent *e) {
  if (e->type() == QEvent::Hide)
  {
    gpppdata.setWinPosStatWin(x(), y());
    return true;
  }
  else
    return QWidget::event(e);
}

void PPPStatsDlg::cancel() {
  hide();
}


void PPPStatsDlg::take_stats() {
  stats->initStats();
  bin_last = stats->ibytes;
  bout_last = stats->obytes;
  ringIdx = 0;
  for(int i = 0; i < MAX_GRAPH_WIDTH; i++) {
    bin[i] = -1;
    bout[i] = -1;
  }

  update_data();

  stats->start();
  if(gpppdata.graphingEnabled())
    graphTimer->start(GRAPH_UPDATE_TIME);
}


void PPPStatsDlg::stop_stats() {
  stats->stop();
  if(gpppdata.graphingEnabled())
    graphTimer->stop();
}

void PPPStatsDlg::paintGraph() {
  // why draw that stuff if not visible?
  if(!isVisible())
    return;

  QPixmap pm(graph->width() - 4, graph->height() - 4);
  QPainter p;
  pm.fill(graph->backgroundColor());
  p.begin(&pm);

  int x;
  int idx = ringIdx - pm.width() + 1;
  if(idx < 0)
    idx += MAX_GRAPH_WIDTH;

  // find good scaling factor
  int last_h_in =
    pm.height() - (int)((float)bin[idx]/max * (pm.height() - 8))-1;
  int last_h_out =
    pm.height() - (int)((float)bout[idx]/max * (pm.height() - 8))-1;

  // plot data
  int last_idx = 0;
  for(x = 1; x < pm.width(); x++) {
    int h_in, h_out;

    h_in = pm.height() - (int)((float)bin[idx]/max * (pm.height() - 8))-1;
    h_out = pm.height() - (int)((float)bout[idx]/max * (pm.height() - 8))-1;

    p.setPen(out);
    if(bout[idx]!=-1)
      p.drawLine(x-1, last_h_out, x, h_out);
    p.setPen(in);
    if(bin[idx]!=-1)
      p.drawLine(x-1, last_h_in, x, h_in);
    last_h_in = h_in;
    last_h_out = h_out;

    last_idx = idx;
    idx = (idx + 1) % MAX_GRAPH_WIDTH;
  }

  // take last value
  int last_max = bin[last_idx]>bout[last_idx] ? bin[last_idx] : bout[last_idx];

  // plot scale line
  p.setPen(text);
  p.setFont(QFont(KGlobalSettings::fixedFont().family(), 8));

  QRect r;
  QString s = i18n("%1 (max. %2) kb/sec",
		 KGlobal::locale()->formatNumber((float)last_max / 1024.0, 1),
		 KGlobal::locale()->formatNumber((float)max / 1024.0, 1));
  p.drawText(0, 0, pm.width(), 2*8, Qt::AlignRight|Qt::AlignVCenter, s, -1, &r);
  p.drawLine(0, 8, r.left() - 8, 8);

  p.end();
  bitBlt(graph, 2, 2, &pm, 0, 0, pm.width(), pm.height());
}

void PPPStatsDlg::updateGraph() {
  bin[ringIdx] = stats->ibytes - bin_last;
  bout[ringIdx] = stats->obytes - bout_last;
  if(bin[ringIdx] > max)
    max = ((bin[ringIdx] / 1024) + 1) * 1024;

 if(bout[ringIdx] > max)
    max = ((bout[ringIdx] / 1024) + 1) * 1024;

  bin_last = stats->ibytes;
  bout_last = stats->obytes;
  ringIdx = (ringIdx + 1) % MAX_GRAPH_WIDTH;
  paintGraph();
}


void PPPStatsDlg::paintEvent (QPaintEvent *) {
  paintIcon(PPPStats::BytesNone); // correct ?
  if(gpppdata.graphingEnabled())
    paintGraph();
}


void PPPStatsDlg::paintIcon(int status) {

  const QPixmap *pixmap;

  switch(status)
    {
    case PPPStats::BytesIn:
      pixmap = &big_modem_left_pixmap;
      break;
    case PPPStats::BytesOut:
      pixmap = &big_modem_right_pixmap;
      break;
    case PPPStats::BytesBoth:
      pixmap = &big_modem_both_pixmap;
      break;
    case PPPStats::BytesNone:
    default:
      pixmap = &big_modem_none_pixmap;
      break;
    }

  bitBlt(pixmap_l, 0, 0, pixmap);

  update_data();
}


void PPPStatsDlg::timeclick() {
  // volume accounting
  switch(gpppdata.VolAcctEnabled()) {
  case 0: // no accounting
    break;

  case 1: // bytes in
    stats->totalbytes = gpppdata.totalBytes() + stats->ibytes;
    break;

  case 2:
    stats->totalbytes = gpppdata.totalBytes() + stats->obytes;
    break;

  case 3:
    stats->totalbytes = gpppdata.totalBytes() + stats->ibytes + stats->obytes;
    break;
  }
}


void PPPStatsDlg::closeEvent(QCloseEvent *) {
  emit cancel();
}


void PPPStatsDlg::update_data() {
  timeclick();

  ibytes_string = KGlobal::locale()->formatNumber(stats->ibytes, 0);
  ipackets_string.setNum(stats->ipackets);
  compressedin_string.setNum(stats->compressedin);
  uncompressedin_string.setNum(stats->uncompressedin);
  errorin_string.setNum(stats->errorin);
  obytes_string = KGlobal::locale()->formatNumber(stats->obytes, 0);
  opackets_string.setNum(stats->opackets);
  compressed_string.setNum(stats->compressed);
  packetsunc_string.setNum(stats->packetsunc);
  packetsoutunc_string.setNum(stats->packetsoutunc);

  labela2[0]->setText(ibytes_string);
  labela2[1]->setText(ipackets_string);
  labela2[2]->setText(compressedin_string);
  labela2[3]->setText(uncompressedin_string);
  labela2[4]->setText(errorin_string);

  labelb2[0]->setText(obytes_string);
  labelb2[1]->setText(opackets_string);
  labelb2[2]->setText(compressed_string);
  labelb2[3]->setText(packetsunc_string);
  labelb2[4]->setText(packetsoutunc_string);

  // if I don't resort to this trick it is imposible to
  // copy/paste the ip out of the lineedits due to
  // reset of cursor position on setText()
  QString local_addr = ( stats->local_ip_address.isEmpty() ?
                         i18n("unavailable") :
                         stats->local_ip_address );

  if( ip_address_label2->text() != local_addr )
    ip_address_label2->setText(local_addr);

  QString remote_addr = ( stats->remote_ip_address.isEmpty() ?
                          i18n("unavailable") :
                          stats->remote_ip_address );

  if( ip_address_label4->text() != remote_addr )
    ip_address_label4->setText(remote_addr);
}


void PPPStatsDlg::toCurrentDesktop() {
  KWindowSystem::setOnDesktop(winId(), KWindowSystem::currentDesktop());
}

#include "pppstatdlg.moc"

