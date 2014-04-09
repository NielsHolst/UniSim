/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "application.h"

using namespace UniSim;

namespace insecticide {
	
Application::Application(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<int>(Name(day), 15, "Day of application after sowing");
    addParameter<double>(Name(dosage), 0., "Dosage (g per ha)");
    addVariable<double>(Name(application), "Take the value dosage on the day, otherwise zero");
}

void Application::reset() {
    daysPassed = 0;
    application = 0;
}

void Application::update() {
    ++daysPassed;
    application = (daysPassed == day) ? dosage : 0.;
}

} //namespace

