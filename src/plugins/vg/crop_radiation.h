/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_RADIATION_H
#define VG_CROP_RADIATION_H

#include <usbase/model.h>

namespace vg {

class CropRadiation : public UniSim::Model
{
public:
    CropRadiation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double kDiffuse, scattering, sinB,
        lightDiffuse, lightDirect,
        absorptivityTop, absorptivityMiddle, absorptivityBottom,
        lai;

    // Outputs
    double
        kDirect, kDirectDirect,
        diffuseReflectivity, directReflectivity,
        reflectivity;
};
} //namespace


#endif
