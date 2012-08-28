/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    void update();

    // special methods
    void changePhase(Phase newPhase);
    void reUpdate();

private:
    // methods
    bool startAtOnce() const;
    bool startNow() const;
    void updateCrownZoneArea();
    void updateLightInterception();
    void updateWeight();

    // parameters
    double initWeight, A, phi, F, theta, k, eps, n;
    int initDay, initMonth;

    // pull variables
    double weight, dweight, totalWeight, sz, total_sz, Lz, fz, LA_per_plant, lai;
    int _phase;
    Phase phase;

    // links
    UniSim::Model *weather, *calendar;
    Plant *other;

};

} //namespace
#endif
