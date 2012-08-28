/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_REPRODUCTION_DEMAND
#define SUPPLYDEMAND_REPRODUCTION_DEMAND
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class ReproductionDemand : public UniSim::Model
{
	Q_OBJECT
public: 
    ReproductionDemand(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void reset();

protected:
	// parameters
    double sexRatio, eggWeight;

    // pull variables
    double value;

    // links
    UniSim::Model *unlaidEggs;
    const double *newAdults, *numAdults, *laidEggs;
};

} //namespace
#endif
