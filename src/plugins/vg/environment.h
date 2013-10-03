/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENVIRONMENT_H
#define VG_ENVIRONMENT_H

#include <usbase/model.h>

namespace vg {

class Environment : public UniSim::Model
{
public:
    Environment(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Parameters
    double temperature, skyTemperature, windSpeed, windDirection, rh,
    radiation, diffuseRadiation, co2;

    // Variables
    double directRadiation, vp, mc;

};
} //namespace


#endif
