/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANIMALSTOCKING_STOCKINGCAPACITY
#define ANIMALSTOCKING_STOCKINGCAPACITY
#include <QObject>
#include <usbase/model.h>

namespace AnimalStocking {

class StockingCapacity : public UniSim::Model
{
	Q_OBJECT
public: 
	StockingCapacity(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double requirement;

	// pull variables
    double value;

    // links
    UniSim::Model *productivity;
};

} //namespace
#endif
