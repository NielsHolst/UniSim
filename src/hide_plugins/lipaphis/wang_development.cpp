/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "wang_development.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(WangDevelopment)

WangDevelopment::WangDevelopment(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, T, 25.);
    Input(double, Tlow, 0.);
    Input(double, Topt, 20.);
    Input(double, Tmax, 35.);
    Input(double, r, 0.17);
    Input(double, delta, 4.);
    Input(double, H, 26.);
    Output(double, step);
}

void WangDevelopment::reset() {
    step = 0.;
}

void WangDevelopment::update() {
    double a = H/(1+exp(-r*(T-Topt))),
           b = (1. - exp(-(T-Tlow)/delta)),
           c = (1. - exp(-(Tmax-T)/delta));
    step = a*b*c;
}

} //namespace

