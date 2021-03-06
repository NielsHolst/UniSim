/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "application.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH(Application)
	
Application::Application(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(int, day, 1);            //days
    //Input(double, dosage, 3.);     //kg a.i/ha
    Input(double, ae, 360.);         //active ingredient, g a.e/L
    Input(double, ar, 6.25);         //application rate, L/ha

    Output(double, application);    //Take the value dosage on the day, otherwise zero (kg a.i/ha)
}

void Application::reset() {
    hoursPassed = 0;
    application = 0;
}

void Application::update() {
    ++hoursPassed;
    //application = (hoursPassed == 24*day) ? dosage : 0.;
    application = (hoursPassed == 24*day) ? (ae/0.75 * ar * 0.001) : 0.;
}

} //namespace

