/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_POLLEN
#define BEEHIVE_POLLEN
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class Pollen : public UniSim::Model
{
	Q_OBJECT
public: 
    Pollen(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void reset();
	void update();

private:    
    // parameters
    double initMass, maxMass, r;

    // pull variables
    double mass;

    // push variables
    double loss;
};

} //namespace
#endif
