/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_LARVA_TIME
#define GRAINSTORE_LARVA_TIME
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class LarvaTime : public UniSim::Model
{
	Q_OBJECT
public: 
    LarvaTime(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double exponent, minimum, optimum, spread;

	// state
    double step, total;

    // models
    UniSim::Model *weather, *lactin;
};

} //namespace
#endif
