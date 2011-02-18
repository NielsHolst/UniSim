/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_INSECT8
#define BEEHIVE_INSECT8
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Stage;
}

namespace beehive{

class Insect8 : public UniSim::Model
{
	Q_OBJECT
public: 
    Insect8(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
    // methods
    void applySurvival(Model *model, Model *survival);

	// parameters
    double initEggs;

	// state
    UniSim::Model *egg, *larva, *fecundity, *eggSurvival, *larvaSurvival;
    QList<UniSim::Stage*> stages;
};

} //namespace
#endif
