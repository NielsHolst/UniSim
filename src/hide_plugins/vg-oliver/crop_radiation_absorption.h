/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_RADIATION_ABSORPTION_H
#define VG_CROP_RADIATION_ABSORPTION_H

#include <usbase/model.h>

namespace vg {

class CropRadiationAbsorption : public UniSim::Model
{
public:
    CropRadiationAbsorption(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double Tindoors, Tcover, Tscreen, diffuseRadiation, screenTransmission,
    lampShortWaveEmission, lampLongWaveEmission;

    // Variables
    double shortWave, longWave;
};
} //namespace


#endif
