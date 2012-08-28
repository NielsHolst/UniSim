/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_ENERGY_BUDGET
#define SUPPLYDEMAND_ENERGY_BUDGET
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class EnergyBudget : public UniSim::Model
{
	Q_OBJECT
public: 
    EnergyBudget(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void update();

private:
	// parameters

    // pull variables

    // links

    // methods
};

} //namespace
#endif
