/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN
#define ECOTOX_POLLEN
#include <QObject>
#include <usbase/model.h>

namespace ecotox {

class Pollen : public UniSim::Model
{
	Q_OBJECT
public: 
	Pollen(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // parameters
    double distance, distanceExp, distanceMin, halfDist, maxTotal;

    // pull variables
    double
    pollenDepositionRate, pollenDepositionTotal,
    pollenDensity;

    // links
    const double *depositionRate, *lossRate;

    // methods
    double distanceEffect();
};

} //namespace
#endif
