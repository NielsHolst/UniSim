/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QSettings>
#include <QWidget>
#include "stored_widget.h"

StoredWidget::StoredWidget(QWidget *widget, QString key) 
	:_widget(widget), _key(key)
{
	Q_ASSERT(widget);
	_settings = new QSettings;
}

StoredWidget::~StoredWidget()
{
	store();
	delete _settings;
}

void StoredWidget::store() const
{
	_settings->setValue(_key+"/pos", _widget->pos());
	_settings->setValue(_key+"/size", _widget->size());
	_settings->setValue(_key+"/isVisible", _widget->isVisible());	
}

void StoredWidget::useStoredGeometry()
{
	QPoint pos = _settings->value(_key+"/pos", QPoint(20, 20)).toPoint();
	QSize size = _settings->value(_key+"/size", QSize(600, 400)).toSize();
	_widget->resize(size);
	_widget->move(pos);
}

void StoredWidget::useStoredVisibility()
{
	// hides all MDI sub-windows!!
/*	bool isVisible = _settings->value(_key+"/isVisible", false).toBool();
	if (isVisible)
		_widget->show();
	else
		_widget->hide(); */
}

