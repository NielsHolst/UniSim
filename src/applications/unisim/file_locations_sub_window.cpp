/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGui>
#include "file_locations_sub_window.h"
#include "file_locations_widget.h"

FileLocationsSubWindow::FileLocationsSubWindow(QMdiArea *area)
    : SubWindow(area, "File Locations")
{
    setType(SubWindow::View);
    setWidget(widget = new FileLocationsWidget(this));
}
