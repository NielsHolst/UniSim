/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "leaf.h"

using namespace UniSim;


namespace dynamic_photosynthesis {

Leaf::Leaf(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new Parameter<double>("Sini", &Sini, 0., this, "Initial induction state");
    new Parameter<double>("alpha", &alpha, 0.001, this, "Response of Seq to PAR");

    new Parameter<double>("a", &a, 0.001, this, "Regression coefficient for S60");
    new Parameter<double>("b", &b, 0.001, this, "Regression coefficient for S60");
    new Parameter<double>("c", &c, 0.001, this, "Regression coefficient for S60");
    new Parameter<double>("gSini", &gSini, 0.001, this, "Initial stomatal conductance");
    new Parameter<double>("PARmin", &PARmin, 1., this, "Value of PAR when measured PAR is zero");

    new PullVariable<double>("S", &S, this, "Induction state");
    new PullVariable<double>("Seq", &Seq, this, "Equilibrium state of induction");
    new PullVariable<double>("S60", &S60, this, "Induction state after 60 seconds");
}

void Leaf::initialize() {
    weather= seekOne<UniSim::Model*>("weather");
}

void Leaf::reset() {
    S = Seq = Sini;
}

void Leaf::update() {
    double PAR = weather->pullVariable<double>("PAR");
    if (PAR == 0.) PAR = PARmin;

    Seq = alpha*PAR;
    if (Seq > 1.) Seq = 1.;

    S60 = a*(1. - exp(-b*PAR))*((1. - exp(-c*gSini)));
    Q_ASSERT(S60 <= Seq);
    double beta = -1./60.*log((Seq - S60)/Seq);
    double dt = 1.;
    double DS = (Seq - S)*beta*dt;
    S += DS;
}

} //namespace

