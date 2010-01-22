/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_PTREPRODUCTION
#define GRAINSTORE_PTREPRODUCTION
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Stage;
}

namespace grainstore{

class PtReproduction : public UniSim::Model
{
	Q_OBJECT
public: 
	PtReproduction(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double sexRatio, tempMax, c1, c2, ageOpt, ageDisp,
        rm_a, rm_b, rm_opt,
        scale_a, scale_b, scale_c;
    int delay;
    bool applyDensityDependence;

    // derived parameters
    double lowCut, highCut, scaleHigh;

    // state
    double fecundity, _densityDependence;

    // derived state
    double temp;

    // models
    UniSim::Model *weather, *egg;
    UniSim::Stage *adult;

    // methods
    double fecundityAtAge(double age);
    double extrValue(double a, double b, double c, double x) const;
    double densityDependence(double density) const;


};

} //namespace
#endif
