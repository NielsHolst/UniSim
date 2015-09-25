/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_STOMATAL_RESISTANCE_BASE_H
#define VG_STOMATAL_RESISTANCE_BASE_H

#include <usbase/model.h>

namespace vg {

class StomatalResistanceBase : public UniSim::Model
{
public:
    StomatalResistanceBase(UniSim::Identifier name, QObject *parent);
    void reset() final;
    void update() final;

private:
    // Outputs
    double rsH2O, rsCO2;
    // Methods
    virtual double resetRsH2O() = 0;
    virtual double updateRsH2O() = 0;
};
} //namespace


#endif
