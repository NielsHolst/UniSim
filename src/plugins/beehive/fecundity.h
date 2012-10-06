/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_FECUNDITY
#define BEEHIVE_FECUNDITY
#include <QObject>
#include <usbase/model.h>

namespace beehive {

class Fecundity : public UniSim::Model
{
	//Q_OBJECT
public: 
    Fecundity(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double rate, eggMass, sizeThreshold, sexRatio;

	// pull variables
    double number, mass;

    // models
    UniSim::Model *adult, *numberOfAdults;

};

} //namespace
#endif
