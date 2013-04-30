/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_H
#define VG_BOUNDARY_LAYER_RESISTANCE_H

#include <usbase/model.h>

namespace vg {

class BoundaryLayerResistance : public UniSim::Model
{
public:
    BoundaryLayerResistance(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double windspeed, greenhouseVentilation;

    // Variables
    double rbH2O;

};
} //namespace


#endif
