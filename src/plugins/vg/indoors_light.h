/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_INDOORS_LIGHT_H
#define VG_INDOORS_LIGHT_H

#include <usbase/model.h>

namespace vg {

class IndoorsLight : public UniSim::Model
{
public:
    IndoorsLight(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double sunlightDiffuse, sunlightDirect, sunlightPropPar,
        growthLigthtsDirect, growthLigthtsPar;

    // Outputs
    double direct, diffuse, total,
           parDirect, parDiffuse, parTotal;
};
} //namespace


#endif
