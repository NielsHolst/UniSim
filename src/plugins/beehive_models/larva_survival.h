/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_LARVA_SURVIVAL
#define BEEHIVE_LARVA_SURVIVAL
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class LarvaSurvival : public UniSim::Model
{
	Q_OBJECT
public: 
    LarvaSurvival(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
    // methods
    double funcResponse(double demand, double attacked);

    // parameters
    double demandRate, attackRate;

    // pull variables
    double survival, eaten, sdRatio;

    // links
    Model *pollen, *larva;
};

} //namespace
#endif
