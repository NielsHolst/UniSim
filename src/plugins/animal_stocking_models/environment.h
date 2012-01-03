/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANIMALSTOCKING_ENVIRONMENT
#define ANIMALSTOCKING_ENVIRONMENT
#include <QObject>
#include <usbase/model.h>

namespace AnimalStocking {

class Environment : public UniSim::Model
{
	Q_OBJECT
public: 
	Environment(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void reset();
	void update();

private:
	// parameters
	double Ninit, K, r;

	// pull variables
	double density;

};

} //namespace
#endif
