/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAMP_BASE_H
#define VG_LAMP_BASE_H

#include <usbase/model.h>

namespace vg {

class LampBase : public UniSim::Model
{
public:
    LampBase(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    virtual void calculate() = 0;
    // Parameters
    bool switchedOn;
protected:
    // Parameters
    double capacity;

    // Variables
    double convectiveHeat, bnLight, rnLight, par, energyUse;

    // Methods
    void setToZero();

};
} //namespace


#endif
