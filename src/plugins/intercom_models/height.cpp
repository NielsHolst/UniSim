/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "height.h"

using namespace UniSim;

namespace intercom{

Height::Height(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("h0", &h0, 0.05, this, "Height at time zero (m)");
    new Parameter<double>("hmax", &hmax, 1.0, this, "Maximum height (m)");
    new Parameter<double>("slope", &slope, 0.0085, this, "Growth rate (per photothermal time unit)");
    new Parameter<double>("tm", &tm, 624., this, "Time when @Math {height = h0 + (hmax - h0) slash 2 }");

    new PullVariable<double>("height", &height, this, "Current height (m)");
}

void Height::initialize() {
    photoThermalTime = seekOneNearest<Model*>("time");
}

void Height::reset() {
    updateHeight(0.);
}

void Height::update() {
    double pt = photoThermalTime->pullVariable<double>("total");
    updateHeight(pt);
}

void Height::updateHeight(double time) {
    height = h0 + (hmax - h0)*(f(time) - f(0))/(1 - f(0));
}

double Height::f(double time) {
    return 1./(1. + exp(-slope*(time-tm)));
}

} //namespace

