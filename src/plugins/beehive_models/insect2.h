/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_INSECT2
#define BEEHIVE_INSECT2
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class Insect2 : public UniSim::Model
{
	Q_OBJECT
public: 
    Insect2(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double initEggs;

	// state
    UniSim::Model *egg, *larva, *pupa, *adult;

};

} //namespace
#endif
