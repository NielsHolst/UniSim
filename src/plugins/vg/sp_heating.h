/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HEATING_H
#define VG_SP_HEATING_H

#include <usbase/model.h>

namespace vg {

class SpHeating : public UniSim::Model
{
public:
    SpHeating(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double globRad, spGlobRad, spHeatingBasis, spHeatingMax, humidityIncrement;

    // Variables
    double sp;
};
} //namespace


#endif
