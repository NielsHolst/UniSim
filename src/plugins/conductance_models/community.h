/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONDUCTANCE_COMMUNITY
#define CONDUCTANCE_COMMUNITY
#include <QObject>
#include <usbase/model.h>
#include "phase.h"

namespace conductance {

class Plant;

class Community : public UniSim::Model
{
	Q_OBJECT
public: 
    Community(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
    // methods
    void updateUnlimited();
    void updateUnderCompression();

	// parameters

	// state
    Phase phase;
    int smallest;

    // links
    QList<Plant*> plants;

};

} //namespace
#endif
