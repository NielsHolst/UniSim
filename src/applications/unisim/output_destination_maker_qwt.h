/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_OUTPUT_DESTINATION_MAKER_QWT_H
#define UniSim_OUTPUT_DESTINATION_MAKER_QWT_H

#include <usengine/output_destination_maker.h>

class QMdiArea;

namespace UniSim{
	
class Output;

class OutputDestinationMakerQwt : public OutputDestinationMaker
{
	Q_OBJECT
public:
    OutputDestinationMakerQwt(QMdiArea *mdiArea, QObject *parent=0);
	
private:
    void createDestination(Output *output);
    QMdiArea *_mdiArea;
};

}
#endif
