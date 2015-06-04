/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "leaf.h"

using namespace UniSim;


namespace dynamic_photosynthesis {

Leaf::Leaf(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    Input(double, Sini, 0.); // Initial induction state, Mmol m @Sup 2 s @Sup{-1}
    Input(double, alpha, 0.007); // Response of Seq to PAR, no dimension
    Input(double, S0, 1.); // Asymptotic Seq with respect to PAR, Mmol m @Sup 2 s @Sup{-1}

    Input(double, a, 0.27); // Asymptotic regression coefficient for S60, Mmol m @Sup 2 s @Sup{-1}
    Input(double, b, 0.017); // Regression coefficient for S60, no dimension
    Input(double, c, 0.007); // Regression coefficient for S60, no dimension
    Input(double, gSini, 250); // Initial stomatal conductance, mmol m @Sup 2 s @Sup{-1}
    Input(double, PARmin, 1.); // Value of PAR when measured PAR is zero, Mmol m @Sup 2 s @Sup{-1}

    Input(double, tm, 30); // location of the peak response on the x axis, @Char ring C
    Input(double, ts, 45); // controls the width of the bell, no dimension
    Input(bool, useTemperature, false); // controls the effect of temperature

    Input(double, A0, 12.); // Max assimilation rate, Mmol m @Sup 2 s @Sup{-1}
    Input(double, theta, 0.0035); // Regression coefficient for photosynthesis, Mmol m @Sup 2 s @Sup{-1}

    Output(double, S); // Induction state, Mmol m @Sup 2 s @Sup{-1}
    Output(double, Seq); // Equilibrium state of induction, Mmol m @Sup 2 s @Sup{-1}
    Output(double, S60); // Induction state after 60 seconds, Mmol m @Sup 2 s @Sup{-1}
    Output2(double, A, Ar); // Photosynthesis rate, Mmol m @Sup 2 s @Sup{-1}
}

void Leaf::initialize() {
    weather= seekOne<UniSim::Model*>("weather");
}

void Leaf::reset() {
    S = Seq = Sini;
}

void Leaf::update() {
    double PAR = weather ->pullValue<double>("PAR");
    double temp = weather ->pullValue<double>("temp");
    if (PAR==0.) PAR = PARmin;


    Seq= S0*(1-exp(-alpha*PAR));
    if (useTemperature)
        Seq *= exp(-pow((temp-tm)/ts,2));
    if (Seq>1.) Seq = 1.;


    S60 = a*(1.-exp(-b*PAR))*((1.-exp(-c*gSini)));

    Q_ASSERT(S60<= Seq);
    double beta = -1./60.*log((Seq - S60)/Seq);
    double dt = 1.;
    double DS = (Seq - S)*beta*dt;
    S +=DS;

    A = A0*(1-exp(-theta*PAR))*S;
}

} //namespace

