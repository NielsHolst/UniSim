/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "leaf_transpiration.h"
using std::max;

using namespace UniSim;

namespace vg {

PUBLISH(LeafTranspiration)

LeafTranspiration::LeafTranspiration(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    InputRef(double, rbH2O, "../rb[rbH2O]");
    InputRef(double, rsH2O, "../rs[rsH2O]");
    InputRef(double, Tleaf, "../Temperature[value]");
    InputRef(double, Rna, "../Rna[Rna]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, xGauss, "..[xGauss]");
    InputRef(double, wGauss, "..[wGauss]");


    Input(double, fractionPlantArea, 0.95);
    Input(double, lai, 3);

}

void LeafTranspiration::reset() {
}

void LeafTranspiration::update() {
    double laiNet = xGauss*lai*fractionPlantArea;
    conductance = wGauss*2*laiNet/((1 + svpSlope(Tleaf)/Psychr)*rbH2O + rsH2O);
    double effAh = sah(Tleaf) +
                   svpSlope(Tleaf)/Psychr*rbH2O/2/laiNet*Rna/LHe;
    vapourFlux = max(conductance*(effAh - indoorsAh), 0.);
    gain = conductance*effAh;
}


} //namespace

