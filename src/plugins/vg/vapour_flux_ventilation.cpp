/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "vapour_flux_ventilation.h"

using namespace UniSim;

namespace vg {

PUBLISH(VapourFluxVentilation)

/*! \class VapourFluxVentilation
 * \brief Vapour flux through exchange with outdoors air
 *
 * Inputs
 * ------
 * - _ventilationSupply_ is the rate of air exchange with the outdoors [m<SUP>3</SUP>]/m<SUP>3</SUP>/h]
 * - _outdoorsAh_ is the ambient absolute humidity outdoors [kg/m<SUP>3</SUP>]
 * - _indoorsAh_ is the ambient absolute humidity indoors [kg/m<SUP>3</SUP>]
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

VapourFluxVentilation::VapourFluxVentilation(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(double, ventilationSupply, 0.);
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, outdoorsAh,"outdoors[ah]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
}

void VapourFluxVentilation::update() {
    conductance = averageHeight*ventilationSupply/3600; // m/s = m3/m2 * m3/m3/h / (s/h)
    vapourFlux = conductance*(outdoorsAh-indoorsAh);    // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                      // kg/m2/s = m/s * kg/m3
}

} //namespace

