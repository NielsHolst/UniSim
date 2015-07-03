/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_AIR_FLUX_CONVECTION_H
#define VG_AIR_FLUX_CONVECTION_H

#include <usbase/model.h>

namespace vg {

class AirFluxConvection : public UniSim::Model
{
public:
    AirFluxConvection(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double state, airTransmission, exponent,
        volumeBelowRoof, volumeRoof;

    // Outputs
    double downwards, upwards;

    // Data
    double bottomProp;

};
} //namespace


#endif
