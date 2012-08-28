/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LAKEOXYGEN_OXYGENCONCENTRATION
#define LAKEOXYGEN_OXYGENCONCENTRATION
#include <QObject>
#include <usbase/model.h>

namespace lakeoxygen {

class OxygenConcentration : public UniSim::Model
{
	Q_OBJECT
public: 
	OxygenConcentration(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize(); //Has been added
	void update();

private:
	// parameters
    double x1, x2, x3, x4;

    // pull variables
    double doSatConc;

    // Links
    UniSim::Model *weather;

};

} //namespace
#endif
