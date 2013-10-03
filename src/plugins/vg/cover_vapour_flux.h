/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_VAPOUR_FLUX_H
#define VG_COVER_VAPOUR_FLUX_H

#include <usbase/model.h>

namespace vg {

class CoverVapourFlux : public UniSim::Model
{
public:
    CoverVapourFlux(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double Tindoors, mcIndoors, rbH2O, diffuseRadiation;

    // Variables
    double value;
};
} //namespace


#endif
