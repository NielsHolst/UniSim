/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MAIN_WINDOW_INTERFACE
#define UNISIM_MAIN_WINDOW_INTERFACE

#include <QString>

namespace UniSim {

class PlotWidget;

class MainWindowInterface {
public:
    virtual PlotWidget* createPlotWidget(QString title) = 0;
    virtual void tile() = 0;
};

} //namespace

#endif
