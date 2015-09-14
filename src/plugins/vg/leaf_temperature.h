/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_TEMPERATURE_H
#define VG_LEAF_TEMPERATURE_H

#include <usbase/model.h>

namespace vg {

class LeafTemperature : public UniSim::Model
{
public:
    LeafTemperature(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double lightAbsorbed, Tgh, RHgh, rsH2O, rbH2O,
        radiationAbsorbed;
    // Outputs
    double value;
};
} //namespace


#endif
