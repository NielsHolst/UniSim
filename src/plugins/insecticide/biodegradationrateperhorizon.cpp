/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "biodegradationrateperhorizon.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (biodegradationrateperhorizon)
	
biodegradationrateperhorizon::biodegradationrateperhorizon(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, k, 0.0001);            //soil degradation rate constant, h-1
    Input (double, tcf1, 0.); //total correction factor
    Input (double, tcf2, 0.);
    Input (double, tcf3, 0.);
    Input (double, tcf4, 0.);
    Input (double, tcf5, 0.);
    Input (double, tcf6, 0.);
    Input (double, tcf7, 0.);
    Input (double, tcf8, 0.);
    Input (double, tcf9, 0.);
    Input (double, tcf10, 0.);

    Output (double, k1); //biodegradation rate per horizon (h-1)
    Output (double, k2);
    Output (double, k3);
    Output (double, k4);
    Output (double, k5);
    Output (double, k6);
    Output (double, k7);
    Output (double, k8);
    Output (double, k9);
    Output (double, k10);

}

void biodegradationrateperhorizon::reset() {

    k1 = 0.;
    k2 = 0.;
    k3 = 0.;
    k4 = 0.;
    k5 = 0.;
    k6 = 0.;
    k7 = 0.;
    k8 = 0.;
    k9 = 0.;
    k10 = 0.;

}

void biodegradationrateperhorizon::update() {//totalCorrFactor1

    k1 = tcf1 * k;
    k2 = tcf2 * k;
    k3 = tcf3 * k;
    k4 = tcf4 * k;
    k5 = tcf5 * k;
    k6 = tcf6 * k;
    k7 = tcf7 * k;
    k8 = tcf8 * k;
    k9 = tcf9 * k;
    k10 = tcf10 * k;

}

} //namespace

