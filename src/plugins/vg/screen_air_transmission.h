/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_AIR_TRANSMISSION_H
#define VG_SCREEN_AIR_TRANSMISSION_H

#include <usbase/model.h>

namespace vg {

class ScreenAirTransmission : public UniSim::Model
{
public:
    ScreenAirTransmission(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double state, airTransmission, exponent,
        volumeBelowRoof, volumeRoof;

    // Outputs
    double transmitted, notTransmitted;

    // Data
    double bottomProp;

};
} //namespace


#endif
