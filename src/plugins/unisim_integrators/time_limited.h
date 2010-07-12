/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TIME_LIMITED_H
#define UNISIM_TIME_LIMITED_H

#include <usbase/integrator.h>

namespace UniSim{
	
class Model;

class TimeLimited : public Integrator
{
	Q_OBJECT
public:
	//! Constructor
    TimeLimited(Identifier name, QObject *parent=0);
	
	void initialize();
	
    void resetRuns();
    void resetSteps();

	bool nextRun();
	
	bool nextStep();
	
private:
	// Parameters
    double maxTime;

	// Data
    double numRuns;

    // Links
    Model* time;
};
//@}
} //namespace


#endif
