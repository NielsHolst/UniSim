/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_WETNESS_H
#define VG_CROP_WETNESS_H

#include <usbase/model.h>

namespace vg {

class CropWetness : public UniSim::Model
{
public:
    CropWetness(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double Tgh, Tcrop, rh;

    // Variables
    double Tdew;
    bool isWet;

};

} //namespace

#endif
