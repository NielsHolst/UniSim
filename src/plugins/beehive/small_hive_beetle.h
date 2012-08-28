/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_SMALL_HIVE_BEETLE
#define BEEHIVE_SMALL_HIVE_BEETLE
#include <QObject>
#include <usbase/model.h>

namespace beehive {

class SmallHiveBeetle : public UniSim::Model
{
	Q_OBJECT
public: 
    SmallHiveBeetle(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
	// parameters

	// pull variables

    // models
    QList<Model*> stages, masses, numbers;
    Model *fecundity;
    int numStages;
};

} //namespace
#endif
