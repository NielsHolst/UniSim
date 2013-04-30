/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_H
#define VG_STOMATAL_RESISTANCE_H

#include <usbase/model.h>

namespace vg {

class StomatalResistance : public UniSim::Model
{
public:
    StomatalResistance(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double rh, rbH2O, co2, Pgc;

    // Variables
    double riH2O;

};
} //namespace


#endif
