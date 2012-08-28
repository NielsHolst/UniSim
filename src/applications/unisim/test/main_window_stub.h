/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MAIN_WINDOW_STUB
#define UNISIM_MAIN_WINDOW_STUB

#include <QObject>
#include <usbase/main_window_interface.h>

namespace UniSim {

class PlotWidget;

class MainWindowStub : public QObject, public MainWindowInterface {
public:
    PlotWidget* createPlotWidget(QString title) { return 0; }
    void tile() { }
};

} //namespace

#endif
