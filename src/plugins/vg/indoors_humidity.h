/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_MICROCLIMATE_HUMIDITY_H
#define VG_MICROCLIMATE_HUMIDITY_H

#include <usbase/model.h>

namespace vg {

class IndoorsHumidity : public UniSim::Model
{
public:
    IndoorsHumidity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double temperature, maxRh, minDeltaX;

    // Variable
    double rh, vp, mc, vpd,
        rhExcess, vpdInsufficiency;
    bool highRh, lowDeltaX, highHumidity;

    // Methods
    void setBalances();
};

} //namespace

#endif
