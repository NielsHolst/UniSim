/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TIME_STEP_LIMITED_H
#define UNISIM_TIME_STEP_LIMITED_H

#include <usbase/integrator.h>

namespace UniSim{
	
class Model;

class TimeStepLimited : public Integrator
{
	Q_OBJECT
public:
    TimeStepLimited(Identifier name, QObject *parent=0);
	
    void resetRuns();
    void resetSteps();

	bool nextRun();
	bool nextStep();
	
private:
	// Parameters
    int maxSteps;

	// Data
    bool doRun;
    int numSteps;
};

} //namespace

#endif
