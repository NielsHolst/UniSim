/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QMdiArea>
#include <qwt_plot.h>
#include <usengine/output.h>
#include "output_destination_qwt.h"
#include "output_destination_maker_qwt.h"
#include "sub_window.h"

namespace UniSim{
	
OutputDestinationMakerQwt::OutputDestinationMakerQwt(QMdiArea *mdiArea, QObject *parent)
    : OutputDestinationMaker(parent), _mdiArea(mdiArea)
{	
    Q_ASSERT(_mdiArea);
}

void OutputDestinationMakerQwt::createDestination(Output *output) {
    SubWindow *subWindow = new SubWindow(_mdiArea, output->title());
    subWindow->setType(SubWindow::Output);

    QwtPlot *plot = new QwtPlot(_mdiArea);

    subWindow->setWidget(plot);

    OutputDestination *destination = new OutputDestinationQwt(plot, subWindow, output);

	output->connectVariables(destination);
}

}
