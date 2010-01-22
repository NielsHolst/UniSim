/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_PROSTEPHANUS
#define GRAINSTORE_PROSTEPHANUS
#include <QObject>
#include <usbase/model.h>

namespace grainstore{

class Prostephanus : public UniSim::Model
{
	Q_OBJECT
public: 
	Prostephanus(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double lossDemandRate, lossSearchRate, damageDemandRate, damageSearchRate;

	// state

    // models
    UniSim::Model *immigration, *development, *reproduction, *emigration;
};

} //namespace
#endif
