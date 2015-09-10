/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "shelters.h"
#include "surface_radiation.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(Shelters)

/*! \class Shelters
 * \brief Collection of all greenhouse shelters
 */

Shelters::Shelters(Identifier name, QObject *parent)
    : ShelterBase(name, parent)
{
}

#define Pull(p) si.p = shelter->pullValuePtr<double>(#p)

void Shelters::initialize() {
    infos.clear();
    auto shelters = seekChildren<ShelterBase*>("*");
    for (ShelterBase* shelter : shelters) {
        ShelterInfo si;
        si.sr = shelter->surfaceRadiation();
        Pull(diffuseLightTransmitted);
        Pull(directLightTransmitted);
        Pull(totalLightTransmitted);
        Pull(lightAbsorbedCover);
        Pull(lightAbsorbedScreens);
        Pull(haze);
        Pull(U);
        Pull(airTransmissivity);
        Pull(relativeArea);
        infos << si;
    }
}

void Shelters::reset() {
    ShelterBase::reset();
}

#define Accumulate(p) p += (*info.p);
#define AccumulateWeighted(p) p += (*info.p) * (*info.relativeArea);
#define AccumulateSr(p) sr.p += info.sr->p * (*info.relativeArea)

void Shelters::update() {
    SurfaceRadiation sr;
    diffuseLightTransmitted =
    directLightTransmitted =
    totalLightTransmitted =
    lightAbsorbedCover =
    lightAbsorbedScreens = 0.;
    for (ShelterInfo info : infos) {
        AccumulateSr(light.inner.abs);
        AccumulateSr(light.inner.ref);
        AccumulateSr(light.outer.abs);
        AccumulateSr(light.outer.ref);
        AccumulateSr(light.tra);
        AccumulateSr(directLight.inner.abs);
        AccumulateSr(directLight.inner.ref);
        AccumulateSr(directLight.outer.abs);
        AccumulateSr(directLight.outer.ref);
        AccumulateSr(directLight.tra);
        AccumulateSr(ir.inner.abs);
        AccumulateSr(ir.inner.ref);
        AccumulateSr(ir.outer.abs);
        AccumulateSr(ir.outer.ref);
        AccumulateSr(ir.tra);

        Accumulate(diffuseLightTransmitted);
        Accumulate(directLightTransmitted);
        Accumulate(totalLightTransmitted);
        Accumulate(lightAbsorbedCover);
        Accumulate(lightAbsorbedScreens);
        AccumulateWeighted(haze);
        AccumulateWeighted(U);
        AccumulateWeighted(airTransmissivity);
    }
    set(sr);
}

} //namespace

