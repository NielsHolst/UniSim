/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
#include "integrator.h"

/*! \class UniSim::Integrator
    \brief The %Integrator class controls the execution of a Simulation

    The two control loops of Simulation::execute() are guarded by the Integrator functions: nextRun() and
    nextStep():

    \verbatim
    Simulation::execute() {
        integrator->resetRuns();
        while integrator->nextRun() {
            reset all components;

            integrator->resetSteps();
            while integrator->nextStep()
                update all components;
            cleanup all components;
        }
        debrief all components;
    }
    \endverbatim

    You set up the logic for the execution of your simulation by deriving a class from Integrator and defining
    nextRun() and nextStep(). Usually you can use one of the Integrator classes already defined.

    Integrators are supplied as plugins. If you derive your own Integrator classes you should also put them
    in a plugin to be generally available.

    \sa IntegratorMakerPlugIn

*/

namespace UniSim{
	
Integrator::Integrator(Identifier name, QObject *parent)
	: QObject(parent)
{
    setObjectName(name.key());
}

} //namespace

