/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONDUCTANCE_PLANT
#define CONDUCTANCE_PLANT
#include <QObject>
#include <usbase/model.h>
#include "phase.h"

namespace conductance {

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
	Plant(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();

    // special methods
    void changePhase(Phase newPhase);
    void updateByDt(double dt);

private:
    // methods
    void updateCrownZoneArea();
    void updateLightInterception();
    void updateWeight(double dt);

    // parameters
    double initWeight, A, phi, F, theta, k, eps, n;

	// state
    double weight, dweight, totalWeight, sz, total_sz, Lz, fz, _phase, LA_per_plant;
    Phase phase;

    // links
    UniSim::Model *weather;
    Plant *other;

};

} //namespace
#endif
