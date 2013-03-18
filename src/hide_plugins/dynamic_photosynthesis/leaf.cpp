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
    new Parameter<double>("Sini", &Sini, 0., this, "Initial induction state, Mmol m @Sup 2 s @Sup{-1}");
    new Parameter<double>("alpha", &alpha, 0.007, this, "Response of Seq to PAR, no dimension");
    new Parameter<double>("S0", &S0, 1., this, "Asymptotic Seq with respect to PAR, Mmol m @Sup 2 s @Sup{-1}");

    new Parameter<double>("a", &a, 0.27, this, "Asymptotic regression coefficient for S60, Mmol m @Sup 2 s @Sup{-1}");
    new Parameter<double>("b", &b, 0.017, this, "Regression coefficient for S60, no dimension");
    new Parameter<double>("c", &c, 0.007, this, "Regression coefficient for S60, no dimension");
    new Parameter<double>("gSini", &gSini, 250, this, "Initial stomatal conductance, mmol m @Sup 2 s @Sup{-1}");
    new Parameter<double>("PARmin", &PARmin, 1., this, "Value of PAR when measured PAR is zero, Mmol m @Sup 2 s @Sup{-1}");

    new Parameter<double>("tm", &tm, 30, this, "location of the peak response on the x axis, @Char ring C");
    new Parameter<double>("ts", &ts, 45, this, "controls the width of the bell, no dimension");
    new Parameter<bool>("useTemperature", &useTemperature, false, this, "controls the effect of temperature");

    new Parameter<double>("A0", &A0, 12, this, "Max assimilation rate, Mmol m @Sup 2 s @Sup{-1}");
    new Parameter<double>("theta", &theta, 0.0035, this, "Regression coefficient for photosynthesis, Mmol m @Sup 2 s @Sup{-1}");

    new Variable<double>("S", &S, this, "Induction state, Mmol m @Sup 2 s @Sup{-1}");
    new Variable<double>("Seq", &Seq, this, "Equilibrium state of induction, Mmol m @Sup 2 s @Sup{-1}");
    new Variable<double>("S60", &S60, this, "Induction state after 60 seconds, Mmol m @Sup 2 s @Sup{-1}");
    new Variable<double>("Ar", &A, this, "Photosynthesis rate, Mmol m @Sup 2 s @Sup{-1}");
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

