/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIFE_CYCLE_H
#define LIFE_CYCLE_H

#include <usbase/model.h>

namespace test{

class LifeStage;

class LifeCycle : public UniSim::Model
{
	Q_OBJECT
public:
    LifeCycle(UniSim::Identifier name, QObject *parent=0);
	void initialize();
	void reset();
	void update();
private:
	int counter;
	QList<LifeStage*> stages;
	
	// State
    double sum;
};

}
#endif
