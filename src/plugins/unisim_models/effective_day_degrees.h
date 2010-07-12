/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_EFFECTIVE_DAY_DEGREES
#define UNISIM_EFFECTIVE_DAY_DEGREES
#include <QObject>
#include "day_degrees.h"

namespace UniSim{

class EffectiveDayDegrees : public DayDegrees
{
	Q_OBJECT
public: 
    EffectiveDayDegrees(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    virtual void initialize();
    virtual void update();

private:
	// parameters
	double q;
	
	// state

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
