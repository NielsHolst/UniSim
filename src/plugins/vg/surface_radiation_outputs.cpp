/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "surface_radiation.h"
#include "surface_radiation_outputs.h"

using namespace UniSim;

namespace vg {

/*! \class SurfaceRadiationOutputs
 * \brief Common surface radiation outputs
 *
 * Outputs
 * ------
 * - _lightTransmissivity_ is the transmissivity for diffuse light [0;1]
 * - _directLightTransmissivity_ is the transmissivity for direct light [0;1]
 * - _lwTransmissivity_ is the transmissivity for long-waved radiation [0;1]
 * - _incomingLightAbsorptivity_ is the absorptivity for diffuse light coming from the outdoors direction [0;1]
 * - _incomingLightReflectivity_ is the reflectivity for diffuse light coming from the outdoors direction [0;1]
 * - _incomingDirectLightAbsorptivity_ is the absorptivity for direct light coming from the outdoors direction [0;1]
 * - _incomingDirectLightReflectivity_ is the reflectivity for direct light coming from the outdoors direction [0;1]
 * - _incomingLwAbsorptivity_ is the absorptivity for long-waved radiation coming from the outdoors direction [0;1]
 * - _incomingLwReflectivity_ is the reflectivity for long-waved radiation coming from the outdoors direction [0;1]
 * - _outgoingLightAbsorptivity_ is the absorptivity for diffuse light going in the outdoors direction [0;1]
 * - _outgoingLightReflectivity_ is the reflectivity for diffuse light going in the outdoors direction [0;1]
 * - _outgoingDirectLightAbsorptivity_ is the absorptivity for direct light going in the outdoors direction [0;1]
 * - _outgoingDirectLightReflectivity_ is the reflectivity for direct light going in the outdoors direction [0;1]
 * - _outgoingLwAbsorptivity_ is the absorptivity for long-waved radiation going in the outdoors direction [0;1]
 * - _outgoingLwReflectivity_ is the reflectivity for long-waved radiation going in the outdoors direction [0;1]
 */

SurfaceRadiationOutputs::SurfaceRadiationOutputs(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, lightTransmissivity);
    Output(double, directLightTransmissivity);
    Output(double, lwTransmissivity);
    Output(double, incomingLightAbsorptivity);
    Output(double, incomingLightReflectivity);
    Output(double, incomingDirectLightAbsorptivity);
    Output(double, incomingDirectLightReflectivity);
    Output(double, incomingLwAbsorptivity);
    Output(double, incomingLwReflectivity);
    Output(double, outgoingLightAbsorptivity);
    Output(double, outgoingLightReflectivity);
    Output(double, outgoingDirectLightAbsorptivity);
    Output(double, outgoingDirectLightReflectivity);
    Output(double, outgoingLwAbsorptivity);
    Output(double, outgoingLwReflectivity);
}

void SurfaceRadiationOutputs::resetRadiationOutputs() {
    set(SurfaceRadiation());
}

void SurfaceRadiationOutputs::set(const SurfaceRadiation &rad_) {
    rad = rad_;
    lightTransmissivity = rad.light.tra;
    directLightTransmissivity = rad.directLight.tra;
    lwTransmissivity = rad.ir.tra;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingDirectLightAbsorptivity = rad.directLight.outer.abs;
    incomingDirectLightReflectivity = rad.directLight.outer.ref;
    incomingLwAbsorptivity = rad.ir.outer.abs;
    incomingLwReflectivity = rad.ir.outer.ref;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingDirectLightAbsorptivity = rad.directLight.outer.abs;
    incomingDirectLightReflectivity = rad.directLight.outer.ref;
    outgoingLwAbsorptivity = rad.ir.inner.abs;
    outgoingLwReflectivity = rad.ir.inner.ref;
}

const SurfaceRadiation* SurfaceRadiationOutputs::surfaceRadiation() const {
    return &rad;
}

} //namespace

