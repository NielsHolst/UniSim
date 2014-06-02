/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTS_VENTILATION_H
#define VG_VENTS_VENTILATION_H

#include <usbase/model.h>

namespace vg {

class VentsVentilation : public UniSim::Model
{
public:
    VentsVentilation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double ventLength, ventWidth, ventDensity, ventMaxOpening, roofPitch,
        ventilationLeeSide, ventilationWindSide,
        greenhouseArea, Tindoors, Toutdoors, windspeed;

    // Variables
    double rate;

    // Methods
    double phiTemp(double angle);
    double phiWindLeeSide(double angle);
    double phiWindWindSide(double angle);
};

} //namespace

#endif
