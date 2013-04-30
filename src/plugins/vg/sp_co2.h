/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_CO2_H
#define VG_SP_CO2_H

#include <usbase/model.h>

namespace vg {

class SpCo2 : public UniSim::Model
{
public:
    SpCo2(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double globRad, spVentilationOpening;

    // Variables
    double sp;
};
} //namespace


#endif
