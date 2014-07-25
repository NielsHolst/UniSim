/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAMP_ATTRIBUTES_H
#define VG_LAMP_ATTRIBUTES_H

#include <usbase/model.h>

namespace vg {

class LampAttributes : public UniSim::Model
{
public:
    LampAttributes(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
private:
    // Parameters
    int lampType;
    double capacity, ballastCorrection;

    // Variables
    double heatEmission, longWaveEmission, shortWaveEmission, parEmission, energyUse;

    // Data
    double heatCoef, longWaveCoef, shortWaveCoef, parCoef;
};
} //namespace


#endif
