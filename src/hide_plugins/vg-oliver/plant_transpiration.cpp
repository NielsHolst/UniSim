/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "plant_transpiration.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::max;

namespace vg {
	
PUBLISH(PlantTranspiration)

/*! \class PlantTranspiration
 * \brief Transpiration from the crop
 *
 * Inputs
 * ------
 * - _lai_ is leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _fractionPlantArea_ is the fraction of the ground area covered by the plant culture [0;1]
 * - _boundaryResistance_ is the boundary layer resistance to water vapour [s/m]
 * - _internalResistance_ is the internal leaf resistance to water vapour [s/m]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _indoorsRadiation_ is the indoors short wave radiation [<SUP>o</SUP>C]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 *
 * Outputs
 * ------
 * - see VapourFlux
 *
 * Default dependencies
 * ------------
 * - a _plant_ model with two ports:
 *   + _lai_ [m<SUP>2</SUP>/m<SUP>2</SUP>]
 *   + _fractionPlantArea_ [0;1]
 * - a _internalResistance_ child model with a _signal_ port (substitutes a plant model)
 * - an _indoors/temperature_ model with a _value_ port
 * - an _indoors/radiation_ model with a _total_ port
 * - an _indoors/humidity_ model with an _ah_ port
 */

PlantTranspiration::PlantTranspiration(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    Input(double, boundaryResistance, 200.);
    InputRef(double, lai, "culture/plant[lai]");
    InputRef(double, fractionPlantArea, "culture/plant[fractionPlantArea]");
    InputRef(double, internalResistance, "./internalResistance[signal]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsRadiation, "indoors/radiation[total]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
}

void PlantTranspiration::update() {
    double laiNet = lai*fractionPlantArea;
    conductance = 2*laiNet/((1 + svpSlope(indoorsTemperature)/Psychr)*boundaryResistance + internalResistance);
    double effAh = sah(indoorsTemperature) +
                   svpSlope(indoorsTemperature)/Psychr*boundaryResistance/2/laiNet*indoorsRadiation/LHe;
    vapourFlux = max(conductance*(effAh - indoorsAh), 0.);
    gain = conductance*effAh;
}

} //namespace

