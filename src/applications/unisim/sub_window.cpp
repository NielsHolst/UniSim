/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QCloseEvent>
#include <QMdiArea>
#include "sub_window.h"

SubWindow::SubWindow(QMdiArea *area, QString title, Type type)
    : QMdiSubWindow(area), _type(type)
{
    Q_ASSERT(area);
    area->addSubWindow(this);
    setWindowTitle(title);

    /* When a window is closed, the default behavior is to hide it;
    ** it is not deleted from memory. For SimulationOutput windows we override
    ** this, so that these windows are deleted when closed.
    */
    if (_type == SimulationOutput)
        setAttribute(Qt::WA_DeleteOnClose);
}

SubWindow::Type SubWindow::type() const {
    return _type;
}

	
