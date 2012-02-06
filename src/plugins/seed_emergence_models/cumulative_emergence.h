/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEED_EMERGENCE_CUMULATIVE_EMERGENCE
#define SEED_EMERGENCE_CUMULATIVE_EMERGENCE
#include <QObject>
#include <usbase/model.h>

namespace seed_emergence {

class CumulativeEmergence : public UniSim::Model
{
	Q_OBJECT
public: 
    CumulativeEmergence(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double a, b;

    // pull variables
    double accumulated, dailyRate;

    // links
    UniSim::Model *hydrotime;

};

} //namespace
#endif
