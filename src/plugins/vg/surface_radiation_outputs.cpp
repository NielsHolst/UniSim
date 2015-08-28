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
 */

SurfaceRadiationOutputs::SurfaceRadiationOutputs(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, lightTransmissivity);
    Output(double, irTransmissivity);
    Output(double, incomingLightAbsorptivity);
    Output(double, incomingLightReflectivity);
    Output(double, incomingIrAbsorptivity);
    Output(double, incomingIrReflectivity);
    Output(double, outgoingLightAbsorptivity);
    Output(double, outgoingLightReflectivity);
    Output(double, outgoingIrAbsorptivity);
    Output(double, outgoingIrReflectivity);
}

void SurfaceRadiationOutputs::resetRadiationOutputs() {
    set(SurfaceRadiation());
}

void SurfaceRadiationOutputs::set(const SurfaceRadiation &rad_) {
    rad = rad_;
    lightTransmissivity = rad.light.tra;
    irTransmissivity = rad.ir.tra;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingIrAbsorptivity = rad.ir.outer.abs;
    incomingIrReflectivity = rad.ir.outer.ref;
    outgoingLightAbsorptivity = rad.light.inner.abs;
    outgoingLightReflectivity = rad.light.inner.ref;
    outgoingIrAbsorptivity = rad.ir.inner.abs;
    outgoingIrReflectivity = rad.ir.inner.ref;
}

const SurfaceRadiation* SurfaceRadiationOutputs::surfaceRadiation() const {
    return &rad;
}

} //namespace

