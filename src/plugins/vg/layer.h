/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAYER_H
#define VG_LAYER_H

#include <usbase/model.h>

namespace vg {

class Layer : public UniSim::Model
{
public:
    Layer(UniSim::Identifier name, QObject *parent);

private:
    double xGauss, wGauss;
};
} //namespace


#endif
