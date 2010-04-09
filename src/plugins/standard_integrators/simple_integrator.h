/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_SIMPLE_CONTROLLER_H
#define UniSim_SIMPLE_CONTROLLER_H

#include <usbase/integrator.h>

namespace UniSim{
	
//! \addtogroup UniSim
//@{
//! The %SimpleIntegrator class lets the Simulation run once with a fixed number of time steps
/*!
	\htmlonly
		<table class="UniSim_parameters">
            <caption>UniSim parameters used by this class</caption>
			<tr><td class="name>numSteps</td><td>The number of time steps carried out in a Simulation run.</td></tr>
		</table>
	\endhtmlonly
*/
class SimpleIntegrator : public Integrator
{
	Q_OBJECT
public:
	//! Constructor
    /*!
      \param name of this object.
      \param parent of this object.
      \return This object.
    */
    SimpleIntegrator(Identifier name, QObject *parent=0);
	
	//! Initializes this once, before the first execution of the simulation.
	void initialize();
	
    void resetRuns();
    void resetSteps();

	//! Allows Simulation::execute to run the simulation only once
	/*!
		\return False after the first run of the model
	*/
	bool nextRun();
	
	//! Allows Simulation::execute to run the simulation for a fixed number if timesteps
	/*!
		\return False after the last step have been run through
	*/
	bool nextStep();
	
private:
	// Parameters
	int _numSteps;
	// Data
	int _runs, _steps;
};
//@}
} //namespace


#endif
