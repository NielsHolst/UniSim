/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef STORED_WIDGET_H
#define STORED_WIDGET_H
#include <QString>

class QSettings;
class QWidget;


class StoredWidget
{
public:
	StoredWidget(QWidget *widget, QString key) ;
	virtual ~StoredWidget();
	void store() const;
	void useStoredGeometry();
	void useStoredVisibility();
	
private:
	QWidget *_widget;
	QString _key;
	QSettings *_settings;
};

#endif
