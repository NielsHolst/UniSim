/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_PLOT
#define UNISIM_OUTPUT_PLOT

#include <QColor>
#include <QList>
#include <usbase/output.h>

namespace UniSim{

class PlotWidget;

class OutputPlot : public Output
{
	Q_OBJECT
public:
    OutputPlot(Identifier name, QObject *parent=0);
    ~OutputPlot();
    // standard methods
	void initialize();
    void cleanup();
	
private:
    // parameters
    QString title;

    // housekeeping
    PlotWidget *plotWidget;
    static QList<QColor> colors;

    // methods
    void createPlotWidget();
    void fillPlotWidget();
    void showPlotWidget();
    bool isEmpty() const;
};

} //namespace


#endif
