/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTER_OUTPUTS_H
#define VG_SHELTER_OUTPUTS_H

#include <usbase/model.h>


namespace vg {

class ShelterOutputs : public UniSim::Model
{
public:
    ShelterOutputs(UniSim::Identifier name, QObject *parent);
    void reset() final;
    virtual void localReset() {}
protected:
    // Outputs
    double U, airTransmission, haze,
        diffuseLightTransmission, directLightTransmissionAsDirect, directLightTransmissionAsDiffuse;
};
} //namespace


#endif
