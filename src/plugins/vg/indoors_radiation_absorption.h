/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_RADIATION_ABSORPTION_H
#define VG_INDOORS_RADIATION_ABSORPTION_H

#include <usbase/model.h>

namespace vg {

class IndoorsRadiationAbsorption : public UniSim::Model
{
public:
    IndoorsRadiationAbsorption(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double Tcover, Tscreen, bnLamps, rnLamps, spScreen, Tindoors,
        incomingRadiation, longwaveEmission, shortwaveEmission;
    // Variables
    double bnAbsorbed, rnAbsorbed;
};
} //namespace


#endif
