/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_LIFESTAGE
#define BEEHIVE_LIFESTAGE
#include <QObject>
#include <usbase/model.h>

namespace beehive {

class LifeStage : public UniSim::Model
{
	Q_OBJECT
public: 
	LifeStage(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters
    double rate;

	// pull variables
    double size;

    // models
    UniSim::Model *number, *mass, *growth;
};

} //namespace
#endif
