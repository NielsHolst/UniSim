/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SKY_H
#define VG_SKY_H

#include <usbase/model.h>

namespace vg {

class Sky : public UniSim::Model
{
public:
    Sky(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double Tair, rh, slope, intercept;

    // Variables
    double temperature, emissivity;

    // Links
    const double *recordedTsky;

};
} //namespace


#endif
