/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_SPECIFIC_LEAF_AREA
#define INTERCOM_SPECIFIC_LEAF_AREA
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class SpecificLeafArea : public UniSim::Model
{
	Q_OBJECT
public: 
    SpecificLeafArea(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void update();

private:
	// parameters
    double initial, slope;

	// state
    double sla;

    // models
    UniSim::Model *photoThermalTime;

};

} //namespace
#endif
