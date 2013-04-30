/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_VENTILATION_RH_H
#define VG_SP_VENTILATION_RH_H

#include "sp_ventilation_base.h"

namespace vg {

class SpVentilationRh : public SpVentilationBase
{
public:
    SpVentilationRh(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double spRh, rh, pBand;
    // Methods
    double spDecrement();
};
} //namespace


#endif
