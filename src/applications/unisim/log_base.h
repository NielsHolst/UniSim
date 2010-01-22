/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_SIM_LOG_BASE_H
#define UniSim_SIM_LOG_BASE_H

#include <QList>
#include <QString>

class LogBase
{
public:
	struct LogItem {
		QString title, text;
	};
	
	LogBase();
    virtual ~LogBase() { }
    void tell(LogItem item);
	void clear();
	void removeAt (int index);
	const QList<LogItem>* items() const;
	QString allAsText() const;

private:
	virtual void doTell(LogItem item) { }
	virtual void doClear() { }
	virtual void doRemoveAt (int index) { }

	QList<LogItem> _items;
};	

#endif

