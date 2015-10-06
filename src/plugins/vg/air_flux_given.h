/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_AIR_FLUX_GIVEN_H
#define VG_AIR_FLUX_GIVEN_H

#include <usbase/model.h>

namespace vg {

class AirFluxGiven : public UniSim::Model
{
public:
    AirFluxGiven(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double infiltration, crackVentilation, gravitation, transmissivity;

    // Outputs
    double value;
};
} //namespace


#endif
