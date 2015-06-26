/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "vapour_flux_air.h"

using namespace UniSim;

namespace vg {

PUBLISH(VapourFluxAir)

/*! \class VapourFluxAir
 * \brief Vapour flux through air exchange
 *
 * Inputs
 * ------
 * - _airFlux_ is the rate of air exchange between compartments [m<SUP>3</SUP>]/m<SUP>3</SUP>/h]
 * - _donorAh_ is the ambient absolute humidity of the donor compartment [kg/m<SUP>3</SUP>]
 * - _receiverAhAh_ is the ambient absolute humidity of the receiver compartment [kg/m<SUP>3</SUP>]
 * - _averageHeight_ is the (average) height of the receiving compartment [m]
 *
 * Outputs
 * ------
 * - see VapourFlux
 *
 * Default dependencies
 * ------------
 * - an _indoors/humidity_ model with an _ah_ port [kg/m<SUP>3</SUP>]
 * - an _outdoors_ model with an _ah_ port [kg/m<SUP>3</SUP>]
 */

VapourFluxAir::VapourFluxAir(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(double, airFlux, 0.);
    Input(double, receiverAh, 0.);
    Input(double, donorAh, 0.);
    Input(double, averageHeight, 0.);
}

void VapourFluxAir::update() {
    conductance = averageHeight*airFlux/3600; // m/s = m3/m2 * m3/m3/h / (s/h)
    vapourFlux = conductance*(donorAh-receiverAh);    // kg/m2/s = m/s * kg/m3
    gain = conductance*donorAh;                      // kg/m2/s = m/s * kg/m3
}

} //namespace

