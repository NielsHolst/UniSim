/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "log_base.h"

LogBase::LogBase() {
}

void LogBase::tell(LogItem item) {
	_items.append(item);
	doTell(item);
}

void LogBase::clear() {
	_items.clear();
	doClear();
}

void LogBase::removeAt (int index) {
	_items.removeAt(index);
	doRemoveAt (index);
}
		
const QList<LogBase::LogItem>* LogBase::items() const {
	return &_items;
}

QString LogBase::allAsText() const {
	QString text;
	for (int i = 0; i < _items.size(); ++i) 
		text += _items.at(i).title + ": " + _items.at(i).text + "\n";
	return text;
}

