/*
 * kPPPlogview: a accounting log system for kPPP
 *
 *            Copyright (C) 1998 Mario Weilguni <mweilguni@kde.org>
 *
 * This file has been contributed by Tilo Ulbrich <TiloUlbrich@web.de>
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

#include "export.h"

#include <qpushbutton.h>

/* data struct */
struct {
    int id;
    QString name;
    QString desc;
    QString ext;
  }  ExportFormats [] = {
  { 1, i18n("CSV")  ,i18n("Export to a Textfile. The Semicolon is used as separator.<p></p>Can be used for spreadsheet programs like <i>KSpread</i>."), "csv" },
  { 2, i18n("HTML") ,i18n("Export to a HTML Page.<p></p>Can be used for easy exchange over the <i>Internet</i>."), "html" },
  { 0, 0, 0, 0 }  /* !! don't forget !! */
};


/***** ExportWizard *****/
ExportWizard::ExportWizard(QWidget *parent, const QString &_filePrefix)
  : KWizard(parent, "", true) {
  filePrefix = _filePrefix;

  setCaption(i18n("Export Wizard for kPPP logs"));

  /* format-page */
  formatPage = new QWidget();
  QHBoxLayout *formatLayout = new QHBoxLayout(formatPage);

  typeList = new QListBox(formatPage);
  connect(typeList, SIGNAL(highlighted(int)), SLOT (typeHighlighted(int)));
  typeList->setMinimumSize(50, 200);
  QToolTip::add(typeList, i18n("List with possible output formats"));
  int i=0;
  while (ExportFormats[i].id) { // add each format to the list
    typeList->insertItem(ExportFormats[i].name);
    i++;
  }

  formatLayout->addWidget(typeList);
  formatLayout->addSpacing(10);

  typeInfo = new QLabel(formatPage);
  typeInfo->setAlignment(Qt::AlignTop | Qt::WordBreak);
  typeInfo->setText(i18n("<qt><b>Please choose the output format on the left side.</b></qt>"));
  typeInfo->setMinimumSize(350, 200);
  formatLayout->addWidget(typeInfo);

  addPage(formatPage, i18n("Selection of filetype"));
  setNextEnabled( formatPage, false );
  setHelpEnabled( formatPage, false );


  /* filename-page */
  filenamePage = new QWidget();
  QVBoxLayout *filenameLayout = new QVBoxLayout( filenamePage );

  QLabel *fnLbl = new QLabel(filenamePage);
  fnLbl->setText(i18n("Filename:"));
  filenameLayout->addWidget(fnLbl);

  fnLine = new QLineEdit(filenamePage);
  fnLine->setText(i18n("[No file selected]"));
  fnLine->setReadOnly(true);
  filenameLayout->addWidget(fnLine);
  filenameLayout->addStretch(1);

  fnGet = new QPushButton(filenamePage);
  fnGet->setText(i18n("&Select file..."));
  fnGet->setMaximumWidth(200);
  QToolTip::add(fnGet, i18n("Select the filename of the exported output-file"));
  filenameLayout->addWidget(fnGet);
  connect(fnGet, SIGNAL(clicked()), SLOT(getFilename()));
  filenameLayout->addStretch(2);

  addPage(filenamePage, i18n("Selection of filename"));
  setNextEnabled( filenamePage, false );
  setHelpEnabled( filenamePage, false );
}

void ExportWizard::typeHighlighted(int index) {
  typeInfo->setText("<qt><b>"+ExportFormats[index].name+" File Format</b><p></p>"+ExportFormats[index].desc+"</qt>");
  setNextEnabled(formatPage, true );
}

void ExportWizard::getFilename() {
  int i = typeList->currentItem();

  // prepare filter: e.g.: HTML (*.html *.HTML)
  QString filter = "*." + ExportFormats[i].ext + " *." + ExportFormats[i].ext.upper() + "|" + ExportFormats[i].name +
        " (*." + ExportFormats[i].ext + " *." + ExportFormats[i].ext.upper() + ")";

  filename = KFileDialog::getSaveFileName(filePrefix + "." + ExportFormats[i].ext, filter, 0, i18n("Please choose a File"));
  if (filename.isEmpty()) // no file selected
    return;
  fnLine->setText(filename);
  setFinishEnabled(filenamePage, true);
}

void ExportWizard::accept() {
  typeID = typeList->currentItem();
  hide();
}

/***** Export *****/
Export::Export(const QString &_filename)
  : filename(_filename),
    buffer("")
{
}

Export::~Export()
{
}

bool Export::openFile() {
  fP = new QFile( filename );
  return fP->open( IO_WriteOnly );
}

bool Export::closeFile() {
  bool ok = true;
  if (fP->writeBlock(buffer.latin1(), buffer.length())<0)
    ok = false;
  fP->close();
  return ok;
}


/***** CSVExport *****/
CSVExport::CSVExport(const QString &filename, const QString &_separator)
  : Export(filename),
    separator(_separator)
{
}



void CSVExport::addDataline(const QString &a, const QString &b,
			    const QString &c, const QString &d,
			    const QString &e, const QString &f,
			    const QString &g, const QString &h) {
  buffer+=a + separator +
          b + separator +
          c + separator +
          d + separator +
          e + separator +
          f + separator +
          g + separator +
          h + separator + "\n";
}


/***** HTMLExport *****/
HTMLExport::HTMLExport(const QString &filename, const QString &date)
  : Export(filename) {
  QString title = i18n("Connection log for %1").arg(date);
  buffer = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
  buffer.append("<html>\n<head>\n  <title>"+title+"</title>\n</head>\n<body>\n<h1>"+title+"</h1>\n<table width=\"100%\" border=\"1\">\n");

  trStartCode = "<tr><td>";
  tdCode      = "</td><td>";
  trEndCode   = "</td></tr>";
}

void HTMLExport::addDataline(const QString &a, const QString &b,
			     const QString &c, const QString &d,
			     const QString &e, const QString &f,
			     const QString &g, const QString &h) {
  buffer+=trStartCode +
          a + tdCode  +
          b + tdCode  +
          c + tdCode  +
          d + tdCode  +
          e + tdCode  +
          f + tdCode  +
          g + tdCode  +
          h + trEndCode + "\n";
}

void HTMLExport::finishCode() {
  buffer+="</table>\n</body>\n</html>\n";
}

#include "export.moc"
