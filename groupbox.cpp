/////////////////////////////////////////////////////////////////////////////
//
// A QGroupBox like box with title, optional a checkbox and a peer window 
// inside
//
/////////////////////////////////////////////////////////////////////////////

#include "groupbox.h"
#include <qlabel.h>

#define BORDER_X 6
#define BORDER_Y 6
#define INNER_X  6
#define INNER_Y  6

KGroupBoxBase::KGroupBoxBase(QWidget *parent, const char *name) :
  QFrame(parent, name),
  _title(0)
{
  setFrameStyle(Box | Sunken);
  _peer = new QWidget(this);
}


void KGroupBoxBase::show() {
  QSize s = peer()->minimumSize();
  s += 2*QSize(BORDER_X + INNER_X, BORDER_Y + INNER_Y);
  if(_title != 0)
    s.setHeight(s.height() + _title->sizeHint().height());
  setMinimumSize(s);
  QFrame::show();
}


QSize KGroupBoxBase::minimumSize() const {
  return QSize(0, 0);
}


QSize KGroupBoxBase::maximumSize() const {
  return QSize(1 << 15, 1 << 15);
}


QSize KGroupBoxBase::sizeHint() const {
  return minimumSize();
}


void KGroupBoxBase::resizeEvent(QResizeEvent *) {
  initBox();
}


void KGroupBoxBase::initBox() {
  QRect peergeo;
  
  frect = QRect(BORDER_X, BORDER_Y, 
		width() - 2*BORDER_X, height() - 2*BORDER_Y);

  peergeo = QRect(frect.left() + INNER_X,
		  frect.top() + INNER_Y,
		  frect.width() - (2*INNER_X) - 1,
		  frect.height() - (2*INNER_Y) - 1);

  // adjust frame to fit title
  if(_title != 0) {
    frect.setTop(frect.top() + _title->minimumSize().height()/2);
    peergeo.setTop(peergeo.top() + _title->minimumSize().height());
    _title->resize(_title->minimumSize().width() + 8,
		   _title->minimumSize().height());
    _title->move(peergeo.left() + INNER_X,
		 frect.top() - _title->minimumSize().height()/2);
  }

  setFrameRect(frect);
  _peer->setGeometry(peergeo);
}


void KGroupBoxBase::setTitleWidget(QWidget *title) {
  if(_title)
    delete _title;

  if(title) {
    QFont f = font();
    f.setBold(true);
    title->setFont(f);
  }

  _title = title;
  initBox();  
}


KGroupBox::KGroupBox(const char *title, QWidget *parent, const char *name) :
  KGroupBoxBase(parent, name)
{
  setTitle(title);
}


void KGroupBox::setTitle(const char *name) {
  QLabel *l = new QLabel(name, this);
  l->setMinimumWidth(l->sizeHint().width()+8);
  l->setMinimumHeight(l->sizeHint().height());
  l->setAlignment(AlignCenter);
  setTitleWidget(l);
}


KCheckGroupBox::KCheckGroupBox(const char *title, QWidget *parent, const char *name) :
  KGroupBoxBase(parent, name)
{
  setTitle(title);
}


void KCheckGroupBox::setTitle(const char *name) {
  cbox = new QCheckBox(name, this);
  cbox->setMinimumWidth(cbox->sizeHint().width()+8);
  cbox->setMinimumHeight(cbox->sizeHint().height());
  connect(cbox, SIGNAL(toggled(bool)),
	  this, SLOT(slot_toggled(bool)));
  setTitleWidget(cbox);  
}


void KCheckGroupBox::slot_toggled(bool b) {
  emit toggled(b);
}


void KCheckGroupBox::setChecked(bool b) {
  cbox->setChecked(b);
}


bool KCheckGroupBox::isChecked() {
  return cbox->isChecked();
}


KRadioGroupBox::KRadioGroupBox(const char *title, QWidget *parent, const char *name) :
  KGroupBoxBase(parent, name)
{
  setTitle(title);
}


void KRadioGroupBox::setTitle(const char *name) {
  rb = new QRadioButton(name, this);
  rb->setMinimumWidth(rb->sizeHint().width()+8);
  rb->setMinimumHeight(rb->sizeHint().height());
  connect(rb, SIGNAL(toggled(bool)),
	  this, SLOT(slot_toggled(bool)));
  setTitleWidget(rb);
}


void KRadioGroupBox::slot_toggled(bool b) {
  emit toggled(b);
}


bool KRadioGroupBox::isChecked() {
  return rb->isChecked();
}


#include "groupbox.moc"
