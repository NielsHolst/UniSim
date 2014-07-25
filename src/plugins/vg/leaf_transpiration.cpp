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

/*! \class LeafTranspiration
 * \brief Transpiration from a canopy layer
 *
 * Inputs
 * ------
 * - _lai_ is leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _fractionPlantArea_ is the fraction of the ground area covered by the plant culture [0;1]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _xGauss_ is the LAI coefficient [0;1]
 * - _wGauss_ is weighing coefficient [0;1]
 * - _absorbedRadiation_ is the absorbed radiation [W/m<SUP>2</SUP>]
 * - _Tleaf_ is leaf temperature [<SUP>o</SUP>C]
 * - _rbH2O_ is the boundary layer resistance to water vapour [s/m]
 * - _rsH2O_ is the internal leaf resistance to water vapour [s/m]
 *
 * Outputs
 * ------
 * - see VapourFlux
 *
 * Default dependencies
 * ------------
 * - an _indoors/humidity_ model with an _ah_ port [kg/m<SUP>3</SUP>]
 * - a parent model with _xGauss_ and _wGauss_ ports [0;1]
 * - an _rna_ sibling model with an _rna_ port [W/m<SUP>2</SUP>]
 * - a _Tleaf_ sibling model with a _temperature_ port [<SUP>o</SUP>C]
 * - an _rb_ sibling model with an _rbH2O_ port [s/m]
 * - an _rs_ sibling model with an _rsH2O_ port [s/m]
 */

LeafTranspiration::LeafTranspiration(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    Input(double, lai, 3);
    Input(double, fractionPlantArea, 0.95);
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, xGauss, "..[xGauss]");
    InputRef(double, wGauss, "..[wGauss]");
    InputRef(double, absorbedRadiation, "../Rna[Rna]");
    InputRef(double, Tleaf, "../Temperature[value]");
    InputRef(double, rbH2O, "../rb[rbH2O]");
    InputRef(double, rsH2O, "../rs[rsH2O]");
}

void LeafTranspiration::reset() {
}

void LeafTranspiration::update() {
    double laiNet = xGauss*lai*fractionPlantArea;
    conductance = wGauss*2*laiNet/((1 + svpSlope(Tleaf)/Psychr)*rbH2O + rsH2O);
    double effAh = sah(Tleaf) +
                   svpSlope(Tleaf)/Psychr*rbH2O/2/laiNet*absorbedRadiation/LHe;
    vapourFlux = max(conductance*(effAh - indoorsAh), 0.);
    gain = conductance*effAh;
}


} //namespace

