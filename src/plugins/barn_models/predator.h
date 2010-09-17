/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BARN_PREDATOR
#define BARN_PREDATOR
#include <QObject>
#include <usbase/model.h>

namespace barn {

class Predator : public UniSim::Model
{
	Q_OBJECT
public: 
	Predator(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void reset();
	void update();

private:
	// parameters
	double Ninit, K, r;

	// state
	double density;

};

} //namespace
#endif
