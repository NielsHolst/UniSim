/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FILE_LOCATIONS_SUB_WINDOW_H
#define FILE_LOCATIONS_SUB_WINDOW_H
#include <QObject>
#include "sub_window.h"

class FileLocationsWidget;

class FileLocationsSubWindow : public SubWindow
{
	Q_OBJECT

public:
    FileLocationsSubWindow(QMdiArea *area);
    void setVisible ( bool visible );
private:
    FileLocationsWidget *widget;
};	


#endif
