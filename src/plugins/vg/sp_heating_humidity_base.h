/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HEATING_HUMIDITY_BASE_H
#define VG_SP_HEATING_HUMIDITY_BASE_H

#include <usbase/model.h>

namespace vg {

class SpHeatingHumidityBase : public UniSim::Model
{
public:
    SpHeatingHumidityBase(UniSim::Identifier name, QObject *parent);
    void reset();

protected:
    // Parameters
    double spMaxIncrement, pBandRh;
    // Variables
    double spIncrement;
};
} //namespace


#endif
