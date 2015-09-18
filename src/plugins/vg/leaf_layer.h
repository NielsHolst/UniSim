/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_LAYER_H
#define VG_LEAF_LAYER_H

#include <usbase/model.h>

namespace vg {

class LeafLayer : public UniSim::Model
{
public:
    LeafLayer(UniSim::Identifier name, QObject *parent);

private:
    double xGaussUpperside, wGaussUpperside,
        xGaussLowerside, wGaussLowerside;
};
} //namespace


#endif
