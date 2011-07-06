/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_MAIZE
#define SUPPLYDEMAND_MAIZE
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class Maize : public UniSim::Model
{
	Q_OBJECT
public: 
    Maize(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void update();

private:
	// parameters
    double pollenProduction, depositionRate, btConcentration;
    // pull variables

    // links
    UniSim::Model *sowing, *vegetative, *pollenRelease, *btDeposited;
    const double *vegetativeOutflow, *pollenReleased;
};

} //namespace
#endif
