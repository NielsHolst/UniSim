/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "leaf_light_response.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(LeafLightResponse)

/*! \class LeafLightResponse
 * \brief Calculates the parameters of the light response curve
 *
 * Inputs
 * ------
 * - _rsCO2_ is the stomatal resistance against CO<SUB>2</SUB> [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 * - _Tleaf_ is the leaf temperature [<SUP>o</SUP>C]
 * - _co2Air_ is ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rhoChl_ is the density of chlorophyll [?]
 * - _theta_ is a shape parameter for the light response curve [-]
 * - _frPARabs_ is the fraction of PAR absorbed [0;1]
 * - _concEnzyme_ is the enzyme concentration [?]
 *
 * Output
 * ------
 * - _LUE_ is the light use efficiency [mg CO<SUB>2</SUB>/J]
 * - _Pnmax_ is the net assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Pgmax_ is the gross assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Rd_ is the dark respiration rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _rtCO2_ is the total resistance against CO<SUB>2</SUB> [s/m]
 *
 */

const double O2i = 210;     // [G94, p.80] O2 partial pressure inside stomata [ppm], [mbar], [ml/l]

LeafLightResponse::LeafLightResponse(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, rsCO2, "../../rs[rsCO2]");
    InputRef(double, rbCO2, "../../rb[rbCO2]");
    InputRef(double, Tleaf, "../../temperature[value]");
    InputRef(double, co2Air, "indoors/co2[value]");
    Input(double, rhoChl, 0.45);
    Input(double, theta, 0.7);
    Input(double, frParAbs, 0.3);
    Input(double, concEnzyme, 87);
    Output(double, LUE);
    Output(double, Pnmax);
    Output(double, Pgmax);
    Output(double, Rd);
    Output(double, rtCO2);
}

void LeafLightResponse::reset() {
    LUE = 0.0155;
    Pnmax = 0.8;
    Rd = 0.015;
    Pgmax = Pnmax + Rd;
}

void LeafLightResponse::update() {
    T25 = 25. + T0;
    TleafK = Tleaf + T0;
    x25 = (Tleaf-25.)/(TleafK*R*T25); // [F80, p.83] intermediate variable [mol/J]
    rhoCo2T = RhoCo2*T0/TleafK;      // [gaslaw] density CO2 at Tleaf [kg/m3], [mg/ml]
    mm.update(x25);

    VCmax = maxCarboxylationRate();
    Jmax = maxPhotosyntheticCapacity();
    KM = RubiscoCarboxylation();
    gamma = co2CompensationConcentration();

    // If CO2a_ppm < gamma: no photosynthesis
    if (co2Air < gamma) {
        LUE = Pnmax = 0.;
    }
    // Reduction of light use efficiency by photorespiration, affected by CO2 concentration
    else {
        LUE = potentialLightUseEfficiency()*(co2Air-gamma)/(co2Air+2*gamma);
        Pnmax = maxNetAssimilation();
    }
    Rd = darkRespirationRate();
    Pgmax = Pnmax + Rd;    // maximum gross assimilation [mg CO2/m2 leaf/s]
}

void LeafLightResponse::MichaelisMenten::update(double x25) {
    const double KC25 = 310,    // [G94, p.80] M-M constantRubisco carboxylation (CO2) [ubar]
                 KO25 = 155,    // [G94, p.80] M-M constant
                 EC = 59356,    // KC Rubisco carboxylation
                 EO = 35948;    // KO Rubisco oxygenation
    KC = KC25*exp(EC*x25);           // [F80, p.83] M-M constant Rubisco carboxylation (CO2) [ubar]
    KO = KO25*exp(EO*x25);           // [F80, p.83] M-M constant Rubisco oxygenation (O2) [mbar]
}

double LeafLightResponse::potentialLightUseEfficiency() {
    const double conversion = 4.59;     // [??] conversion factor [umol photons/J]
    double alpha = (1-frParAbs)/2;      // electron yield of absorbed photons at lowlight intensity:
                                        // 2 electrons per absorbed photon [umol e-/umol photons]
    double alpha2 = (MCo2/4)*alpha;     // [mg CO2/umol photons]
    return alpha2*conversion;           // [??] potential light use efficiency in
                                        // absence of oxygen [mg CO2/J]
}

double LeafLightResponse::maxCarboxylationRate() {
    const double EVC = 58520,               // maximum carboxylation rate
                 tc = 2.5;                  // [F80, p.89] (kc) turnover number of RuP2 carboxylase [1/s]
    double VCmax25 = rhoChl*tc*concEnzyme;  // [F80, p.84] maximum carboxylation rate [umol CO2/m2/s]
    return VCmax25*exp(EVC*x25);            // [F80, p.83] maximum carboxylation rate [umol e-/m2/s]
}

double LeafLightResponse::maxPhotosyntheticCapacity() {
    // [F80, p.88-89] constants for optimum curve temperature dependent electron transport rate
    const double S = 710,                   // [J/mol/K]
                 H = 220000,                // [J/mol]
                 Jmax25 = 467*rhoChl;       // [F80, p.84] maximum electron transport rate
                                            // (on chlorofylbasis) [umol e-/m2/s]
    const double EJ = 37000;                // Jmax maximum electron transport rate
    double D1 = 1 + exp((S - H/TleafK)/R);  // [F82, erratum] intermediate variable [-]
    double D0 = 1 + exp((S - H/T25)/R);     // [G94, p.VII-5] intermediate variable [-]
    double D = D1/D0;                       // [G94, p.VII-5] intermediate variable [-]

    double Jmax = Jmax25*exp(EJ*x25)/D;     // [F80, p.84] maximum electron transport rate
                                            // (on chlorofylbasis) [umol e-/m2/s]
    return MCo2/4*Jmax;                     // [??  (2.31 at 25 oC)] maximum endogenous
                                            // photosynthetic capacity [mg CO2/m2/s]
}

double LeafLightResponse::RubiscoCarboxylation() {
    return mm.KC*(1+O2i/mm.KO)*rhoCo2T;     // [G94, p.80] M-M constant Rubisco carboxylation (CO2) [mg CO2/m3]
}

double LeafLightResponse::co2CompensationConcentration() {
    const double VOC = 0.21;        // [F80, p.81] VOC=VOmax/VCmax=ko/kc=constant
    return mm.KC*O2i*VOC/(2*mm.KO); // [F80, p.85] CO2 compensation concentration in absence of dark respiration [ppm], [ubar]
}

double LeafLightResponse::darkRespirationRate() {
    const double ED = 66405,                 // RD dark respiration rate
                 RD25 = 1.1;                 // [F80, p.89] dark respiration rate [umol CO2/m2/s]
    double Q10 = exp(13.6e-6*ED);            // (2.0) Q10 RD
    return RD25*MCo2*pow(Q10,(Tleaf-25)/10); // dark respiration rate [umol CO2/m2/s * mg CO2/umol CO2 = mg CO2/m2/s]
}

double LeafLightResponse::maxNetAssimilation() {
    double RcCo2 = KM/VCmax/MCo2;     // carboxylation resistance [s/m]
    double RCo2 = rsCO2+rbCO2;
    rtCO2 = RCo2+RcCo2;                  // [G94, p.79] total resistance to CO2 diffusion [s/m]
    double PNC = (co2Air-gamma)*rhoCo2T/rtCO2;
    return (Jmax + PNC - sqrt(sqr(Jmax+PNC) - 4*theta*Jmax*PNC))/(2*theta);
}

} //namespace

