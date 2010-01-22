/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QCloseEvent>
#include <QMdiArea>
#include "sub_window.h"

SubWindow::SubWindow(QMdiArea *area, QString title)
    : QMdiSubWindow(area), StoredWidget(area, "geometries/" + title),
      _type(Untyped)
{
    Q_ASSERT(area);
    area->addSubWindow(this);
    setWindowTitle(title);

    //useStoredGeometry();
    //useStoredVisibility();
}

void SubWindow::closeEvent(QCloseEvent *event) {
    if (_type == Output) {
        event->ignore();
        hide();
    }
}

void SubWindow::setType(Type type) {
    _type = type;
}

SubWindow::Type SubWindow::type() const {
    return _type;
}

	
