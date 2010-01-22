/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_ASSIMILATION_MAX_GIVEN_TEMP
#define INTERCOM_ASSIMILATION_MAX_GIVEN_TEMP
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class AssimilationMaxGivenTemp : public UniSim::Model
{
	Q_OBJECT
public: 
    AssimilationMaxGivenTemp(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double slope, maxAmax;

	// state
    double amax;
    // models
    UniSim::Model *weather;

};

} //namespace
#endif
