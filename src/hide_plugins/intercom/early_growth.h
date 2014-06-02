/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_EARLY_GROWTH
#define INTERCOM_EARLY_GROWTH
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class EarlyGrowth : public UniSim::Model
{
	//Q_OBJECT
public: 
    EarlyGrowth(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
    void reset();
	void update();

private:
	// parameters
    double initialArea, growthRate;

	// state
    double area;

    // links
    UniSim::Model *photoThermalTime;
};

} //namespace
#endif
