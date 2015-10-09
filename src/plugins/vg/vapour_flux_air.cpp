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
 * - _airFlux_ is the rate of air exchange [<SUP>h</SUP>]
 * - _outdoorsAh_ is the outdoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _height_ is the average height of the greenhouse [m]
 */

VapourFluxAir::VapourFluxAir(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    Input(double, airFlux, 0.);
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, outdoorsAh, "outdoors[ah]");
    InputRef(double, height, "geometry[indoorsAverageHeight]");
}

void VapourFluxAir::update() {
    conductance = height*airFlux/3600; // m/s = m3/m2 * m3/m3/h / (s/h)
    vapourFlux = conductance*(outdoorsAh-indoorsAh);    // kg/m2/s = m/s * kg/m3
    gain = conductance*outdoorsAh;                      // kg/m2/s = m/s * kg/m3
}

} //namespace

