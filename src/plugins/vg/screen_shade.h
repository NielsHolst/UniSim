/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_SHADE_H
#define VG_SCREEN_SHADE_H

#include <usbase/model.h>

namespace vg {

class ScreenShade : public UniSim::Model
{
public:
    ScreenShade(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    double globRad, Tindoors, spLight, spTemperature, spScreenEnergy;

    // Variable
    double sp;

    // Links
    const int *type;
    const double *spMaxOpening;
    const bool *spHumidityPassed;

};
} //namespace


#endif
