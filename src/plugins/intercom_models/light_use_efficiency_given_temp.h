/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_LIGHT_USE_EFFICIENCY_GIVEN_TEMP
#define INTERCOM_LIGHT_USE_EFFICIENCY_GIVEN_TEMP
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class LightUseEfficiencyGivenTemp : public UniSim::Model
{
	Q_OBJECT
public: 
    LightUseEfficiencyGivenTemp(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double slope, intercept;

	// state
    double efficiency;

    // models
    UniSim::Model *weather;

};

} //namespace
#endif
