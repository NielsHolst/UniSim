/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_GROWTH
#define BEEHIVE_GROWTH
#include <QObject>
#include <usbase/model.h>

namespace beehive {

class Growth : public UniSim::Model
{
	//Q_OBJECT
public: 
    Growth(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double maxGrowthRate, resource, attackRate, egestion;

	// pull variables
    double value;

    // models
    UniSim::Model *consumerMass, *consumerNumber;
    double duration;

};

} //namespace
#endif
