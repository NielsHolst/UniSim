/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "output.h"
#include "output_destination.h"
#include "output_destination_maker.h"
#include "simulation.h"

namespace UniSim{
	
OutputDestinationMaker::OutputDestinationMaker(QObject *parent)
	: Component("OutputDestinationMaker", parent)
{	
}

void OutputDestinationMaker::createDestinationsForAllOutput(Simulation *simulation) {
    QList<Output*> outputs = find<Output*>("*", simulation);

    for (QList<Output*>::iterator ou = outputs.begin(); ou != outputs.end(); ++ou) {
        createDestination(*ou);
    }
}

} //namespace
