/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "idealretardationtime.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (idealretardationtime)
	
idealretardationtime::idealretardationtime(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, apparentdiffusionanddispersioncoefficient, 0.8);             //m
    Input (double, porewatervelocity, 1.);
    Input (double, Htc3, 0.);                      //Htc1 to Htc10 have units of "m"
    Input (double, Htc2, 0.);
    Input (double, Htc4, 0.);
    Input (double, Htc1, 0.);
    Input (double, Htc5, 0.);
    Input (double, Htc6, 0.);
    Input (double, Htc7, 0.);
    Input (double, Htc8, 0.);
    Input (double, Htc9, 0.);
    Input (double, Htc10, 0.);

    Output (double, Irt3);                      //Irt1 to Irt10 have units of "h"
    Output (double, Irt2);
    Output (double, Irt4);
    Output (double, Irt1);
    Output (double, Irt5);
    Output (double, Irt6);
    Output (double, Irt7);
    Output (double, Irt8);
    Output (double, Irt9);
    Output (double, Irt10);

}

void idealretardationtime::reset() {
    Irt3 = 0.;
    Irt2 = 0.;
    Irt4 = 0.;
    Irt5 = 0.;
    Irt6 = 0.;
    Irt7 = 0.;
    Irt1 = 0.;
    Irt8 = 0.;
    Irt9 = 0.;
    Irt10 = 0.;


}

void idealretardationtime::update() {

    //double x1= std::pow((apparentdiffusionanddispersioncoefficient/(Htc1*porewatervelocity)), 2);
    //Rt1 = (Htc1 > 0) ? ((24*Htc1/porewatervelocity)*
                       //(std::sqrt((1+x1))-(apparentdiffusionanddispersioncoefficient/(Htc1+porewatervelocity)))) : 0;


    Irt1 = (Htc1 > 0) ? ((24. * Htc1/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc1*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc1+porewatervelocity)))) : 0;

    Irt2 = (Htc2 > 0) ? ((24. * Htc2/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc2*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc2+porewatervelocity)))) : 0;

    Irt3 = (Htc3 > 0) ? ((24. * Htc3/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc3*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc3+porewatervelocity)))) : 0;

    Irt4 = (Htc4 > 0) ? ((24. * Htc4/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc4*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc4+porewatervelocity)))) : 0;

    Irt5 = (Htc5 > 0) ? ((24. * Htc5/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc5*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc5+porewatervelocity)))) : 0;

    Irt6 = (Htc6 > 0) ? ((24. * Htc6/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc6*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc6+porewatervelocity)))) : 0;

    Irt7 = (Htc7 > 0) ? ((24. * Htc7/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc7*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc7+porewatervelocity)))) : 0;

    Irt8 = (Htc8 > 0) ? ((24. * Htc8/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc8*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc8+porewatervelocity)))) : 0;

    Irt9 = (Htc9 > 0) ? ((24. * Htc9/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc9*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc9+porewatervelocity)))) : 0;

    Irt10 = (Htc10 > 0) ? ((24. * Htc10/porewatervelocity)*
                        ((std::sqrt(1. + std::pow((apparentdiffusionanddispersioncoefficient/(Htc10*porewatervelocity)),2)))-
                        (apparentdiffusionanddispersioncoefficient/(Htc10+porewatervelocity)))) : 0;

}

} //namespace

