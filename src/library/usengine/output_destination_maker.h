/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_OUTPUT_DESTINATION_MAKER_H
#define UniSim_OUTPUT_DESTINATION_MAKER_H

#include <usbase/component.h>

namespace UniSim{
	
class Output;
class Simulation;

class OutputDestinationMaker : public Component
{
	Q_OBJECT
public:
	OutputDestinationMaker(QObject *parent=0);
    void createDestinationsForAllOutput(Simulation *simulation);

private:
    virtual void createDestination(Output *output) = 0;
};

} //namespace

#endif
