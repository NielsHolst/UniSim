/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_EGG_SURVIVAL
#define BEEHIVE_EGG_SURVIVAL
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class EggSurvival : public UniSim::Model
{
	Q_OBJECT
public: 
    EggSurvival(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
    // methods
    double calcSurvival(double temp) const;

    // parameters
    double a, b, c, Tlow;

    // pull variables
    double survival;

    // links
    Model * weather;
};

} //namespace
#endif
