/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TEST_COST_H
#define UNISIM_TEST_COST_H
#include <QObject>
#include <usbase/model.h>

namespace test{

class Cost : public UniSim::Model
{
public: 
    Cost(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void reset();
    void update();

private:
    // parameters
    int energy, labour, CO2;
    double energyRate, labourRate, CO2Rate;
    // pull variables
    double energyCost, labourCost, CO2Cost;
};

} //namespace
#endif
