/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_RADIATION_H
#define VG_INDOORS_RADIATION_H

#include <usbase/model.h>

namespace vg {

class IndoorsRadiation : public UniSim::Model
{
public:
    IndoorsRadiation(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double diffuseTransmission, directTransmissionAsDirect, directTransmissionAsDiffuse,
        outdoorsDirectRadiation, outdoorsDiffuseRadiation,
        growthLightShortWaveRadiation, growthLightPropDirect;

    // Variables
    double direct, diffuse, total;
};
} //namespace


#endif
