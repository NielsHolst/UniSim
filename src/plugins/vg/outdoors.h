/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_OUTDOORS_H
#define VG_OUTDOORS_H

#include <usbase/model.h>

namespace vg {

class Outdoors : public UniSim::Model
{
public:
    Outdoors(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double temperature, skyTemperature, windSpeed, rh,
    radiation, diffuseRadiation, co2, soilTemperature;

    // Variables
    double directRadiation, propDirectRadiation, ah, sh;

};
} //namespace


#endif
