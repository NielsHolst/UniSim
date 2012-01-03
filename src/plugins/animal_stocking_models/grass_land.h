/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANIMALSTOCKING_GRASSLAND
#define ANIMALSTOCKING_GRASSLAND
#include <QObject>
#include <usbase/model.h>

namespace AnimalStocking {

class GrassLand : public UniSim::Model
{
	Q_OBJECT
public: 
	GrassLand(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters

    // pull variables

};

} //namespace
#endif
