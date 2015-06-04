/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAYER_PHOTOSYNTHESIS_H
#define VG_LAYER_PHOTOSYNTHESIS_H


#include <usbase/model.h>

namespace vg {

class LayerPhotosynthesis : public UniSim::Model
{
public:
    LayerPhotosynthesis(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double Pnmax,Pgmax, LUE, radiationDif, radiationDir, Kdif, SCV,
    xGauss, wGauss, lai, Rd, sinB;
    double Pg, Pn;
};
} //namespace


#endif
