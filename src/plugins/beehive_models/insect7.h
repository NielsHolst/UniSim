/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_INSECT7
#define BEEHIVE_INSECT7
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Stage;
}

namespace beehive{

class Insect7 : public UniSim::Model
{
	Q_OBJECT
public: 
    Insect7(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double initEggs;

	// state
    UniSim::Model *egg, *fecundity, *eggSurvival;
    QList<UniSim::Stage*> stages;
};

} //namespace
#endif
