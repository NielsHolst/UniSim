/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "crop_transpiration.h"

using namespace UniSim;

namespace vg {

PUBLISH(CropTranspiration)

/*! \class CropTranspiration
 * \brief Transpiration variables for the crop
 *
 * Inputs
 * ------
 * - _conductanceIn_ is the vapour flux (condensation) conductance [m/s]
 * - _vapourFluxIn_ is the condensation rate [kg/m<SUP>2</SUP>/s]
 * - _gainIn_ is used to integrate over all water vapour fluxes (GCC, p. 148) [kg/m<SUP>2</SUP>/s]
 *
 * Outputs
 * ------
 * See VapourFlux
 *
 * Default dependencies
 * ------------
 * - a _crop/conductance_ model with a _value_ port [m/s]
 * - a _crop/vapourFlux model with a _value_ port [kg/m<SUP>2</SUP>/s]
 * - a _crop/gain model with a _value_ port [kg/m<SUP>2</SUP>/s]
*/

CropTranspiration::CropTranspiration(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    InputRef(double, conductanceIn, "crop/conductance[value]");
    InputRef(double, vapourFluxIn, "crop/vapourFlux[value]");
    InputRef(double, gainIn, "crop/gain[value]");
}

void CropTranspiration::update() {
    conductance = conductanceIn;
    vapourFlux = vapourFluxIn;
    gain = gainIn;
}


} //namespace

