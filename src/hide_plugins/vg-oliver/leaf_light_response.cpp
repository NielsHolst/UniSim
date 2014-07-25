/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_light_response.h"
#include "publish.h"
#include <stdlib.h>

using namespace UniSim;
using namespace std;

namespace vg {
	
PUBLISH(LeafLightResponse)



LeafLightResponse::LeafLightResponse(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, rsCO2, "../../rs[rsCO2]");
    InputRef(double, rbCO2, "../../rb[rbCO2]");
    InputRef(double, Tleaf, "../../temperature[value]");
    Input(double, CO2air, 400);
    Input(double, rhoChl, 0.4500);
    Input(double, theta, 0.7);
    Input(double, frPARabs, 0.3);
    Input(double, concEnzyme, 87);


    Output(double, Pnmax);
    Output(double, Pgmax);
    Output(double, Eff);
    Output(double, Rd);
}

void LeafLightResponse::reset() {
    Eff = 0;
    Pnmax = 0;
    Rd = 0;
}

void LeafLightResponse::update() {

    double Rg = 8.314,                        // gas constant [J/mol/K]
    RHO_CO2 = 1.98,                           // density CO2 at T0 [kg/m3], [mg/ml]
    M_CO2 = 44e-3,                            // molar mass CO2 [kg/mol], [mg/umol]
    T0 = 273.15,                              // 273.15 K = 0 oC
    T25 = T0+25,                              // 273.15+25 K = 25 oC
    LGHTCON = 4.59,                           // [??] conversion factor [umol photons/J]
    O2i_ppm = 210,                            // [G94, p.80] O2 partial pressure
                                              // inside stomata [ppm], [mbar], [ml/l]
    KC25 = 310,                               // [G94, p.80] Michaelis Menten constant
                                              // Rubisco carboxilation (CO2) [ubar]
    KO25 = 155,                               // [G94, p.80] Michaelis Menten constant
    tc = 2.5,                                 // [F80, p.89] (kc) turnover number of RuP2
                                              // carboxylase [1/s]

    EC = 59356,                               // KC Rubisco carboxilation
    EO = 35948,                               // KO Rubisco oxygenation
    EVC = 58520,                              // VCmax maximum carboxylation rate
    ED = 66405,                               // RD dark respiration rate
    EJ = 37000,                               // Jmax maximum electron transport rate



    // [F80, p.88-89] constants for optimum curve temperature dependent electron transport rate
    S = 710,                                  // [J/mol/K]
    H = 220000,                               // [J/mol]
    VOC = 0.21,                               // [F80, p.81] VOC=VOmax/VCmax=ko/kc=constant
    RD25 = 1.1;                               // [F80, p.89] dark respiration rate [umol CO2/m2/s]

    double ALPHA,EFF0,VCmax25, Jmax25,Jmax, X, D, D0, KC, KO, VCmax, Q10RD,RHO_CO2T, KM,
            GAMMA,CO2a_max,Rc_CO2,Rtot_CO2,R_CO2, PNC;


    double TleafK=Tleaf+T0;

    RD25 = M_CO2*RD25;                        // dark respiration rate [mg CO2/m2/s]
    ALPHA = (1-frPARabs)/2;                   // electron yield of absorbed photons at low
                                              // light intensities: 2 electrons
                                              // per absorbed photon [umol e-/umol photons]
    ALPHA = (M_CO2/4)*ALPHA;                  // [mg CO2/umol photons]
    EFF0 = ALPHA*LGHTCON;                     // [??] potential light use efficiency in
                                              // absence of oxygen [mg CO2/J]

                                              // sites [umol/gChl]
    VCmax25 = rhoChl*tc*concEnzyme;                  // [F80, p.84] maximum carboxylation rate
                                              // [umol CO2/m2/s]

    Jmax25 = 467*rhoChl;                     // [F80, p.84] maximum electron transport rate
                                              // (on chlorofylbasis) [umol e-/m2/s]


    X = (TleafK-T25)/(TleafK*Rg*T25);          // [F80, p.83] intermediate variable [mol/J]
    D = 1+exp((S-H/TleafK)/Rg);               // [F82, erratum] intermediate variable [-]
    D0 = 1+exp((S-H/T25)/Rg);                 // [G94, p.VII-5] intermediate variable [-]
    D = D/D0;                                 // [G94, p.VII-5] intermediate variable [-]

    KC = KC25*exp(EC*X);                      // [F80, p.83] Michaelis Menten constant
                                              // Rubisco carboxilation (CO2) [ubar]
    KO = KO25*exp(EO*X);                      // [F80, p.83] Michaelis Menten constant
                                              // Rubisco oxygenation (O2) [mbar]
    VCmax = VCmax25*exp(EVC*X);               // [F80, p.83] maximum carboxylation rate
                                              // [umol e-/m2/s]
    Q10RD = exp(13.6e-6*ED);                  // (2.0) Q10 RD
    Rd = RD25*pow(Q10RD,(TleafK-T25)/10);        // dark respiration rate [mg CO2/m2/s]

    Jmax = Jmax25*exp(EJ*X)/D;               // [F80, p.84] maximum electron transport rate
                                              // (on chlorofylbasis) [umol e-/m2/s]
    Jmax = (M_CO2/4)*Jmax;                    // [??  (2.31 at 25 oC)] maximum endogenous
                                              // photosynthetic capacity [mg CO2/m2/s]

    RHO_CO2T = RHO_CO2*T0/TleafK;             // [gaslaw] density CO2 at Tleaf [kg/m3], [mg/ml]

    KM = KC*(1+O2i_ppm/KO)*RHO_CO2T;       // [G94, p.80] Michaelis Menten constant
                                              // Rubisco carboxilation (CO2) [mg CO2/m3]

    GAMMA = KC*O2i_ppm*VOC/(2*KO);           // [F80, p.85] CO2 compensation concentration
                                              // in absence of dark respiration [ppm], [ubar]
    // reduction of light use efficiency by photorespiration, affected by CO2 concentration
    CO2a_max = max(CO2air,GAMMA);           // if CO2a_ppm < GAMMA: no photosynthesis


    // light use efficiency [mg CO2/J]
    Eff = EFF0*(CO2a_max-GAMMA)/(CO2a_max+2*GAMMA);


    Rc_CO2 = (KM/VCmax)/M_CO2;               // carboxylation resistance [s/m]

    R_CO2 = rsCO2+rbCO2;
    Rtot_CO2 = R_CO2+Rc_CO2;                  // [G94, p.79] total resistance to CO2
                                            // diffusion [s/m]

                                              // CO2 limited rate of net photosynthesis
                                              // [mg CO2/m2 leaf/s]
    PNC = (CO2a_max-GAMMA)*RHO_CO2T/Rtot_CO2;
                                              // maximum net assimilation,
                                              // shows saturation with PNC [mg CO2/m2 leaf/s]
    Pnmax = (Jmax+PNC-sqrt(pow((Jmax+PNC),2)-4*theta*Jmax*PNC))/(2*theta);

    Pgmax = Pnmax+Rd;                        // maximum gross assimilation [mg CO2/m2 leaf/s]


}


} //namespace

