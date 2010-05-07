/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_INSECT1
#define BEEHIVE_INSECT1
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class Insect1 : public UniSim::Model
{
	Q_OBJECT
public: 
    Insect1(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double initEggs, sexRatio;

	// links
    UniSim::Model *egg, *larva, *pupa, *adult;

};

} //namespace
#endif
