/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "ventilated_latent_heat_converter.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::max;

namespace vg {

PUBLISH(VentilatedLatentHeatConverter)

/*! \class VentilatedLatentHeatConverter
 * \brief Condensation on greenhouse cover
 *
 * Inputs
 * ------
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _volumeFlowRate_ is the rate of air flow through the converter [m<SUP>3</SUP>/s]
 *
 * Outputs
 * ------
 * - see VapourFlux
 *
 * Default dependencies
 * ------------
 */

VentilatedLatentHeatConverter::VentilatedLatentHeatConverter(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    InputRef(double, groundArea, "construction/geometry[groundArea]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    Input(double, volumeFlowRate, 2.5);
}

void VentilatedLatentHeatConverter::update() {
    conductance = volumeFlowRate/groundArea;         // m/s = m3/s  / m2
    vapourFlux = conductance*(indoorsAh-convertedAh());    // kg/m2/s = m/s * kg/m3
    gain = conductance*convertedAh();              // kg/m2/s = m/s * kg/m3
}

double VentilatedLatentHeatConverter::dewRate() {
    // interpolation...
    return 0.004; // kg/s
}

double VentilatedLatentHeatConverter::convertedAh() {
    return indoorsAh - dewRate()/volumeFlowRate; // kg/m3 = kg/s / m3/s
}

} //namespace

