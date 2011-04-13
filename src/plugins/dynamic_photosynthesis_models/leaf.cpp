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
        new Parameter<double>("Pini", &Pini, 0., this, "Inital induction state");
        new Parameter<double>("phiA", &phiA, 0.0052, this, "Photosynthesis determining curvature factor");
        new Parameter<double>("alpha", &alpha, 0.05, this, "Temperature dependence factor");
        new Parameter<double>("Rd", &Rd, -2., this, "Dark respiration");
        new Parameter<double>("b", &b, 0.03, this, "Increase of Amax with respect to PPFD");
        new Parameter<double>("PPFDmax", &PPFDmax, 800., this, "Saturating light level");
        new Parameter<double>("eqrate", &eqrate, 0.4, this, "Equilibration rate ()");
        new Parameter<double>("widtheqrate", &widtheqrate, 1.24, this, "Width of the equilibrium rate");
        new Parameter<double>("eqmax", &eqmax, 2.06, this, "Maximum equilibrium rate");
        new Parameter<double>("poseqrate", &poseqrate, 76.8, this, "Position of equilibrium rate on x axis");
        new Parameter<double>("AmaxSp", &AmaxSp, 12, this, "Light saturated maximum photosynthesis");
        new PullVariable<double>("P", &P, this, "Induction state");
        new PullVariable<double>("Aind", &Aind, this, "Induction dependent assimilation rate");
        new PullVariable<double>("eqrate", &eqrate, this, "Rate of change of equilibrium (will depend on PPFD and maybe Temperature)");
        new PullVariable<double>("Peq", &Peq, this, "Equilibrium state of induction");
        new PullVariable<double>("Amax", &Amax, this, "Max assimilation rate at current PPFD");
}
void Leaf::initialize() {
    weather= seekOne<UniSim::Model*>("weather");
}

void Leaf::reset() {
    P = Pini;
}

void Leaf::update() {
    double PPFD = weather->pullVariable<double>("PPFD");
    calcPeq(PPFD);
    calceqrate(PPFD);

    double dP;
    double dt = 2.0/60.0;
    dP = eqrate*(Peq - P)*dt;
    P+=dP;
    if (P<0.) P = 0.;
    Aind = Rd + (AmaxSp - Rd)*P*(1-exp(-phiA*PPFD));
}

void Leaf::calcPeq(double PPFD) {
    if (PPFD > PPFDmax)
        PPFD = PPFDmax;
    Amax = calcAmax(PPFD);
    double AmaxSp = calcAmax(PPFDmax);
    Peq=(Amax-Rd)/(AmaxSp-Rd);
    //Peq = (1.-exp(-b*PPFD));
}

double Leaf::calcAmax(double PPFD) {
    return (PPFD == 0.) ? 0 : Rd+b*log(PPFD);

}

void Leaf::calceqrate(double PPFD) {
    double rate= (PPFD == 0.) ? eqrate : eqmax* exp(-0.5*(log(PPFD/poseqrate)/widtheqrate)*(log(PPFD/poseqrate)/widtheqrate));
    eqrate= rate; //  + rate*alpha*(T-20.);
}

} //namespace

