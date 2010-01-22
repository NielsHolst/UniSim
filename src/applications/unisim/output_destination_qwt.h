/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_OUTPUT_DESTINATION_QWT_H
#define UniSim_OUTPUT_DESTINATION_QWT_H

#include <QList>
#include <usengine/output_destination.h>

class QTextStream;

class SubWindow;
class QwtPlot;
class QColor;

namespace UniSim{

class Output;
class XYStateVariables;

class OutputDestinationQwt : public OutputDestination
{
	Q_OBJECT
public:
    OutputDestinationQwt(QwtPlot *plot, SubWindow *subWindow, Output *output);
       
private:
    void showTitle();
    void showLabels();
    void showData();

    QwtPlot *_plot;
    SubWindow *_subWindow;
    static int _count;
    static QList<QColor> _colors;
};

}
#endif
