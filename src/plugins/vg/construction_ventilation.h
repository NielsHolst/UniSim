/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONSTRUCTION_VENTILATION_H
#define VG_CONSTRUCTION_VENTILATION_H

#include <usbase/model.h>

namespace vg {

class ConstructionVentilation : public UniSim::Model
{
public:
    ConstructionVentilation(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double ventLength, ventWidth, ventMaxOpening, ventDensity;
};
} //namespace


#endif
