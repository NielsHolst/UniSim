/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FLOOR_TEMPERATURE_H
#define VG_FLOOR_TEMPERATURE_H

#include <usbase/model.h>

namespace vg {

class FloorTemperature : public UniSim::Model
{
public:
    FloorTemperature(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double initValue, soilTemperature, Cfloor, Usoil, fluxToFloor;

    // Variables
    double value;
};
} //namespace


#endif
