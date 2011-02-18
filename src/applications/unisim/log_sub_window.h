/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LOG_SUB_WINDOW_H
#define LOG_SUB_WINDOW_H
#include <QObject>
#include "sub_window.h"
#include "log_base.h"

class QTextCursor;
class QTextDocument;
class QTextEdit;

class LogSubWindow : public SubWindow, public LogBase
{
	Q_OBJECT

public:
	LogSubWindow(QMdiArea *area);
	
private:
	void doTell(LogItem item);
	void doClear();

	QTextDocument *_doc;
	QTextEdit *_edit;
	QTextCursor *_cursor;
};	


#endif

