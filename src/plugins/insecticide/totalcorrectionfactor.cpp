/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "totalcorrectionfactor.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (totalcorrectionfactor)
	
totalcorrectionfactor::totalcorrectionfactor(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fsm, 0.);            //moisture correction factor
    Input (double, fssdT, 0.25);  //factor to correct for soil temperature, based on averaged annual soil temperature
    Input (double, fba1, 0.);
    Input (double, fba2, 0.);
    Input (double, fba3, 0.);
    Input (double, fba4, 0.);
    Input (double, fba5, 0.);
    Input (double, fba6, 0.);
    Input (double, fba7, 0.);
    Input (double, fba8, 0.);
    Input (double, fba9, 0.);
    Input (double, fba10, 0.);

    Input (double, depthCorrPerHorizon1, 0.);
    Input (double, depthCorrPerHorizon2, 0.);
    Input (double, depthCorrPerHorizon3, 0.);
    Input (double, depthCorrPerHorizon4, 0.);
    Input (double, depthCorrPerHorizon5, 0.);
    Input (double, depthCorrPerHorizon6, 0.);
    Input (double, depthCorrPerHorizon7, 0.);
    Input (double, depthCorrPerHorizon8, 0.);
    Input (double, depthCorrPerHorizon9, 0.);
    Input (double, depthCorrPerHorizon10, 0.);

    Output (double, tcf1);
    Output (double, tcf2);
    Output (double, tcf3);
    Output (double, tcf4);
    Output (double, tcf5);
    Output (double, tcf6);
    Output (double, tcf7);
    Output (double, tcf8);
    Output (double, tcf9);
    Output (double, tcf10);

}

void totalcorrectionfactor::reset() {

    tcf1 = 0.;
    tcf2 = 0.;
    tcf3 = 0.;
    tcf4 = 0.;
    tcf5 = 0.;
    tcf6 = 0.;
    tcf7 = 0.;
    tcf8 = 0.;
    tcf9 = 0.;
    tcf10 = 0.;

}

void totalcorrectionfactor::update() {

    tcf1 = fba1 * depthCorrPerHorizon1 * fsm * fssdT;
    tcf2 = fba2 * depthCorrPerHorizon2 * fsm * fssdT;
    tcf3 = fba3 * depthCorrPerHorizon3 * fsm * fssdT;
    tcf4 = fba4 * depthCorrPerHorizon4 * fsm * fssdT;
    tcf5 = fba5 * depthCorrPerHorizon5 * fsm * fssdT;
    tcf6 = fba6 * depthCorrPerHorizon6 * fsm * fssdT;
    tcf7 = fba7 * depthCorrPerHorizon7 * fsm * fssdT;
    tcf8 = fba8 * depthCorrPerHorizon8 * fsm * fssdT;
    tcf9 = fba9 * depthCorrPerHorizon9 * fsm * fssdT;
    tcf10 = fba10 * depthCorrPerHorizon10 * fsm * fssdT;

}

} //namespace

