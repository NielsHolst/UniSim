/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include "log_sub_window.h"

LogSubWindow::LogSubWindow(QMdiArea *area)
	: SubWindow(area, "Log window")
{
    setType(SubWindow::Log);
    setWidget(_edit = new QTextEdit(this));
    _edit->setDocument(_doc = new QTextDocument(this));
    _edit->setEnabled(true);
    _cursor = new QTextCursor(_doc);
}

void LogSubWindow::doTell(LogItem item) {
    _cursor->insertText(item.title + ": " + item.text + "\n");
    if (item.title == "Warning" || item.title == "Error")
        show();
}

void LogSubWindow::doClear() {
    _doc->clear();
}
