/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATION_MAX_H
#define VG_VENTILATION_MAX_H

#include <usbase/model.h>

namespace vg {

class VentilationMax : public UniSim::Model
{
public:
    VentilationMax(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double radiation, dayMax, nightMax, thresholdRadiation;
    // Variables
    double signal;
};
} //namespace


#endif
