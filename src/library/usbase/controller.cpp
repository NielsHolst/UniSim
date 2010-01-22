/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QString>
#include "controller.h"

/*! \class UniSim::Controller
    \brief The %Controller class controls the execution of a Simulation

    The two control loops of Simulation::execute() are guarded by the Controller functions: nextRun() and
    nextStep():

    \verbatim
    Simulation::execute() {
        controller->resetRuns();
        while controller->nextRun() {
            reset all components;

            controller->resetSteps();
            while controller->nextStep()
                update all components;
            cleanup all components;
        }
        debrief all components;
    }
    \endverbatim

    You set up the logic for the execution of your simulation by deriving a class from Controller and defining
    nextRun() and nextStep(). Usually you can use one of the Controller classes already defined.

    Controllers are supplied as plugins. If you derive your own Controller classes you should also put them
    in a plugin to be generally available.

    \sa ControllerMakerPlugIn

*/

namespace UniSim{
	
Controller::Controller(Identifier name, QObject *parent)
	: QObject(parent)
{
    setObjectName(name.key());
}

} //namespace

