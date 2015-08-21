/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_RADIATION_ABSORBED_H
#define VG_RADIATION_ABSORBED_H

#include <usbase/model.h>

namespace vg {

class RadiationAbsorbed : public UniSim::Model
{
public:
    RadiationAbsorbed(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double Rna, indoorsLight;
};
} //namespace


#endif