/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "ventilation_vapour.h"
using namespace UniSim;

namespace vg {

PUBLISH(VentilationVapour)

/*! \class VentilationVapour
 * \brief Total water vapour ventilation
 *
 * Inputs
 * ------
 * - _ventilationRate_ is the rate of air exchange through ventilation [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
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
 * - an _indoors/ventilation_ model with a _rate_ port [m<SUP>3</SUP>]/m<SUP>2</SUP>/h]
 * - an _outdoors_ model with an _ah_ port [kg/m<SUP>3</SUP>]
 */

VentilationVapour::VentilationVapour(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    InputRef(double, ventilationRate,"indoors/ventilation[rate]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, outdoorsAh,"outdoors[ah]");
}

void VentilationVapour::update() {
    conductance = ventilationRate/3600;                 // m/s = m3/m2/s = m3/m2/h / s/h
    vapourFlux = conductance*(indoorsAh-outdoorsAh);    // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                      // kg/m2/s = m/s * kg/m3
}

} //namespace

